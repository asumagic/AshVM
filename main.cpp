#include "ash/vm.hpp"
#include "ash/fileimport.h"

#include <cstdio>
#include <stdlib.h>
#include <string.h>

using namespace ash;

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		perror("Usage : AshVM [file] (arguments)");
		return EXIT_FAILURE;
	}

	programData program = ash::loadProgramFromFile(argv[1]);
	if (program.program == nullptr)
		return EXIT_FAILURE;

	ash::VM virtualMachine(program.program, program.size);

	for (uint32_t i = 2; i < argc; ++i)
	{
		if (argv[i][0] != '-')
		{
			puts("Parameters must be prefixed by a '-' character.");
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
			printf("Unrecognized commandline argument %s!\n", arg);
		}
	}

	virtualMachine.prepare();
	virtualMachine.run();

	delete[] program.program;

    return 0;
}