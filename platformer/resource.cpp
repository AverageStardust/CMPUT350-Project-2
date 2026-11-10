#include "resource.h"

#include <filesystem>
#include <iostream>
#include <string>

std::string gResourceRoot = "../assets/";

std::string resourcePath(std::string relativePath) {
    return std::filesystem::path{gResourceRoot}.concat(relativePath);
}

void setupResources(int argc, char* argv[]) {
    if (argc >= 3 && argv[1] == std::string("-path")) {
        gResourceRoot = argv[2];
    }
    std::cout << "Usage: " << argv[0] << " [-path resource_path]\n";
    std::cout << "Using resource path: " << gResourceRoot << "\n";

    // set current working directory to executable location
    std::filesystem::current_path(std::filesystem::path{argv[0]}.parent_path());
}
