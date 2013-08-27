
#include "core/fakefs.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
	Core::FakeFS fs("Hello World !");
	fs.print();
	return 0;
}

