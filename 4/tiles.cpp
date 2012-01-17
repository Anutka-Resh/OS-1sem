#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
using namespace std;

const int r = 8;
const int size = 1 << r;

const int TileSize = r / 2;
const int ElInTile = 1 << TileSize;
const int MOD = ElInTile - 1;
const int NumOfTiles = size / ElInTile;

int TilesA[NumOfTiles][NumOfTiles][ElInTile][ElInTile];
int TilesB[NumOfTiles][NumOfTiles][ElInTile][ElInTile];
int TilesRes[NumOfTiles][NumOfTiles][ElInTile][ElInTile];

int main()
{
    srand(9);

    for (int i = 0; i < NumOfTiles; ++i)
    {
        for (int j = 0; j < NumOfTiles; ++j)
        {
            for (int x = 0; x < (ElInTile); ++x)
            {
                for (int y = 0; y < (ElInTile); ++y)
                {
                    TilesA[i][j][x][y] = rand();
                    TilesB[i][j][x][y] = rand();
                }
            }
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
                TilesRes[x >> TileSize][y >> TileSize][x & MOD][y & MOD] += TilesA[x >> TileSize][i >> TileSize][x & MOD][i & MOD] * TilesB[i >> TileSize][y >> TileSize][i & MOD][y & MOD];
            }
        }
    }
    
    finish = clock();
    double fintime = (double)(finish - start) / CLOCKS_PER_SEC;

    FILE *stream;
    stream=fopen("outT.txt", "w");

    fprintf(stream, "%f\n", fintime);
    return 0;
}
