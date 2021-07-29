#ifndef SPACEBUILDER_H
#define SPACEBUILDER_H

#include <vector>
#include <CL/cl.h>


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
        mimageBuffer(nullptr),
        zoneBuffer(nullptr),
        oldBufferSize(0)
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
    void CreateMimageData(int bufferSize)
    {
        DeleteZoneData();
        if(mimageBuffer)
        {
            if(oldBufferSize == bufferSize)
                return;
            DeleteMimageData();
        }
        mimageBuffer = new MimageData[bufferSize];
        oldBufferSize = bufferSize;
    }
    void CreateZoneData(int bufferSize)
    {
        DeleteMimageData();
        if(zoneBuffer)
        {
            if(oldBufferSize == bufferSize)
                return;
            DeleteZoneData();
        }
        zoneBuffer = new int[bufferSize];
        oldBufferSize = bufferSize;
    }
    void DeleteMimageData()
    {
        if(mimageBuffer)
        {
            delete mimageBuffer;
            mimageBuffer = nullptr;
        }
    }
    void DeleteZoneData()
    {
        if(zoneBuffer)
        {
            delete zoneBuffer;
            zoneBuffer = nullptr;
        }
    }
    int GetSpaceSize()
    {
        return spaceUnits.x*spaceUnits.y*spaceUnits.z;
    }
    int GetBufferSize()
    {
        return oldBufferSize;
    }
    cl_float3 GetPointCoords(int i)
    {
        cl_float3 pos;
        pos.x = startPoint.x + pointSize.x * (i / ( spaceUnits.z * spaceUnits.y ));
        pos.y = startPoint.y + pointSize.y * (( i / spaceUnits.z ) % spaceUnits.y);
        pos.z = startPoint.z + pointSize.z * (i % spaceUnits.z);
        return pos;
    }

    cl_uint3  spaceUnits;
    cl_float3 startPoint;
    cl_float3 pointSize;
    cl_float3 pointHalfSize;

    int         oldBufferSize;
    MimageData* mimageBuffer;
    int*        zoneBuffer;
};


class SpaceManager
{
public:
    enum class BufferType
    { ZoneBuffer, MimageBuffer };
    static SpaceManager& Self();
    ~SpaceManager();

    void InitSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const cl_uint3 &units);
    void InitSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const int &recur);

    void ActivateBuffer(BufferType buffer, int bufferSize);
    void ResetBufferSize(int size = 0);
    int GetBufferSize();


protected:
    void DeleteMimageBuffer();
    void DeleteZoneBuffer();
    void CreateBuffer(BufferType buffer);


private:
    SpaceManager();

    cl_uint3  _spaceUnits;
    cl_float3 _startPoint;
    cl_float3 _pointSize;
    cl_float3 _pointHalfSize;

    int _bufferSize;
    MimageData* _mimageBuffer;
    int*        _zoneBuffer;
};

#endif // SPACEBUILDER_H
