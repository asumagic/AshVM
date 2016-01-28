#include "ash/vm.hpp"
#include "ash/fileimport.h"

#include <cstdio>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	using namespace ash;

	if (argc <= 1)
	{
		printf(cinNotice);
		perror("Usage : AshVM [file] (arguments)");
		return EXIT_FAILURE;
	}

	programData program = ash::loadProgramFromFile(argv[1]);
	if (program.program == nullptr)
		return EXIT_FAILURE;

	unsigned int errcount = 0;

	VM virtualMachine(program.program, program.size);

	for (uint32_t i = 2; i < argc; ++i)
	{
		if (argv[i][0] != '-')
		{
			printf("%sParameters must be prefixed by a '-' character.\n", cinError);
			++errcount;
		}
		else
		{
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
				printf("%sUnrecognized commandline argument %s!\n", cinError, arg);
				++errcount;
			}
		}
	}

	if (errcount != 0)
	{
		printf("%sVM execution aborted with %u errors.", fatalError, errcount);
		return EXIT_FAILURE;
	}

	virtualMachine.prepare();
	virtualMachine.run();

	delete[] program.program;

    return 0;
}