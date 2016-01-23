#include "vm.hpp"

#include <cstdio>

using namespace ash;

basetype program[] =
{
	instrbase(mov, ax), 8,
	instrbase(mov, bx), 1,

	instrbase(load, ax), 0,
	instrbase(jz), 15,

	instrbase(load, bx), 0,
	instrbase(print), 0,

	instrbase(load, ax), 0,
	instrbase(load, bx), 0,
	instrbase(mul), 0,
	instrbase(store, bx), 0,

	instrbase(load, ax), 0,
	instrbase(push), 1,
	instrbase(sub), 0,
	instrbase(store, ax), 0,

	instrbase(jmp), 2,

	instrbase(end), 0,
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
