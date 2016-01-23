#ifndef VM_HPP_INCLUDED
#define VM_HPP_INCLUDED

#include <stdint.h>

namespace ash
{
	typedef uint32_t basetype;
	typedef int32_t  cpuval;

	enum instructions
	{
		null = 0, end, mov, push, load, store, add, sub, mul, jmp, jz, jnz, rjmp, print, dup
	};

	constexpr basetype instruction(instructions opcode, uint8_t reg1 = 0, uint8_t reg2 = 0, uint8_t reg3 = 0)
	{
		return (opcode << 24) | (reg1 << 16) | (reg2 << 8) << reg3;
	}

	class VM
	{
	public:
		VM() {}

		enum vmflags
		{
			op_prefetch = 0,
			flags_total
		};

		void setFlag(vmflags flag, bool value);

		void prepare();

	private:
		basetype* program;

		cpuval* registers;
		cpuval* stack;
		uint32_t flags;
	};
}

#endif
