#include "ash/vm.hpp"
#include "ash/fileimport.h"

#include <cstdio>
#include <stdlib.h>
#include <string.h>

using namespace ash;

#include <chrono>

uint32_t programFibonacci[] =
{
		push, 50000000, // 285212671

		push, 0,
		create, 2,
		store, 2, // first = 0

		push, 1,
		create, 3,
		store, 3, // second = 0

		load, 2,
		load, 3,
		add, 0, // tmp = first + second

		load, 3,
		store, 2, // first = second

		store, 3, // second = tmp

		decr, 0, // --n
		dupo, 0,

		jnz, 7, // jnz 7

		end, 0,
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
	ash::VM virtualMachine(programFibonacci, 17);

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
