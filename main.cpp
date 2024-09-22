#include "func.h"
#include "apis.h"
#include "sha1.h"

#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#ifdef TEST
int main() {
    testInit();
    testAdd();
    testCommit();
    testLog();
    std::cout << "All tests passed." << std::endl;
    return 0;
}
#else
int main(int argc, char** argv) {
	Parser parser(argc, argv);
	parser.getArgs();
	parser.parse();

	return 0;
}
#endif