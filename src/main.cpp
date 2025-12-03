#include <config.h>
#include <iostream>

int main()
{
    std::cout << "Version:" << +PROJECT_VERSION_MAJOR << "." << +PROJECT_VERSION_MINOR << "." <<
        +PROJECT_VERSION_PATCH << "." << +PROJECT_VERSION_TWEAK << "." << std::endl;
    return 0;
}