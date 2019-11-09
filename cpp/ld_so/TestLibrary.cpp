#include <iostream>
#define LIBRARY_API

extern "C" void LIBRARY_API hello();

void hello()
{
    std::cout << "Tom shits his pants!" << std::endl;
}
