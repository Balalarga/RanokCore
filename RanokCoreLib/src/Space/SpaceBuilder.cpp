#include "SpaceBuilder.h"
#include <cmath>

using namespace std;


SpaceManager &SpaceManager::Self()
{
    static SpaceManager builder;
    return builder;
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
}

void SpaceManager::InitSpace(const std::pair<double, double> &dim1,
                             const std::pair<double, double> &dim2,
                             const std::pair<double, double> &dim3,
                             const int &recur)
{
    unsigned depth = pow(2, recur);
    InitSpace(dim1, dim2, dim3, {depth, depth, depth});
}

void SpaceManager::ActivateBuffer(BufferType buffer, int bufferSize = 0)
{
    if(bufferSize && bufferSize != _bufferSize)
    {
        _bufferSize = bufferSize;
        if(_zoneBuffer)
            CreateBuffer(BufferType::ZoneBuffer);
        else if(_mimageBuffer)
            CreateBuffer(BufferType::MimageBuffer);
    }
    else
    {

    }
}

void SpaceManager::ResetBufferSize(int size)
{
    if(_bufferSize != size)
    {
        _bufferSize = size;
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

void SpaceManager::DeleteMimageBuffer()
{
    if(_mimageBuffer)
    {
        delete _mimageBuffer;
        _mimageBuffer = 0;
    }
}

void SpaceManager::DeleteZoneBuffer()
{
    if(_zoneBuffer)
    {
        delete _zoneBuffer;
        _zoneBuffer = 0;
    }
}

void SpaceManager::CreateBuffer(BufferType buffer)
{
    switch (buffer)
    {
    case BufferType::ZoneBuffer:
    {
        DeleteMimageBuffer();
    }
    case BufferType::MimageBuffer:
    {
        DeleteZoneBuffer();
    }
    }
}
