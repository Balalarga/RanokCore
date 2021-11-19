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

int SpaceManager::ComputeSpaceSize(const cl_uint3 &units)
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
                             const cl_uint3 &units)
{
    _spaceUnits = units;

    _pointSize.x = (dim1.second-dim1.first)/units.x;
    _pointSize.y = (dim2.second-dim2.first)/units.y;
    _pointSize.z = (dim3.second-dim3.first)/units.z;

    _pointHalfSize = {_pointSize.x/2.f,
                      _pointSize.y/2.f,
                      _pointSize.z/2.f};
    _startPoint.x = dim1.first + _pointHalfSize.x;
    _startPoint.y = dim2.first + _pointHalfSize.y;
    _startPoint.z = dim3.first + _pointHalfSize.z;

    _bufferSize = GetSpaceSize();

    metadata.commonData.spaceUnitsX = _spaceUnits.x;
    metadata.commonData.spaceUnitsY = _spaceUnits.y;
    metadata.commonData.spaceUnitsZ = _spaceUnits.z;
    metadata.commonData.startPointX = _startPoint.x;
    metadata.commonData.startPointY = _startPoint.y;
    metadata.commonData.startPointZ = _startPoint.z;
    metadata.commonData.pointSizeX = _pointSize.x;
    metadata.commonData.pointSizeY = _pointSize.y;
    metadata.commonData.pointSizeZ = _pointSize.z;
    metadata.negativeCount = 0;
    metadata.zeroCount = 0;
    metadata.positiveCount = 0;
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
    _spaceUnits = {metadata.commonData.spaceUnitsX,
                  metadata.commonData.spaceUnitsY,
                  metadata.commonData.spaceUnitsZ};

    _pointSize = {metadata.commonData.pointSizeX,
                  metadata.commonData.pointSizeY,
                  metadata.commonData.pointSizeZ};
    _pointHalfSize = {_pointSize.x/2.f,
                      _pointSize.y/2.f,
                      _pointSize.z/2.f};
    _startPoint = {metadata.commonData.startPointX,
                  metadata.commonData.startPointY,
                  metadata.commonData.startPointZ};

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
    return _spaceUnits.x * _spaceUnits.y * _spaceUnits.z;
}

cl_uint3 SpaceManager::GetSpaceUnits()
{
    return _spaceUnits;
}

cl_float3 SpaceManager::GetPointSize()
{
    return _pointSize;
}

cl_float3 SpaceManager::GetHalfPointSize()
{
    return _pointHalfSize;
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

cl_float3 SpaceManager::GetPointCoords(int i)
{
    return
    {
        _startPoint.x + _pointSize.x * (i / ( _spaceUnits.z * _spaceUnits.y)),
        _startPoint.y + _pointSize.y * ((i / _spaceUnits.z) % _spaceUnits.y),
        _startPoint.z + _pointSize.z * (i % _spaceUnits.z)
    };
}

cl_float3 SpaceManager::GetStartPoint()
{
    return _startPoint;
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

std::ostream& operator<<(std::ostream& stream, SpaceManager::CommonMetadata& data)
{
    stream << data.pointSizeX;
    stream << data.pointSizeY;
    stream << data.pointSizeZ;

    stream << data.spaceUnitsX;
    stream << data.spaceUnitsY;
    stream << data.spaceUnitsZ;

    stream << data.startPointX;
    stream << data.startPointY;
    stream << data.startPointZ;

    return stream;
}

std::ostream& operator<<(std::ostream& stream, SpaceManager::ModelMetadata& data)
{
    stream << data.commonData;
    stream << data.negativeCount;
    stream << data.zeroCount;
    stream << data.negativeCount;
    return stream;
}

std::istream& operator>>(std::istream& stream, SpaceManager::CommonMetadata& data)
{
    stream >> data.pointSizeX;
    stream >> data.pointSizeY;
    stream >> data.pointSizeZ;
    stream >> data.spaceUnitsX;
    stream >> data.spaceUnitsY;
    stream >> data.spaceUnitsZ;
    stream >> data.startPointX;
    stream >> data.startPointY;
    stream >> data.startPointZ;

    return stream;
}

std::istream& operator>>(std::istream& stream, SpaceManager::ModelMetadata& data)
{
    stream >> data.commonData;
    stream >> data.negativeCount;
    stream >> data.zeroCount;
    stream >> data.negativeCount;
    return stream;
}
