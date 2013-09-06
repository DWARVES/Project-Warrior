
#include "lua/script.hpp"
#include "lua/helper.hpp"
#include "core/logger.hpp"
#include <iostream>

int function(lua_State* st)
{
    std::vector<lua::Script::VarType> args = lua::helper::listArguments(st);
    std::cout << "Function called with " << args.size() << " arguments." << std::endl;

    for(size_t i = 0; i < args.size(); ++i)
    {
        switch(args[i]) {
            case lua::Script::NUMBER: std::cout << i+1 << "eme arg is a number : " << lua_tonumber(st, (int)i+1) << std::endl;   break;
            case lua::Script::STRING: std::cout << i+1 << "eme arg is a string : " << lua_tostring(st, (int)i+1) << std::endl;   break;
            case lua::Script::BOOL:   std::cout << i+1 << "eme arg is a boolean : " << lua_toboolean(st, (int)i+1) << std::endl; break;
            case lua::Script::TABLE:  std::cout << i+1 << "eme arg is a table.";                                               break;
            case lua::Script::USER:   std::cout << i+1 << "eme arg is a userdata.";                                            break;
            case lua::Script::NIL:    std::cout << i+1 << "eme arg is a nil value.";                                           break;
            default:                  std::cout << i+1 << "eme arg is of unknown type." << std::endl;                          break;
        }
    }

    return lua::helper::returnString(st, "Hy everybody out of here");
}

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    core::logger::logm("Begin of the program", core::logger::MSG);

    try {
        /* Creating */
        core::logger::logm("Creating lua script object.", core::logger::MSG);
        lua::Script scr;

        /* Registering functions */
        core::logger::logm("Testing exposure of functions.", core::logger::MSG);
        scr.registerFunction(function, "xargs");

        /* loading */
        core::logger::logm("Loading lua script.", core::logger::MSG);
        if(!scr.load("script.lua")) {
            core::logger::logm("Couldn't load s.lua.", core::logger::ERROR);
            throw lua::exception("Script couldn't be loaded.");
        }

        /* Variables methods */
        std::cout << "Type of \"var\" variable : " << scr.typeVariable("var") << std::endl;
        std::cout << "Type of \"num\" variable : " << scr.typeVariable("num") << std::endl;
        std::string sval;
        bool bval;
        double dval;
        scr.getVariable("var", sval);
        std::cout << "Value of \"var\" variable : " << sval << std::endl;
        scr.getVariable("num", bval);
        std::cout << "Value of \"num\" variable : " << bval << std::endl;
        scr.setVariable("num", "Hello world !");
        scr.getVariable("num", sval);
        std::cout << "Value of \"num\" variable : " << sval << std::endl;
        scr.setVariable("num", 3.14);
        scr.getVariable("num", dval);
        std::cout << "Value of \"num\" variable : " << dval << std::endl;

        /* Function methods */
        core::logger::logm("Testing and calling functions.", core::logger::MSG);
        if(!scr.existsFunction("un")) {
            std::cout << "Function un doesn't exists." << std::endl;
        }
        if(scr.existsFunction("echo")) {
            scr.callFunction<void>("echo", NULL, "Hello world", "Second line", 27, false);
        }
        if(scr.existsFunction("add")) {
            float r;
            scr.callFunction("add", &r, 1.f, 2.5f, 50.f);
            std::cout << "Ret value for add : " << r << std::endl;
        }
        scr.callFunction<void>("unexists", NULL); /* Try call of unexistent function */
    }
    catch(const lua::exception& e) {
        core::logger::logm(std::string("Lua exception catched : ") + e.what(), core::logger::FATAL);
        return 1;
    }
    catch(const std::exception& e) {
        core::logger::logm(std::string("Standart exception catched : ") + e.what(), core::logger::FATAL);
        return 1;
    }
    catch(...) {
        core::logger::logm("Unknown exception catched.", core::logger::FATAL);
        return 1;
    }

    core::logger::free();
    return 0;
}

