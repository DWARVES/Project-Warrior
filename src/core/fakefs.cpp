
#include "core/fakefs.hpp"
#include <iostream>

namespace Core
{
		FakeFS::FakeFS(const std::string& str)
			: m_str(str)
		{}

		void FakeFS::print()
		{
			std::cout << m_str << std::endl;
		}
}

