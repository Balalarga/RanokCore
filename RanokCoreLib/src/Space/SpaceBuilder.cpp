#include "SpaceBuilder.h"
#include <cmath>

using namespace std;


SpaceManager* SpaceManager::_selfInstance = nullptr;
SpaceManager &SpaceManager::Self()
{
    if(!_selfInstance)
        _selfInstance = new SpaceManager;
    return *_selfInstance;
}

void SpaceManager::Destroy()
{
    if(_selfInstance)
        delete _selfInstance;
}

SpaceManager::SpaceManager()
{
    _bufferSize = 0;
    _mimageBuffer = 0;
    _zoneBuffer = 0;
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
}

void SpaceManager::InitSpace(const std::pair<double, double> &dim1,
                             const std::pair<double, double> &dim2,
                             const std::pair<double, double> &dim3,
                             const int &recur)
{
    unsigned depth = pow(2, recur);
    InitSpace(dim1, dim2, dim3, {depth, depth, depth});
}

void SpaceManager::ActivateBuffer(BufferType buffer)
{
    if((_zoneBuffer && buffer == BufferType::ZoneBuffer) ||
       (_mimageBuffer && buffer == BufferType::MimageBuffer))
        return;
    CreateBuffer(buffer);
}

/// Setup cpu ram memory usage
void SpaceManager::ResetBufferSize(int size)
{
    if(_bufferSize != size)
    {
        if(size)
            _bufferSize = size;
        else
            _bufferSize = GetSpaceSize();

        if(_zoneBuffer)
            CreateBuffer(BufferType::ZoneBuffer);
        else if(_mimageBuffer)
            CreateBuffer(BufferType::MimageBuffer);
    }
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
        _zoneBuffer = new int[_bufferSize];
        break;
    case BufferType::MimageBuffer:
        _mimageBuffer = new MimageData[_bufferSize];
        break;
    }
}
