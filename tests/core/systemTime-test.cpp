
#include "core/systemtime.hpp"
#include <iostream>

int main()
{
    core::SystemTime st;
    std::cout << "Default constructor time (localtime) : " << st << std::endl;
    st.set(90000, false);
    std::cout << "With set(90000, false) : " << st << std::endl;
    st.set(90000, true);
    std::cout << "With set(90000, true) : " << st << std::endl;
    st.set(1996, 9, 19, 23, 35, 37);
    std::cout << "With set(1998, 10, 17, 23, 35, 37) : " << st << std::endl;
    std::cout << "Using independant access : " << std::endl
        << "\t > YEAR : " << st.get(core::SystemTime::YEAR) << std::endl
        << "\t > LYEAR : " << st.get(core::SystemTime::LYEAR) << std::endl
        << "\t > MDAY : " << st.get(core::SystemTime::MDAY) << std::endl
        << "\t > WDAY : " << st.get(core::SystemTime::WDAY) << std::endl
        << "\t > YDAY : " << st.get(core::SystemTime::YDAY) << std::endl
        << "\t > MONTH : " << st.get(core::SystemTime::MONTH) << std::endl
        << "\t > HOUR : " << st.get(core::SystemTime::HOUR) << std::endl
        << "\t > MINUTE : " << st.get(core::SystemTime::MINUTE) << std::endl
        << "\t > SECONDS : " << st.get(core::SystemTime::SECONDS) << std::endl;

    st.useUTCTime();
    std::cout << "With UTC time : " << st << std::endl;
    st.useSystemTime();
    std::cout << "With system time : " << st << std::endl;
    std::cout << "Using independant access : " << std::endl
        << "\t > YEAR : " << st.get(core::SystemTime::YEAR) << std::endl
        << "\t > LYEAR : " << st.get(core::SystemTime::LYEAR) << std::endl
        << "\t > MDAY : " << st.get(core::SystemTime::MDAY) << std::endl
        << "\t > WDAY : " << st.get(core::SystemTime::WDAY) << std::endl
        << "\t > YDAY : " << st.get(core::SystemTime::YDAY) << std::endl
        << "\t > MONTH : " << st.get(core::SystemTime::MONTH) << std::endl
        << "\t > HOUR : " << st.get(core::SystemTime::HOUR) << std::endl
        << "\t > MINUTE : " << st.get(core::SystemTime::MINUTE) << std::endl
        << "\t > SECONDS : " << st.get(core::SystemTime::SECONDS) << std::endl;

    core::SystemTime nt(st);
    std::cout << "Copied time from st : " << nt << std::endl;
    nt.set(2013, 9, 4, 8, 0, 0); /* Must be older than localtime, change it for tests */
    std::cout << "With set(2013, 9, 4, 8, 0, 0) : " << nt << std::endl;

    core::SystemTime rt = nt - st;
    std::cout << "[[ " << nt << " ]] - [[ " << st << " ]] = " << rt.format("%j days %H hours %M minutes and %S seconds", 50) << std::endl;
}

