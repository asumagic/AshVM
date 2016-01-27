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
	push, static_cast<basetype>(-10000000),
	incr, 0,
	dupo, 0,
	jnz, 1,
	end, 0,
};

int main()
{
	puts("Getting the VM ready...");
	ash::VM virtualMachine;

	puts("Binding program and setting flags...");

	virtualMachine.bindProgram(&program[0], getProgramSize(program));
	for (VM::vmflags flag : { VM::dbg_measure_runtime })
		virtualMachine.setFlag(flag);

	puts("Preparing the VM...");
	virtualMachine.prepare();

	puts("Running the VM...");
	virtualMachine.run();

	puts("Execution done");

    return 0;
}
