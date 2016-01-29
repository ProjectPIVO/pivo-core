#include "General.h"
#include "Application.h"

int main(int argc, char** argv)
{
    if (!sApplication->Init(argc, argv))
        return -1;

    return sApplication->Run();
}
