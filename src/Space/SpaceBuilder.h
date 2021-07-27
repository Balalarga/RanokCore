#ifndef SPACEBUILDER_H
#define SPACEBUILDER_H


#include <vector>
#include <QDebug>

#include <CL/cl.h>

template<class T>
struct CubeMatrix
{
    CubeMatrix(cl_uint3 size):
        _size(size),
        _rawPtr(new T[size.x*size.y*size.z])
    {
    }
    ~CubeMatrix()
    {
        delete[] _rawPtr;
    }
    T& At(int x, int y, int z)
    {
        return _rawPtr[x*_size.y*_size.z + y*_size.z + z];
    }
    T& At(int i)
    {
        return _rawPtr[i];
    }

    cl_uint3 GetSize()
    {
        return _size;
    }
    T* GetPointer()
    {
        return _rawPtr;
    }

private:
    T* _rawPtr = 0;
    cl_uint3 _size;
};


struct MimageData
{
    double Cx = -10;
    double Cy = -10;
    double Cz = -10;
    double Cw = -10;
    double Ct = -10;
};


struct SpaceData
{
    SpaceData(cl_uint3 spaceUnits,
              cl_float3 startPoint,
              cl_float3 pointSize):
        spaceUnits(spaceUnits),
        pointSize(pointSize),
        mimageData(nullptr),
        zoneData(nullptr)
    {
        pointHalfSize = {pointSize.x/2.f,
                         pointSize.y/2.f,
                         pointSize.z/2.f};
        this->startPoint.x = startPoint.x + pointHalfSize.x;
        this->startPoint.y = startPoint.y + pointHalfSize.y;
        this->startPoint.z = startPoint.z + pointHalfSize.z;
    }
    ~SpaceData()
    {
        DeleteZoneData();
        DeleteMimageData();
    }
    void CreateMimageData()
    {
        DeleteZoneData();
        if(mimageData)
        {
            auto oldSize = mimageData->GetSize();
            if(oldSize.x == spaceUnits.x &&
                    oldSize.y == spaceUnits.y &&
                    oldSize.z == spaceUnits.z)
                return;
            DeleteMimageData();
        }
        mimageData = new CubeMatrix<MimageData>(spaceUnits);
    }
    void CreateZoneData()
    {
        DeleteMimageData();
        if(zoneData)
        {
            auto oldSize = zoneData->GetSize();
            if(oldSize.x == spaceUnits.x &&
                    oldSize.y == spaceUnits.y &&
                    oldSize.z == spaceUnits.z)
                return;
            DeleteZoneData();
        }
        zoneData = new CubeMatrix<int>(spaceUnits);
    }
    void DeleteMimageData()
    {
        if(mimageData)
        {
            delete mimageData;
            mimageData = nullptr;
        }
    }
    void DeleteZoneData()
    {
        if(zoneData)
        {
            delete zoneData;
            zoneData = nullptr;
        }
    }
    int GetSize()
    {
        return spaceUnits.x*spaceUnits.y*spaceUnits.z;
    }
    cl_float3 GetPos(int i)
    {
        cl_float3 pos;
        pos.x = startPoint.x + pointSize.x * (i / ( spaceUnits.z * spaceUnits.y ));
        pos.y = startPoint.y + pointSize.y * (( i / spaceUnits.z ) % spaceUnits.y);
        pos.z = startPoint.z + pointSize.z * (i % spaceUnits.z);
        return pos;
    }

    cl_uint3   spaceUnits;
    cl_float3 startPoint;
    cl_float3 pointSize;
    cl_float3 pointHalfSize;

    CubeMatrix<MimageData>* mimageData;
    CubeMatrix<int>*        zoneData;
};


class SpaceBuilder
{
public:
    static SpaceBuilder& Instance(){ static SpaceBuilder builder; return builder; }

    ~SpaceBuilder();

    SpaceData* CreateSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const cl_uint3 &units);
    SpaceData* CreateSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const int &recur);

    SpaceData* GetSpace();

    void DeleteSpace();

private:
    SpaceBuilder();

    SpaceData* _space;
};

#endif // SPACEBUILDER_H
