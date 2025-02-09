#include <iostream>
#include "SpectrotoolApp/SpectrotoolApp.hpp"


int main(int argc, char *argv[]) {
    try {
        Spectrotool::SpectrotoolApp app;
        app.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
