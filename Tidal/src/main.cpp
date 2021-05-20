#include "PCHeader.h"
#include "./Core/App.h"

int main(int argc, char const *argv[])
{
    App app;
    if (app.init())
    {
        app.run();
    }

    return 0;
}