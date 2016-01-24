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

basetype program[] =
{
	instrbase(push), static_cast<basetype>(-10000000),
	instrbase(incr), 0,
	instrbase(dupo), 0,
	instrbase(jnz), 1,
	instrbase(end), 0,
};

int main()
{
	puts("Preparing and starting the VM...");

	ash::VM virtualMachine;

	virtualMachine.bindProgram(&program[0], getProgramSize(program));
	for (VM::vmflags flag : { VM::flag_noprint, VM::op_morph_dup_dupo })
		virtualMachine.setFlag(flag);

	virtualMachine.prepare();
	virtualMachine.run();

	puts("Execution done");

    return 0;
}
