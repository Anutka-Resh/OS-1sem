#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

const int size = 1<<11;

int RowsA[size][size];
int RowsB[size][size];
int RowsRes[size][size];

int main()
{
    int startp = 9;
    srand(startp);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            RowsA[i][j] = rand();
            RowsB[i][j] = rand();
        }
    }

    clock_t start, finish;
    start = clock();

    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            for (int i = 0; i < size; ++i)
            {
                RowsRes[x][y] += RowsA[x][i] * RowsB[i][y];
            }
        }
    }
    finish = clock();
    double fintime = (double)(finish - start) / CLOCKS_PER_SEC;

    FILE *stream;
    stream=fopen("outR.txt", "w");

    fprintf(stream, "%f\n", fintime);
    return 0;
}
