#include "ModelMetadata.h"


std::ostream& operator<<(std::ostream& stream, ModelMetadata& data)
{
    stream << data.spaceUnit.x;
    stream << data.spaceUnit.y;
    stream << data.spaceUnit.z;
    stream << data.pointSize.x;
    stream << data.pointSize.y;
    stream << data.pointSize.z;
    stream << data.startPoint.x;
    stream << data.startPoint.y;
    stream << data.startPoint.z;

    stream << data.negativeCount;
    stream << data.zeroCount;
    stream << data.positiveCount;
    return stream;
}

std::istream& operator>>(std::istream& stream, ModelMetadata& data)
{
    stream >> data.spaceUnit.x;
    stream >> data.spaceUnit.y;
    stream >> data.spaceUnit.z;
    stream >> data.pointSize.x;
    stream >> data.pointSize.y;
    stream >> data.pointSize.z;
    stream >> data.startPoint.x;
    stream >> data.startPoint.y;
    stream >> data.startPoint.z;

    stream >> data.negativeCount;
    stream >> data.zeroCount;
    stream >> data.positiveCount;

    return stream;
}
