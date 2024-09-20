#include "func.h"
#include "apis.h"
#include "sha1.h"

int main(int argc, char** argv) {
	Parser parser(argc, argv);
	parser.getArgs();
	parser.parse();

	return 0;
}