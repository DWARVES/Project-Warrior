
#include "core/fakefs.hpp"
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
    std::istringstream iss(str);
    std::string line;
    std::vector<std::string> ret;
    ret.reserve(5);

    while(std::getline(iss, line, ' '))
        ret.push_back(line);

    return ret;
}

/* For saving */
class Saver {
    public:
        std::string operator()(int* v) const {
            std::ostringstream oss;
            oss << *v;
            return oss.str();
        }
};

/* For loading */
class Loader {
    public:
        int* operator()(const std::string& str) const {
            std::istringstream iss(str);
            int* v = new int;
            iss >> *v;
            return v;
        }
};

bool parseCommand(std::string str, core::FakeFS<int*, core::PointerLiberator<int*>>* fs)
{
    std::vector<std::string> tokens = toTokens(str);
    if(tokens.size() == 0)
        return false;

    if(tokens[0] == "cd") {
        if(tokens.size() < 2)
            return false;
        std::cout << "Entering namespace " << tokens[1] << std::endl;
        return fs->enterNamespace(tokens[1]);
    }
    else if(tokens[0] == "mkdir") {
        if(tokens.size() < 2)
            return false;
        std::cout << "Creating namespace " << tokens[1] << std::endl;
        return fs->createNamespace(tokens[1]);
    }
    else if(tokens[0] == "rmdir") {
        if(tokens.size() < 2)
            return false;
        std::cout << "Deleting namespace " << tokens[1] << std::endl;
        fs->deleteNamespace(tokens[1]);
        return true;
    }
    else if(tokens[0] == "pwd") {
        std::cout << "Actual namespace : " << fs->actualNamespace() << std::endl;
        return true;
    }
    else if(tokens[0] == "touch") {
        if(tokens.size() < 3)
            return false;
        std::istringstream iss(tokens[2]);
        int* v = new int;
        iss >> *v;
        std::cout << "Creating entity " << tokens[1] << " with value " << *v << std::endl;
        if(fs->createEntity(tokens[1], v))
            return true;
        else {
            delete v;
            return false;
        }
    }
    else if(tokens[0] == "rm") {
        if(tokens.size() < 2)
            return false;
        std::cout << "Deleting entity " << tokens[1] << std::endl;
        fs->deleteEntity(tokens[1]);
        return true;
    }
    else if(tokens[0] == "cat") {
        if(tokens.size() < 2)
            return false;
        int* v = fs->getEntityValue(tokens[1]);
        if(!v) {
            if(!fs->existsEntity(tokens[1])) {
                std::cout << "Entity " << tokens[1] << "does not exists." << std::endl;
                return false;
            }
            else {
                std::cout << tokens[1] << " contains NULL." << std::endl;
                return true;
            }
        }
        std::cout << tokens[1] << " contains " << *v << "." << std::endl;
        return true;
    }
    else if(tokens[0] == "ln") {
        if(tokens.size() < 3)
            return false;
        std::cout << "Linking " << tokens[2] << " as " << tokens[1] << std::endl;
        return fs->link(tokens[1], tokens[2]);
    }
    else if(tokens[0] == "set") {
        if(tokens.size() < 3)
            return false;
        std::istringstream iss(tokens[2]);
        int* v = fs->getEntityValue(tokens[1]);
        if(v)
            delete v;
        v = new int;
        iss >> *v;
        std::cout << "Setting " << tokens[1] << " to " << *v << std::endl;
        return fs->setEntityValue(tokens[1], v);
    }
    else if(tokens[0] == "ls") {
        std::vector<std::string> list = fs->listNamespaces();
        for(size_t i = 0; i < list.size(); ++i) {
            std::cout << i << ". " << list[i] << "/" << std::endl;
        }
        list = fs->listEntities();
        for(size_t i = 0; i < list.size(); ++i) {
            std::cout << i << ". " << list[i] << std::endl;
        }
        return true;
    }
    else if(tokens[0] == "clear") {
        fs->clear();
        return true;
    }
    else if(tokens[0] == "save") {
        Saver sav;
        if(tokens.size() < 2) {
            fs->save(std::cout, sav);
        }
        else {
            std::ofstream ofs(tokens[1]);
            if(!ofs) return false;
            fs->save(ofs, sav);
        }
        return true;
    }
    else if(tokens[0] == "load") {
        if(tokens.size() < 2)
            return false;
        Loader ld;
        std::ifstream ifs(tokens[1]);
        if(!ifs) return false;
        fs->load(ifs, ld);
        return true;
    }
    else if(tokens[0] == "exec") {
        /* Load a script and execute it */
        if(tokens.size() < 2)
            return false;
        std::ifstream ifs(tokens[1]);
        if(!ifs)
            return false;
        std::cout << ">>>>>>>> Executing " << tokens[1] << " <<<<<<<<<<" << std::endl;
        std::string line;
        while(std::getline(ifs, line)) {
            if(line.empty() || line[0] == '#')
                continue;
            if(!parseCommand(line, fs)) {
                std::cout << ">>>>>>>> Error, stopped execution of " << tokens[1] << std::endl;
                return false;
            }
        }
        std::cout << ">>>>>>>> End of " << tokens[1] << " <<<<<<<<<<" << std::endl;
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

    core::FakeFS<int*, core::PointerLiberator<int*>> fs;
    std::string cmd;

    while(true) {
        std::cout << "> ";
        std::getline(std::cin, cmd);
        if(cmd == "quit")
            break;
        if(parseCommand(cmd, &fs))
            std::cout << "Command correctly termined." << std::endl;
        else
            std::cout << "Error while executing command." << std::endl;
    }

#ifdef USELOG
    core::logger::free();
#endif
    return 0;
}

