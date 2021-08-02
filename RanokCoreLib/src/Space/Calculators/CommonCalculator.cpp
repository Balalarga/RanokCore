#include "CommonCalculator.h"

CommonCalculator::CommonCalculator(std::function<void (CalculatorMode, int, int, int)> func):
    ISpaceCalculator(func)
{

}


void CommonCalculator::CalcModel(int spaceOffset, int start, int end)
{
    SpaceManager& space = SpaceManager::Self();

    auto program = GetProgram();
    cl_float3 halfSize = space.GetHalfPointSize();
    constexpr unsigned verticesSize = 8;
    std::vector<double> values(8);
    cl_double3 vertices[verticesSize];
    cl_float3 point;

    int count = end-start;
    for(int i = 0; i < count; ++i)
    {
        point = space.GetPointCoords(spaceOffset+start+i);
        vertices[0] = { point.x + halfSize.x, point.y + halfSize.y, point.z + halfSize.z };
        vertices[1] = { point.x + halfSize.x, point.y + halfSize.y, point.z - halfSize.z };
        vertices[2] = { point.x + halfSize.x, point.y - halfSize.y, point.z + halfSize.z };
        vertices[3] = { point.x + halfSize.x, point.y - halfSize.y, point.z - halfSize.z };
        vertices[4] = { point.x - halfSize.x, point.y + halfSize.y, point.z + halfSize.z };
        vertices[5] = { point.x - halfSize.x, point.y + halfSize.y, point.z - halfSize.z };
        vertices[6] = { point.x - halfSize.x, point.y - halfSize.y, point.z + halfSize.z };
        vertices[7] = { point.x - halfSize.x, point.y - halfSize.y, point.z - halfSize.z };

        for(size_t vi = 0; vi < verticesSize; vi++)
            values[vi] = program->Compute(vertices[vi]);
        space.AddZoneData(start+i, GetZone(values));
    }
}

void CommonCalculator::CalcMImage(int spaceOffset, int start, int end)
{
    SpaceManager& space = SpaceManager::Self();

    auto program = GetProgram();
    cl_float3 size = space.GetPointSize();
    std::vector<double> wv(4);
    std::vector<std::vector<double>> a;
    std::vector<std::vector<double>> b;
    std::vector<std::vector<double>> c;
    std::vector<std::vector<double>> d;
    std::vector<std::vector<double>> f;
    cl_float3 point;

    int count = end-start;
    for(int i = 0; i < count; ++i)
    {
        point = space.GetPointCoords(spaceOffset+start+i);
        wv[0] = program->Compute({point.x,        point.y,        point.z       });
        wv[1] = program->Compute({point.x+size.x, point.y,        point.z       });
        wv[2] = program->Compute({point.x,        point.y+size.y, point.z       });
        wv[3] = program->Compute({point.x,        point.y,        point.z+size.z});

        a = {
            {point.y,        point.z,        wv[0], 1},
            {point.y,        point.z,        wv[1], 1},
            {point.y+size.y, point.z,        wv[2], 1},
            {point.y,        point.z+size.z, wv[3], 1},
        };
        b = {
            {point.x,        point.z,        wv[0], 1},
            {point.x+size.x, point.z,        wv[1], 1},
            {point.x,        point.z,        wv[2], 1},
            {point.x,        point.z+size.z, wv[3], 1},
        };
        c = {
            {point.x,        point.y,        wv[0], 1},
            {point.x+size.x, point.y,        wv[1], 1},
            {point.x,        point.y+size.y, wv[2], 1},
            {point.x,        point.y,        wv[3], 1},
        };
        d = {
            {point.x,        point.y,        point.z,        1},
            {point.x+size.x, point.y,        point.z,        1},
            {point.x,        point.y+size.y, point.z,        1},
            {point.x,        point.y,        point.z+size.z, 1},
        };
        f = {
            {point.x,        point.y,        point.z,        wv[0]},
            {point.x+size.x, point.y,        point.z,        wv[1]},
            {point.x,        point.y+size.y, point.z,        wv[2]},
            {point.x,        point.y,        point.z+size.z, wv[3]},
        };

        double detA = DeterminantOfMatrix(a, 4);
        double detB = DeterminantOfMatrix(b, 4);
        double detC = DeterminantOfMatrix(c, 4);
        double detD = DeterminantOfMatrix(d, 4);
        double detF = DeterminantOfMatrix(f, 4);
        double div = sqrt(pow(detA, 2)+pow(detB, 2)+
                          pow(detC, 2)+pow(detD, 2)+pow(detF, 2));
        space.AddMimageData(i, {detA/div,
                                -detB/div,
                                -detC/div,
                                detD/div,
                                detF/div});
    }
}

double CommonCalculator::DeterminantOfMatrix(std::vector<std::vector<double>> &mat,
                                              int n)
{
    if (n == 1)
        return mat[0][0];

    double D = 0;
    std::vector<std::vector<double>> temp(n, std::vector<double>(n));
    int sign = 1;
    for (int f = 0; f < n; f++)
    {
        GetCofactor(mat, temp, 0, f, n);
        D += sign * mat[0][f]
                * DeterminantOfMatrix(temp, n - 1);
        sign = -sign;
    }
    return D;
}

void CommonCalculator::GetCofactor(std::vector<std::vector<double> > &mat,
                                   std::vector<std::vector<double> > &temp,
                                   int p, int q, int n)
{
    int i = 0, j = 0;
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp[i][j++] = mat[row][col];
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

int CommonCalculator::GetZone(const std::vector<double> &values)
{
    bool plus = false;
    bool zero = false;
    bool minus = false;

    for(size_t i = 0; i < values.size(); i++)
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
}
