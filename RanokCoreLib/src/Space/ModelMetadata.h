#ifndef MODELMETADATA_H
#define MODELMETADATA_H

#include <iostream>
#include "Vectors.h"

class ModelMetadata
{
public:
    ModelMetadata() = default;

    // Start point
    Vector3f startPoint;

    // Voxel size
    Vector3f pointSize;

    // Space size
    Vector3u spaceUnit;

    // Zones data
    unsigned positiveCount = 0,
             zeroCount = 0,
             negativeCount = 0;

    friend std::ostream& operator<<(std::ostream& stream, ModelMetadata& data);
    friend std::istream& operator>>(std::istream& stream, ModelMetadata& data);
};

#endif // MODELMETADATA_H
