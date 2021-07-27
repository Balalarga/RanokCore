#include "CommonCalculator.h"

CommonCalculator::CommonCalculator(QObject *parent):
    ISpaceCalculator(parent)
{

}

void CommonCalculator::CalcModel(SpaceData* space, int start, int end)
{
    if(!space)
        return;

    auto program = GetProgram();
    cl_float3 halfSize = space->pointHalfSize;
    constexpr unsigned verticesSize = 8;
    QVector<double> values(8);
    cl_double3 vertices[verticesSize];
    cl_float3 point;

    if(end == 0)
        end = space->GetSize();

    for(int i = start; i < end; ++i)
    {
        point = space->GetPos(i);
        vertices[0] = { point.x + halfSize.x, point.y + halfSize.y, point.z + halfSize.z };
        vertices[1] = { point.x + halfSize.x, point.y + halfSize.y, point.z - halfSize.z };
        vertices[2] = { point.x + halfSize.x, point.y - halfSize.y, point.z + halfSize.z };
        vertices[3] = { point.x + halfSize.x, point.y - halfSize.y, point.z - halfSize.z };
        vertices[4] = { point.x - halfSize.x, point.y + halfSize.y, point.z + halfSize.z };
        vertices[5] = { point.x - halfSize.x, point.y + halfSize.y, point.z - halfSize.z };
        vertices[6] = { point.x - halfSize.x, point.y - halfSize.y, point.z + halfSize.z };
        vertices[7] = { point.x - halfSize.x, point.y - halfSize.y, point.z - halfSize.z };

        for(size_t i = 0; i < verticesSize; i++)
            values[i] = program->Compute(vertices[i]);
        space->zoneData->At(i) = GetZone(values);
    }
    emit ComputedModel(start, end);
}

void CommonCalculator::CalcMImage(SpaceData* space, int start, int end)
{
    if(!space)
        return;

    auto program = GetProgram();
    cl_float3 size = space->pointSize;
    QVector<double> wv(4);
    QVector<QVector<double>> a;
    QVector<QVector<double>> b;
    QVector<QVector<double>> c;
    QVector<QVector<double>> d;
    QVector<QVector<double>> f;
    cl_float3 point;

    if(end == 0)
        end = space->GetSize();

    for(int i = start; i < end; ++i)
    {
        point = space->GetPos(i);
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

        space->mimageData->At(i).Cx = detA/div;
        space->mimageData->At(i).Cy = -detB/div;
        space->mimageData->At(i).Cz = -detC/div;
        space->mimageData->At(i).Cw = detD/div;
        space->mimageData->At(i).Ct = detF/div;
    }
    emit ComputedMimage(start, end);
}

double CommonCalculator::DeterminantOfMatrix(QVector<QVector<double>> &mat,
                                              int n)
{
    if (n == 1)
        return mat[0][0];

    double D = 0;
    QVector<QVector<double>> temp(n, QVector<double>(n));
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

void CommonCalculator::GetCofactor(QVector<QVector<double> > &mat,
                                   QVector<QVector<double> > &temp,
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

int CommonCalculator::GetZone(const QVector<double> &values)
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
