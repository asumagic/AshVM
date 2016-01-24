#include "vm.hpp"

#include <cstdio>

using namespace ash;

/*basetype program[] =
{
	instrbase(mov, ax), 10,
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
};*/

/*	0x02000000, -1000000,	// push 0
	0x02000000, 1,		// push 1
	0x05000000, 0,		// add
	0x0C000000, 2,		// dup
	//0x0B000000, 0,		// print
	0x0A000000, 1,		// jnz 1
	0x00000000,	0,		// end*/

basetype program[] =
{
	instrbase(push), static_cast<basetype>(-10000000),
	instrbase(push), 1,
	instrbase(add), 0,
	instrbase(dup), 2,
	instrbase(jnz), 1,
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
