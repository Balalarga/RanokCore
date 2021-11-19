#ifndef SPACEBUILDER_H
#define SPACEBUILDER_H

#include <stdexcept>
#include <iostream>
#include <vector>
#include <CL/cl.h>
#include <assert.h>


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
    struct CommonMetadata
    {
        // Start point
        float startPointX = 0,
              startPointY = 0,
              startPointZ = 0;

        // Voxel size
        float pointSizeX = 0,
              pointSizeY = 0,
              pointSizeZ = 0;

        // Space size
        unsigned spaceUnitsX = 0,
                 spaceUnitsY = 0,
                 spaceUnitsZ = 0;
        friend std::ostream& operator<<(std::ostream& stream, SpaceManager::CommonMetadata& data);
        friend std::istream& operator>>(std::istream& stream, SpaceManager::CommonMetadata& data);
    };

    struct ModelMetadata
    {
        CommonMetadata commonData;

        // Zones data
        unsigned positiveCount = 0,
                 zeroCount = 0,
                 negativeCount = 0;
        friend std::ostream& operator<<(std::ostream& stream, SpaceManager::ModelMetadata& data);
        friend std::istream& operator>>(std::istream& stream, SpaceManager::ModelMetadata& data);
    } metadata;

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
    void InitFromMetadata();
    bool WasInited();

    void ActivateBuffer(BufferType buffer);

    /// Setup cpu ram memory usage
    void ResetBufferSize(int size = 0);
    int GetBufferSize();
    int GetSpaceSize();

    inline void AddMimageData(const int& id, const MimageData& data)
    {
        assert(_activeBuffer == BufferType::MimageBuffer && id < _bufferSize);
        _mimageBuffer[id] = data;
    }
    inline void AddZoneData(const int& id, const int& data)
    {
        assert(_activeBuffer == BufferType::ZoneBuffer && id < _bufferSize);
        _zoneBuffer[id] = data;
    }
    inline char* GetZoneBuffer(){ return _zoneBuffer; }
    inline MimageData* GetMimageBuffer(){ return _mimageBuffer; }
    inline const char& GetZone(int id)
    {
        assert(id < _bufferSize);
        return _zoneBuffer[id];
    }
    inline const MimageData& GetMimage(int id)
    {
        assert(id < _bufferSize);
        return _mimageBuffer[id];
    }


    cl_uint3 GetSpaceUnits();
    cl_float3 GetPointCoords(int i);
    cl_float3 GetStartPoint();
    cl_float3 GetPointSize();
    cl_float3 GetHalfPointSize();


    void SaveMimageRange(std::ostream& stream, int end = 0);
    void SaveZoneRange(std::ostream &stream, int end = 0);


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
    char*        _zoneBuffer;

    BufferType _activeBuffer;
};


#endif // SPACEBUILDER_H
