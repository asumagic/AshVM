#include "vm.hpp"

#include <cstdio>

using namespace ash;

basetype program[] =
{
	instrbase(push), 20,
	instrbase(print), 0,
    instrbase(end), 0
};

int main()
{
	puts("Preparing and starting the VM...");

	ash::VM virtualMachine;

	virtualMachine.bindProgram(&program[0], getProgramSize(program));
	virtualMachine.prepare();
	virtualMachine.run();

	puts("Execution done");

    return 0;
}
