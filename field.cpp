#include "field.h"

Field::Field(int x, int y)
{
    field.resize(x, std::vector<Cell>(y));

    int bombs = 0;

    if (x == 8 && y == 8) bombs = 10;

    srand(time(NULL));

    for (; bombs > 0; bombs--)
    {
        int rx = rand() % x, ry = rand() % y;
        if (field[rx][ry].isMine) bombs++;
        else field[rx][ry].isMine = 1;
    }

    for (int a = 0; a < x; a++)
    {
        for (int b = 0; b < y; b++)
        {
           
            if (field[a][b].isMine) continue;

            for (int i = a - 1; i < a + 2; i++)
            {
                for (int j = b - 1; j < b + 2; j++)
                {
                    if (i > -1 && i < x && j > -1 && j < y && field[i][j].isMine) field[a][b].num++;
                }
            }
        }
    }
}

Field::~Field() {}