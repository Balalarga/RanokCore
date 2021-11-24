#ifndef SPACEBUILDER_H
#define SPACEBUILDER_H

#include <iostream>
#include <vector>
#include <assert.h>


#include "ModelMetadata.h"


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

    ~SpaceManager();
    static SpaceManager& Self();

    static int ComputeSpaceSize(const int &recur);
    static int ComputeSpaceSize(const Vector3u &units);

    inline void AddMimageData(const int& id, const MimageData& data)
    {
        if(_activeBuffer == BufferType::MimageBuffer)
            _mimageBuffer[id] = data;
    }
    inline void AddZoneData(const int& id, const int& data)
    {
        if(_activeBuffer == BufferType::ZoneBuffer)
            _zoneBuffer[id] = data;
    }
    inline char* GetZoneBuffer(){ return _zoneBuffer; }
    inline MimageData* GetMimageBuffer() { return _mimageBuffer; }
    inline const char& GetZone(int id) { return _zoneBuffer[id]; }
    inline const MimageData& GetMimage(int id) { return _mimageBuffer[id]; }


    void InitSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const Vector3u &units);
    void InitSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const int &recur);

    void InitFromMetadata();
    void SetMetadata(const ModelMetadata& metaData);
    const ModelMetadata& GetMetadata();

    bool WasInited();

    void ActivateBuffer(BufferType buffer);

    /// Setup cpu ram memory usage
    void ResetBufferSize(int size = 0);
    int GetBufferSize();
    int GetSpaceSize();

    Vector3u GetSpaceUnits();
    Vector3f GetPointCoords(int i);
    Vector3f GetStartPoint();
    Vector3f GetPointSize();
    Vector3f GetHalfPointSize();

    void SaveMimageRange(std::ostream& stream, int end = 0);
    void SaveZoneRange(std::ostream &stream, int end = 0);

    void Clear();

protected:
    void DeleteMimageBuffer();
    void DeleteZoneBuffer();
    void CreateBuffer(BufferType buffer);


private:
    SpaceManager();

    ModelMetadata metadata;
    Vector3f _halfPointSize;

    int _bufferSize;
    MimageData* _mimageBuffer;
    char*        _zoneBuffer;

    BufferType _activeBuffer;
};


#endif // SPACEBUILDER_H
