#include "vm.hpp"

using namespace ash;

basetype program[]
{
	instruction<push>(), 20,
	instruction<print>(), 20,
    instruction<end>(), 0
};

int main()
{
	ash::VM virtualMachine;
	virtualMachine.setFlag(VM::op_prefetch, true);
    return 0;
}
