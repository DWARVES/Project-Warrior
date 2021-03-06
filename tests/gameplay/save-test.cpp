
#include "gameplay/save.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#define USELOG
#ifdef USELOG
#include "core/logger.hpp"
#endif

std::vector<std::string> toTokens(const std::string& str)
{
    std::string cpy(str);
    std::size_t last = 0;
    std::size_t found = cpy.find(' ');
    std::vector<std::string> ret;

    while(found != std::string::npos) {
        std::string tmp = cpy.substr(last, found - last);
        if(!tmp.empty())
            ret.push_back(tmp);
        last = found + 1;
        cpy[found] = '_'; /* To be sure it won't be found again */
        found = cpy.find(' ');
    }

    std::string tmp = cpy.substr(last, cpy.size() - last);
    if(!tmp.empty())
        ret.push_back(tmp);

    return ret;
}

bool parseCommand(std::string str)
{
    gameplay::Save save;

    std::vector<std::string> tokens = toTokens(str);
    if(tokens.size() == 0)
        return false;

    if(tokens[0] == "cd") {
        if(tokens.size() < 2)
            return false;
        std::cout << "Entering namespace " << tokens[1] << std::endl;
        return save.enterNamespace(tokens[1]);
    }
    else if(tokens[0] == "mkdir") {
        if(tokens.size() < 2)
            return false;
        std::cout << "Creating namespace " << tokens[1] << std::endl;
        return save.createNamespace(tokens[1]);
    }
    else if(tokens[0] == "rmdir") {
        if(tokens.size() < 2)
            return false;
        std::cout << "Deleting namespace " << tokens[1] << std::endl;
        save.deleteNamespace(tokens[1]);
        return true;
    }
    else if(tokens[0] == "pwd") {
        std::cout << "Actual namespace : " << save.actualNamespace() << std::endl;
        return true;
    }
    else if(tokens[0] == "touch") {
        if(tokens.size() < 3)
            return false;
        std::istringstream iss(tokens[2]);
        long int v;
        iss >> v;
        std::cout << "Creating entity " << tokens[1] << " with value " << v << std::endl;
        if(save.createVariable(tokens[1], v))
            return true;
        else
            return false;
    }
    else if(tokens[0] == "rm") {
        if(tokens.size() < 2)
            return false;
        std::cout << "Deleting entity " << tokens[1] << std::endl;
        save.deleteVariable(tokens[1]);
        return true;
    }
    else if(tokens[0] == "cat") {
        if(tokens.size() < 2)
            return false;
        long int v = save.getVariable(tokens[1]);
        if(!v) {
            if(!save.existsVariable(tokens[1])) {
                std::cout << "Entity " << tokens[1] << "does not exists." << std::endl;
                return false;
            }
            else {
                std::cout << tokens[1] << " contains NULL." << std::endl;
                return true;
            }
        }
        std::cout << tokens[1] << " contains " << v << "." << std::endl;
        return true;
    }
    else if(tokens[0] == "set") {
        if(tokens.size() < 3)
            return false;
        std::istringstream iss(tokens[2]);
        long int v;
        iss >> v;
        std::cout << "Setting " << tokens[1] << " to " << v << std::endl;
        return save.setVariable(tokens[1], v);
    }
    else if(tokens[0] == "save") {
        if(tokens.size() < 2)
            return false;
        save.save(tokens[1]);
        return true;
    }
    else if(tokens[0] == "load") {
        if(tokens.size() < 2)
            return false;
        save.load(tokens[1]);
        return true;
    }
    else
        return false;
}

int main()
{
#ifdef USELOG
    core::logger::init();
    core::logger::addOutput(&std::cout, false);
#endif

    gameplay::Save::init();
    std::string cmd;

    while(true) {
        std::cout << "> ";
        std::getline(std::cin, cmd);
        if(cmd == "quit")
            break;
        if(parseCommand(cmd))
            std::cout << "Command correctly termined." << std::endl;
        else
            std::cout << "Error while executing command." << std::endl;
    }

    gameplay::Save::quit();
#ifdef USELOG
    core::logger::free();
#endif
    return 0;
}

