/*
 * This is the client-server main
 */

#include <iostream>

#include "common/MyQApp.h"
#include "ServerPlatform.h"
#include "common/Viewer.h"

int main(int argc, char ** argv)
{
    // setup Qt
    MyQApp app(argc, argv);

    std::cerr << "defer: deferring\n";
    defer( []() {
        std::cerr << "defer: running, yay\n";

        std::cerr << "+defer: deferring\n";
        defer( []() {
            std::cerr << "+defer: running, yay\n";

        });
        std::cerr << "+defer: deferred\n";


    });
    std::cerr << "defer: deferred\n";


    // create a platform
    MyQApp::setPlatform( new ServerPlatform( argc, argv));

    // run the viewer with this platorm
    Viewer viewer( MyQApp::platform());
    viewer.start();

    return app.exec();
}


