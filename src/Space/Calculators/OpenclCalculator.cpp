#include "OpenclCalculator.h"
#include <QDebug>
#include <sstream>
#include <CL/cl.h>
using namespace std;

OpenclCalculator::OpenclCalculator(QObject *parent):
    ISpaceCalculator(parent)
{
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if(ret != CL_SUCCESS)
        qDebug()<<Qt::endl<<ret<<Qt::endl;
    qDebug()<<"platform_id: "<<platform_id;
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1,
                         &device_id, &ret_num_devices);
    if(ret != CL_SUCCESS)
        qDebug()<<Qt::endl<<ret;
    qDebug()<<"device_id: "<<device_id;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    qDebug()<<"context: "<<context;
    command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);
}

OpenclCalculator::~OpenclCalculator()
{
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    if(program)
        ret = clReleaseProgram(program);
    if(kernel)
        ret = clReleaseKernel(kernel);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
}

QString OpenclCalculator::CreateOpenclSource(const Program& program)
{
    stringstream result;
    result << R"(
double __matrix4x4Det(double* m, int n)
{
      return
         m[3] * m[1*n+2] * m[2*n+1] * m[3*n+0] - m[0*n+2] * m[1*n+3] * m[2*n+1] * m[3*n+0] -
         m[3] * m[1*n+1] * m[2*n+2] * m[3*n+0] + m[0*n+1] * m[1*n+3] * m[2*n+2] * m[3*n+0] +
         m[2] * m[1*n+1] * m[2*n+3] * m[3*n+0] - m[0*n+1] * m[1*n+2] * m[2*n+3] * m[3*n+0] -
         m[3] * m[1*n+2] * m[2*n+0] * m[3*n+1] + m[0*n+2] * m[1*n+3] * m[2*n+0] * m[3*n+1] +
         m[3] * m[1*n+0] * m[2*n+2] * m[3*n+1] - m[0*n+0] * m[1*n+3] * m[2*n+2] * m[3*n+1] -
         m[2] * m[1*n+0] * m[2*n+3] * m[3*n+1] + m[0*n+0] * m[1*n+2] * m[2*n+3] * m[3*n+1] +
         m[3] * m[1*n+1] * m[2*n+0] * m[3*n+2] - m[0*n+1] * m[1*n+3] * m[2*n+0] * m[3*n+2] -
         m[3] * m[1*n+0] * m[2*n+1] * m[3*n+2] + m[0*n+0] * m[1*n+3] * m[2*n+1] * m[3*n+2] +
         m[1] * m[1*n+0] * m[2*n+3] * m[3*n+2] - m[0*n+0] * m[1*n+1] * m[2*n+3] * m[3*n+2] -
         m[2] * m[1*n+1] * m[2*n+0] * m[3*n+3] + m[0*n+1] * m[1*n+2] * m[2*n+0] * m[3*n+3] +
         m[2] * m[1*n+0] * m[2*n+1] * m[3*n+3] - m[0*n+0] * m[1*n+2] * m[2*n+1] * m[3*n+3] -
         m[1] * m[1*n+0] * m[2*n+2] * m[3*n+3] + m[0*n+0] * m[1*n+1] * m[2*n+2] * m[3*n+3];
}


double __rand(double a, double b)
{
    return a + b - sqrt(pow(a, 2) + pow(b, 2));
}
double __ror(double a, double b)
{
    return a + b + sqrt(pow(a, 2) + pow(b, 2));
}

int checkZone(double *values)
{
    bool plus = false;
    bool zero = false;
    bool minus = false;
    for(int i = 0; i < 8; i++)
    {
        if(values[i] == 0)
            zero = true;
        else if(values[i] < 0)
            minus = true;
        else if(values[i] > 0)
            plus = true;
    }
    if(zero || (plus && minus))
        return 0;
    if(plus)
        return 1;
    return -1;
})";
    result << program.GetOpenclCode();

    result << R"(
kernel void __calcualteModel(global int *resultZones,
                           const int startId,
                           const uint3 spaceSize,
                           const float3 startPoint,
                           const float3 pointSize,
                           const float3 halfSize)
{
    int id = get_global_id(0);
    int spaceId = startId + id;
    float3 point;
    point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
    point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
    point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);


    double values[8];
    values[0] = __resultFunc(point.x+halfSize.x, point.y+halfSize.y, point.z+halfSize.z);
    values[1] = __resultFunc(point.x+halfSize.x, point.y+halfSize.y, point.z-halfSize.z);
    values[2] = __resultFunc(point.x+halfSize.x, point.y-halfSize.y, point.z+halfSize.z);
    values[3] = __resultFunc(point.x+halfSize.x, point.y-halfSize.y, point.z-halfSize.z);
    values[4] = __resultFunc(point.x-halfSize.x, point.y+halfSize.y, point.z+halfSize.z);
    values[5] = __resultFunc(point.x-halfSize.x, point.y+halfSize.y, point.z-halfSize.z);
    values[6] = __resultFunc(point.x-halfSize.x, point.y-halfSize.y, point.z+halfSize.z);
    values[7] = __resultFunc(point.x-halfSize.x, point.y-halfSize.y, point.z-halfSize.z);

    resultZones[id] = checkZone(values);
}

kernel void __calculateMImage(global double *result,
                           const int startId,
                           const uint3 spaceSize,
                           const float3 startPoint,
                           const float3 pointSize,
                           const float3 halfSize)
{
    int id = get_global_id(0);
    int spaceId = startId + id;
    float3 point;
    point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
    point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
    point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);

    double wv[4];
    wv[0] = __resultFunc(point.x,             point.y,             point.z            );
    wv[1] = __resultFunc(point.x+pointSize.x, point.y,             point.z            );
    wv[2] = __resultFunc(point.x,             point.y+pointSize.y, point.z            );
    wv[3] = __resultFunc(point.x,             point.y,             point.z+pointSize.z);

    double a[] = {
        point.y,             point.z,             wv[0], 1,
        point.y,             point.z,             wv[1], 1,
        point.y+pointSize.y, point.z,             wv[2], 1,
        point.y,             point.z+pointSize.z, wv[3], 1
    };
    double b[] = {
        point.x,             point.z,             wv[0], 1,
        point.x+pointSize.x, point.z,             wv[1], 1,
        point.x,             point.z,             wv[2], 1,
        point.x,             point.z+pointSize.z, wv[3], 1
    };
    double c[] = {
        point.x,             point.y,             wv[0], 1,
        point.x+pointSize.x, point.y,             wv[1], 1,
        point.x,             point.y+pointSize.y, wv[2], 1,
        point.x,             point.y,             wv[3], 1
    };
    double d[] = {
        point.x,             point.y,             point.z,              1,
        point.x+pointSize.x, point.y,             point.z,              1,
        point.x,             point.y+pointSize.y, point.z,              1,
        point.x,             point.y,             point.z+pointSize.z, 1
    };
    double f[] = {
        point.x,             point.y,             point.z,             wv[0],
        point.x+pointSize.x, point.y,             point.z,             wv[1],
        point.x,             point.y+pointSize.y, point.z,             wv[2],
        point.x,             point.y,             point.z+pointSize.z, wv[3]
    };
    double detA = __matrix4x4Det(a, 4);
    double detB = __matrix4x4Det(b, 4);
    double detC = __matrix4x4Det(c, 4);
    double detD = __matrix4x4Det(d, 4);
    double detF = __matrix4x4Det(f, 4);
    double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

    result[id*5  ] = detA/div;
    result[id*5+1] = -detB/div;
    result[id*5+2] = -detC/div;
    result[id*5+3] = detD/div;
    result[id*5+4] = detF/div;
}

)";
    return QString::fromStdString(result.str());
}

int OpenclCalculator::GetLocalGroupSize()
{
    return localGroupSize;
}


void OpenclCalculator::CalcModel(SpaceData* space, int start, int end)
{
    if(!space)
        return;

    auto prog = GetProgram();
    QString source = CreateOpenclSource(*prog);
    QByteArray ba = source.toLocal8Bit();
    const char* src_str = ba.data();

    if(source != prevSource)
    {
        if(!prevSource.isEmpty())
        {
            ret = clReleaseProgram(program);
            ret = clReleaseKernel(kernel);
        }

        program = clCreateProgramWithSource(context, 1,
                                            (const char **)&src_str, NULL, &ret);
        if (!program)
        {
            qDebug()<<"Error: Failed to create compute program!";
            return;
        }
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        if (ret != CL_SUCCESS)
        {
            size_t len;
            char buffer[2048];

            qDebug()<<"Error: Failed to build program executable!";
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
                                  sizeof(buffer), buffer, &len);
            qDebug()<<buffer;
            return;
        }
        prevSource = source;
    }

    kernel = clCreateKernel(program, "__calcualteModel", &ret);
    if (!kernel || ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to create compute kernel!";
        return;
    }

    if(end == 0)
        end = space->GetSize();

    // Create gpu buffers
    cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                        (end-start) * sizeof(cl_int), NULL, &ret);
    if (!out_mem_obj)
    {
        qDebug()<<"Error: Failed to allocate device memory!";
        return;
    }

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_int), &start);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_uint3), &space->spaceUnits);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_float3), &space->startPoint);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_float3), &space->pointSize);
    ret = clSetKernelArg(kernel, 5, sizeof(cl_float3), &space->pointHalfSize);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to set kernel arguments! "<<ret;
        clReleaseMemObject(out_mem_obj);
        return;
    }
    // Get the maximum work group size for executing the kernel on the device
    //
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    ret = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE,
                                   sizeof(local), &local, NULL);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to retrieve kernel work group info! " << ret;
        clReleaseMemObject(out_mem_obj);
        return;
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    global = (end-start);
    localGroupSize = local;

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                                 &global, &local, 0, NULL, NULL);
    if (ret)
    {
        qDebug()<<"Error: Failed to execute kernel!\n";
        clReleaseMemObject(out_mem_obj);
        return;
    }
    clFinish(command_queue);

    ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0,
                              (end-start) * sizeof(int), &space->zoneData->At(start), 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to read output array! "<<ret;
        clReleaseMemObject(out_mem_obj);
        return;
    }
    ret = clReleaseMemObject(out_mem_obj);
    emit ComputedModel(start, end);
}

void OpenclCalculator::CalcMImage(SpaceData* space, int start, int end)
{
    if(!space)
        return;

    auto prog = GetProgram();
    QString source = CreateOpenclSource(*prog);
    QByteArray ba = source.toLocal8Bit();
    const char* src_str = ba.data();

    if(source != prevSource)
    {
        if(!prevSource.isEmpty())
        {
            ret = clReleaseProgram(program);
            ret = clReleaseKernel(kernel);
        }

        program = clCreateProgramWithSource(context, 1,
                                            (const char **)&src_str, NULL, &ret);
        if (!program)
        {
            qDebug()<<"Error: Failed to create compute program!";
            return;
        }
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        if (ret != CL_SUCCESS)
        {
            size_t len;
            char buffer[2048];

            qDebug()<<"Error: Failed to build program executable!";
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
                                  sizeof(buffer), buffer, &len);
            qDebug()<<buffer;
            return;
        }
        prevSource = source;
    }

    kernel = clCreateKernel(program, "__calculateMImage", &ret);
    if (!kernel || ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to create compute kernel!";
        return;
    }

    if(end == 0)
        end = space->GetSize();

    // Create gpu buffers
    cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                        (end-start) * 5 * sizeof(cl_double), NULL, &ret);
    if (!out_mem_obj)
    {
        qDebug()<<"Error: Failed to allocate device memory!";
        return;
    }

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_int), &start);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_uint3), &space->spaceUnits);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_float3), &space->startPoint);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_float3), &space->pointSize);
    ret = clSetKernelArg(kernel, 5, sizeof(cl_float3), &space->pointHalfSize);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to set kernel arguments! "<<ret;
        ret = clReleaseMemObject(out_mem_obj);
        return;
    }
    // Get the maximum work group size for executing the kernel on the device
    //
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    ret = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE,
                                   sizeof(local), &local, NULL);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to retrieve kernel work group info! " << ret;
        clReleaseMemObject(out_mem_obj);
        return;
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    localGroupSize = local;
    global = (end-start);

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                                 &global, &local, 0, NULL, NULL);
    if (ret)
    {
        qDebug()<<"Error: Failed to execute kernel!\n";
        clReleaseMemObject(out_mem_obj);
        return;
    }
    clFinish(command_queue);

    ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0,
                              (end-start) * 5 * sizeof(cl_double), &space->mimageData->At(start),
                              0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to read output array! "<<ret;
        clReleaseMemObject(out_mem_obj);
        return;
    }
    ret = clReleaseMemObject(out_mem_obj);
    emit ComputedMimage(start, end);
}
