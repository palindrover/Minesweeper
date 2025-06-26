#include "cell.h"
#include <vector>
#include <cstdlib>
#include <time.h>

#ifndef FIELD_H
#define FIELD_H

class Field
{
public:
    std::vector<std::vector<Cell>> field;
    Field(int x, int y, int bombs);
    ~Field();
};

#endif 