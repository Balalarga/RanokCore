#ifndef SPACEBUILDER_H
#define SPACEBUILDER_H

#include <stdexcept>
#include <iostream>
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


class SpaceManager
{
public:
    enum class BufferType { None, ZoneBuffer, MimageBuffer };

    static SpaceManager& Self();

    static int ComputeSpaceSize(const int &recur);
    static int ComputeSpaceSize(const cl_uint3 &units);

    ~SpaceManager();


    void InitSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const cl_uint3 &units);
    void InitSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const int &recur);
    bool WasInited();

    void ActivateBuffer(BufferType buffer);

    /// Setup cpu ram memory usage
    void ResetBufferSize(int size = 0);
    int GetBufferSize();
    int GetSpaceSize();

    inline void AddMimageData(const int& id, const MimageData& data)
    {
        if(_activeBuffer != BufferType::MimageBuffer || id >= _bufferSize )
            throw std::runtime_error("AddMimageData: out of buffer range");
        else
            _mimageBuffer[id] = data;
    }
    inline void AddZoneData(const int& id, const int& data)
    {
        if(_activeBuffer != BufferType::ZoneBuffer || id >= _bufferSize )
            throw std::runtime_error("AddZoneData: out of buffer range");
        else
            _zoneBuffer[id] = data;
    }
    inline int* GetZoneBuffer(){ return _zoneBuffer; }
    inline MimageData* GetMimageBuffer(){ return _mimageBuffer; }
    inline const int& GetZone(int id)
    {
        if(id >= _bufferSize )
            throw std::runtime_error("GetZone: out of buffer range");
        else
            return _zoneBuffer[id];
    }
    inline const MimageData& GetMimage(int id)
    {
        if(id >= _bufferSize )
            throw std::runtime_error("GetMimage: out of buffer range");
        else
            return _mimageBuffer[id];
    }


    cl_uint3 GetSpaceUnits();
    cl_float3 GetPointCoords(int i);
    cl_float3 GetStartPoint();
    cl_float3 GetPointSize();
    cl_float3 GetHalfPointSize();


    void SaveMimageRange(std::ostream& stream, int start, int end);
    void SaveZoneRange(std::ostream &stream, int start, int end);


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

    BufferType _activeBuffer;
};

#endif // SPACEBUILDER_H
