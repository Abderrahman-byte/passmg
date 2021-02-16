#include <iostream>
#include "utils.h"

#define DB_PATH "./pass.db"

int main() {
	std::cout << "C++ " << __cplusplus << std::endl;

	if(!file_exists(DB_PATH)) {
		std::cout << "First Time" << std::endl;
	}

	return 0;
}
