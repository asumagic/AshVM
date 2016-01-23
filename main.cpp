#include "vm.hpp"

#include <cstdio>

using namespace ash;

basetype program[] =
{
	instrbase(push), 20,
	instrbase(print), 20,
    instrbase(end), 0
};

int main()
{
	ash::VM virtualMachine;

	puts("Binding program.");
	virtualMachine.bindProgram(&program[0], getProgramSize(program));
	puts("Preparing the VM.");
	virtualMachine.prepare();
	puts("Running the VM.");
	virtualMachine.run();
    return 0;
}
