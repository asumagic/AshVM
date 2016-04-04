#include "ash/vm.hpp"
#include "ash/fileimport.h"

#include <cstdio>
#include <stdlib.h>
#include <string.h>

using namespace ash;

#include <chrono>

uint32_t funcTest[]
{
	push, 10,
	call, 4,
	print, 0,
	end, 0,

	// f(x) = 3 * x
	push, 3,
	mul, 0,
	ret, 0,
};


#include <iostream>
int main(int argc, char* argv[])
{
	/*if (argc <= 1)
	{
		perror("Usage : AshVM [file] (arguments)");
		return EXIT_FAILURE;
	}

	programData program = ash::loadProgramFromFile(argv[1]);
	if (program.program == nullptr)
		return EXIT_FAILURE;*/

	//ash::VM virtualMachine(program.program, program.size);
	ash::VM virtualMachine(funcTest, 7);

	uint32_t errCount = 0;

	for (uint32_t i = 2; i < argc; ++i)
	{
		if (argv[i][0] != '-')
		{
			printf("%sParameters must be prefixed by a '-' character (is your filename the first argument?).\n", ash::error);
			++errCount;
		}

		char* arg = argv[i] + 1;

		bool foundMatch = false;
		for (uint32_t j = 0; j < VM::flags_total; ++j)
		{
			if (strcmp(VM::vmflagsStrings[j], arg) == 0)
			{
				virtualMachine.setFlag(static_cast<VM::vmflags>(j));
				foundMatch = true;
				break;
			}
		}

		if (!foundMatch)
		{
			printf("%sUnrecognized commandline argument %s.\n", ash::error, arg);
			++errCount;
		}
	}

	if (errCount != 0)
	{
		printf("%s%u errors encountered! Aborting.", ash::fatalError, errCount);
		return EXIT_FAILURE;
	}

	virtualMachine.prepare();
	virtualMachine.run();

	//delete[] program.program;

    return 0;
}
