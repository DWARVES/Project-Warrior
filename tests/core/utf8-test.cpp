
#include <iostream>
#include <string>
#include <cstring>
#include "core/utf8.hpp"

void dumpCmp(const std::string& str);

int main()
{
    std::string str = "a\x80\xe0\xa0\xc0\xaf\xed\xa0\x80z";
    dumpCmp(str);
    str = "Où est Anaïs.";
    dumpCmp(str);
    str = "\xe6\x97\xa5\xd1\x88";
    dumpCmp(str);
    return 0;
}

void dumpCmp(const std::string& str)
{
    core::UTF8String utf(str);
    std::cout << "Comparing \"" << str << "\" :" << std::endl;
    std::cout << "\t> Valid ? " << utf.valid() << std::endl;
    if(!utf.valid()) {
        utf.removeErrors();
        std::cout << "\t> Validated \"" << utf << std::endl;
    }
    std::cout << "\t> Size : " << str.size() << " vs " << utf.size() << std::endl;

    std::cout << "\t> Letters :" << std::endl;
    for(size_t i = 0; i < utf.size(); ++i) {
        char toprint[5];
        unsigned int letter = utf[i];
        memcpy(toprint, &letter, sizeof(unsigned int));
        toprint[4] = '\0';
        std::cout << "\t\t" << i << ". " << toprint << " (" << letter << ")" << std::endl;
    }
}

