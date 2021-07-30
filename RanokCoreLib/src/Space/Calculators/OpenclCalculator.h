#ifndef OPENCLCALCULATOR_H
#define OPENCLCALCULATOR_H

#include "ISpaceCalculator.h"

class OpenclCalculator : public ISpaceCalculator
{
public:
    OpenclCalculator(std::function<void (CalculatorMode, int, int, int)> func);
    ~OpenclCalculator();

    void CalcModel(int start = 0, int end = 0) override;
    void CalcMImage(int start = 0, int end = 0) override;

    std::string CreateOpenclSource(const Program& program);
    int GetLocalGroupSize();

private:
    cl_platform_id   platform_id = 0;
    cl_device_id     device_id = 0;
    cl_context       context;
    cl_command_queue command_queue;
    cl_uint          ret_num_devices;
    cl_uint          ret_num_platforms;
    cl_int           ret;// сюда будут записываться сообщения об ошибках
    cl_program       program = 0;
    cl_kernel        kernel = 0;
    std::string      prevSource;

    int localGroupSize = 0;
};

#endif // OPENCLCALCULATOR_H
