// unistd.cpp
#include <iostream>
#include "unistd.h"

void errif(bool condition, const char *errmsg)
{
    if (condition)
    {
        std::cout << errmsg << std::endl;
        exit(EXIT_FAILURE);
    }
}