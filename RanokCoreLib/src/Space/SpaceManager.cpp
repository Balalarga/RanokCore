#include "SpaceManager.h"
#include <cmath>

using namespace std;


SpaceManager &SpaceManager::Self()
{
    static SpaceManager manager;
    return manager;
}

int SpaceManager::ComputeSpaceSize(const int& recur)
{
    unsigned depth = pow(2, recur);
    return ComputeSpaceSize({depth, depth, depth});
}

int SpaceManager::ComputeSpaceSize(const Vector3u &units)
{
    return units.x * units.y * units.z;
}

SpaceManager::SpaceManager()
{
    _bufferSize = 0;
    _mimageBuffer = 0;
    _zoneBuffer = 0;
    _activeBuffer = BufferType::None;
}

SpaceManager::~SpaceManager()
{
    DeleteZoneBuffer();
    DeleteMimageBuffer();
}

void SpaceManager::InitSpace(const std::pair<double, double> &dim1,
                             const std::pair<double, double> &dim2,
                             const std::pair<double, double> &dim3,
                             const Vector3u &units)
{
    metadata.spaceUnit = units;
    metadata.startPoint.x = dim1.first + _halfPointSize.x;
    metadata.startPoint.y = dim2.first + _halfPointSize.y;
    metadata.startPoint.z = dim3.first + _halfPointSize.z;
    metadata.pointSize.x = (dim1.second-dim1.first)/units.x;
    metadata.pointSize.y = (dim2.second-dim2.first)/units.y;
    metadata.pointSize.z = (dim3.second-dim3.first)/units.z;
    metadata.negativeCount = 0;
    metadata.zeroCount = 0;
    metadata.positiveCount = 0;

    InitFromMetadata();
}

void SpaceManager::InitSpace(const std::pair<double, double> &dim1,
                             const std::pair<double, double> &dim2,
                             const std::pair<double, double> &dim3,
                             const int &recur)
{
    unsigned depth = pow(2, recur);
    InitSpace(dim1, dim2, dim3, {depth, depth, depth});
}

void SpaceManager::InitFromMetadata()
{
    _halfPointSize = {metadata.pointSize.x/2.f,
                      metadata.pointSize.y/2.f,
                      metadata.pointSize.z/2.f};
    _bufferSize = GetSpaceSize();
}

bool SpaceManager::WasInited()
{
    return _zoneBuffer || _mimageBuffer;
}

void SpaceManager::ActivateBuffer(BufferType buffer)
{
    if(_activeBuffer == buffer)
        return;
    CreateBuffer(buffer);
}

/// Setup cpu ram memory usage
void SpaceManager::ResetBufferSize(int size)
{
    if(_bufferSize == size)
        return;

    if(size && size < GetSpaceSize())
        _bufferSize = size;
    else
        _bufferSize = GetSpaceSize();

    if(_zoneBuffer || _activeBuffer == BufferType::None)
        _activeBuffer = BufferType::ZoneBuffer;
    else if(_mimageBuffer)
        _activeBuffer = BufferType::ZoneBuffer;

    CreateBuffer(_activeBuffer);
}

int SpaceManager::GetBufferSize()
{
    return _bufferSize;
}

int SpaceManager::GetSpaceSize()
{
    return metadata.spaceUnit.x * metadata.spaceUnit.y * metadata.spaceUnit.z;
}

void SpaceManager::SetMetadata(const ModelMetadata &metaData)
{
    this->metadata = metaData;
}

const ModelMetadata &SpaceManager::GetMetadata()
{
    return metadata;
}

Vector3u SpaceManager::GetSpaceUnits()
{
    return metadata.spaceUnit;
}

Vector3f SpaceManager::GetPointSize()
{
    return metadata.pointSize;
}

Vector3f SpaceManager::GetHalfPointSize()
{
    return _halfPointSize;
}

void SpaceManager::SaveMimageRange(std::ostream &stream, int count)
{
    if(!_mimageBuffer)
        return;

    if(!count)
        count = _bufferSize;

    stream.write((char*)&_mimageBuffer[0], sizeof(MimageData)*count);
}

void SpaceManager::SaveZoneRange(std::ostream &stream, int count)
{
    if(!_zoneBuffer)
        return;

    if(!count)
        count = _bufferSize;

    for(int i = 0; i < count; i++)
    {
        if(_zoneBuffer[i] < 0)
            ++metadata.negativeCount;
        else if(_zoneBuffer[i] == 0)
            ++metadata.zeroCount;
        else
            ++metadata.positiveCount;
    }

    stream.write((char*)&_zoneBuffer[0], sizeof(_zoneBuffer[0])*count);
}

void SpaceManager::Clear()
{
    DeleteMimageBuffer();
    DeleteZoneBuffer();
}

Vector3f SpaceManager::GetPointCoords(int i)
{
    return
    {
        metadata.startPoint.x + metadata.pointSize.x * (i / ( metadata.spaceUnit.z * metadata.spaceUnit.y)),
        metadata.startPoint.y + metadata.pointSize.y * ((i / metadata.spaceUnit.z) % metadata.spaceUnit.y),
        metadata.startPoint.z + metadata.pointSize.z * (i % metadata.spaceUnit.z)
    };
}

Vector3f SpaceManager::GetStartPoint()
{
    return metadata.startPoint;
}
void SpaceManager::DeleteMimageBuffer()
{
    if(_mimageBuffer)
    {
        delete[] _mimageBuffer;
        _mimageBuffer = 0;
    }
}

void SpaceManager::DeleteZoneBuffer()
{
    if(_zoneBuffer)
    {
        delete[] _zoneBuffer;
        _zoneBuffer = 0;
    }
}

void SpaceManager::CreateBuffer(BufferType buffer)
{
    DeleteMimageBuffer();
    DeleteZoneBuffer();
    switch (buffer)
    {
    case BufferType::ZoneBuffer:
        _zoneBuffer = new char[_bufferSize];
        break;
    case BufferType::MimageBuffer:
        _mimageBuffer = new MimageData[_bufferSize];
        break;
    default:
        break;
    }
    _activeBuffer = buffer;
}

