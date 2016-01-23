#include "vm.hpp"

namespace ash
{
	void VM::setFlag(vmflags flag, bool value)
	{
		flags |= value << flag;
	}
}
