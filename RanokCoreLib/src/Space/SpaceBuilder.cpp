#include "SpaceBuilder.h"
#include <cmath>

using namespace std;

SpaceBuilder::SpaceBuilder()
{

}

SpaceBuilder::~SpaceBuilder()
{
    DeleteSpace();
}


SpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
                                     const std::pair<double, double> &dim2,
                                     const std::pair<double, double> &dim3,
                                     const cl_uint3 &units)
{
    DeleteSpace();

    cl_float3 startPoint;
    startPoint.x = dim1.first;
    startPoint.y = dim2.first;
    startPoint.z = dim3.first;
    cl_float3 pointSize;
    pointSize.x = (dim1.second-dim1.first)/units.x;
    pointSize.y = (dim2.second-dim2.first)/units.y;
    pointSize.z = (dim3.second-dim3.first)/units.z;
    _space = new SpaceData(units, startPoint, pointSize);

    return _space;
}

SpaceData* SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
                                     const std::pair<double, double> &dim2,
                                     const std::pair<double, double> &dim3,
                                     const int &recur)
{
    unsigned depth = pow(2, recur);
    return CreateSpace(dim1, dim2, dim3, {depth, depth, depth});
}

SpaceData *SpaceBuilder::GetSpace()
{
    return _space;
}

//Linear2dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
//                                             const std::pair<double, double> &dim2,
//                                             const Vector2i &step)
//{
//    Delete2dSpace();
//    _data2 = new Linear2dSpaceData(step.x*step.y,
//                                  {
//                                      static_cast<float>((dim1.second-dim1.first)/step.x),
//                                      static_cast<float>((dim2.second-dim2.first)/step.y)
//                                  });
//    double x = dim1.first + _data3->pointSize.x/2.;
//    while(x < dim1.second)
//    {
//        double y = dim2.first + _data3->pointSize.y/2.;
//        while(y < dim2.second)
//        {
//            _data2->points.push_back({x, y});
//            y += _data2->pointSize.y;
//        }
//        x += _data2->pointSize.x;
//    }

//    return _data2;
//}

//Linear2dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
//                                             const std::pair<double, double> &dim2,
//                                             const int &recur)
//{
//    int depth = pow(2, recur);
//    return CreateSpace(dim1, dim2, depth);
//}

//Linear1dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
//                                             const double &step)
//{
//    _data1 = new Linear1dSpaceData(step,static_cast<float>((dim1.second-dim1.first)/step));

//    double x = dim1.first + _data3->pointSize.x/2.;
//    while(x < dim1.second)
//    {
//        _data1->points.push_back(x);
//        x += _data1->pointSize;
//    }

//    return _data1;
//}

//Linear1dSpaceData *SpaceBuilder::CreateSpace(const std::pair<double, double> &dim1,
//                                             const int &recur)
//{
//    int depth = pow(2, recur);
//    return CreateSpace(dim1, depth);
//}


//Linear3dSpaceData *SpaceBuilder::Get3dSpace()
//{
//    return _data3;
//}
//Linear2dSpaceData *SpaceBuilder::Get2dSpace()
//{
//    return _data2;
//}


void SpaceBuilder::DeleteSpace()
{
    if(_space)
    {
        delete _space;
        _space = nullptr;
    }
}
