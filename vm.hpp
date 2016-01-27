#ifndef VM_HPP_INCLUDED
#define VM_HPP_INCLUDED

#include <stdint.h>
#include <vector>
#include <memory>

#define getProgramSize(program) sizeof(program) / (sizeof(uint32_t) * 2)

namespace ash
{
	typedef uint32_t basetype;
	typedef int32_t  cpuval;
	typedef unsigned int uint;

	enum instructions
	{
		null = 0, end, push, pop, add, incr, sub, decr, mul, jmp, jz, jnz, rjmp, print, dup, dupo, OPTOTAL
	};

	static const char* instructionStrings[] =
	{
		"null", "end", "push", "pop", "add", "incr", "sub", "decr", "mul", "jmp", "jz", "jnz", "rjmp", "print", "dup", "dupo"
	};

	struct instruction
	{
		uint32_t opcode;
		basetype value;
	};

	class VM
	{
	public:
		VM();
		~VM();

		enum vmflags
		{
			flag_noprint,
			dbg_list_preprocessed,
			dbg_measure_runtime
		};

		void setFlag(vmflags flag, bool value = true);
		bool getFlag(vmflags flag) const
		{
			return static_cast<bool>((flags & (1 << flag)) >> flag);
		}

		void bindProgram(basetype* program, uint _programsize);

		void setStackSize(uint32_t size);
		uint32_t getStackSize() const { return stackSize; }

		instruction fetch(const basetype loc);

		void prepare();
		void run();

	private:
		inline void stackPush(cpuval val);
		inline cpuval stackPopValue();
		inline void stackPop();

		std::vector<instruction> instructionArray;

		basetype* program;
		uint32_t value;
		uint programsize;

		std::unique_ptr<cpuval[]> stack;

		uint pc = -1;
		uint stackptr = 1;

		bool hasInitialized = false, isRunning = false;

		uint32_t stackSize = 512;
		uint32_t flags = 0;
	};
}

#endif
