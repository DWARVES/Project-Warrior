
#ifndef DEF_CORE_FAKEFS
#define DEF_CORE_FAKEFS

#include <string>

namespace Core
{
	class FakeFS
	{
		public:
			FakeFS(const std::string& str);
			void print();

		private:
			std::string m_str;
	};
}

#endif

