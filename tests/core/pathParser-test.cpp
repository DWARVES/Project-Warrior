
#include <iostream>
#include "core/pathParser.hpp"

int main()
{
    std::string path;
    std::cout << "Enter a path (mustn't necessarily exists) : ";
    std::cin >> path;
    std::cout << "You entered \"" << path << "\"" << std::endl;

    std::cout << "Without doubles : " << core::path::cleanSep(path) << std::endl;

    std::vector<std::string> parts = core::path::parts(path);
    std::cout << "Begin parts :" << std::endl;
    for(size_t i = 0; i < parts.size(); ++i) {
        std::cout << "\tPart " << i << " : " << parts[i] << std::endl;
    }
    std::cout << "End parts." << std::endl;

    std::cout << "The extension : " << core::path::extension(path) << std::endl;

    std::cout << "The head : " << core::path::head(path, true) << " (without extension : " << core::path::head(path, false) << ")" << std::endl;

    std::cout << "Is absolute ? " << (core::path::absolute(path) ? "yes" : "no") << std::endl;

    core::path::Type t = core::path::type(path);
    std::string str;
    switch(t) {
        case core::path::Type::Reg:
            str = "regular file"; break;
        case core::path::Type::Dir:
            str = "directory"; break;
        case core::path::Type::Socket:
            str = "socket"; break;
        case core::path::Type::Unknown:
            str = "unknown"; break;
        case core::path::Type::Unexistant:
            str = "unexistant"; break;
        default:
            str = "must NOT happen :"; break;
    }
    std::cout << "Type : " << str << std::endl;

    if(t == core::path::Type::Dir) {
        std::vector<std::string> contents = core::path::dirContents(path);
        std::cout << "Begin directory contents." << std::endl;
        for(size_t i = 0; i < contents.size(); ++i) {
            std::cout << "\tEntity " << i << " : " << contents[i] << std::endl;
        }
        std::cout << "End directory contents." << std::endl;
    }

    return 0;
}

