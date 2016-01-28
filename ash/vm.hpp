#ifndef VM_HPP_INCLUDED
#define VM_HPP_INCLUDED

#include <stdint.h>
#include <vector>
#include <memory>

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

	const char fatalError[] = "[ XX ] ";

	template<typename T, size_t size>
	constexpr size_t getArraySize(T(&)[size])
	{
		return size;
	}

	class VM
	{
	public:
		VM();
		VM(basetype* program, uint _programsize);
		~VM();

		enum vmflags
		{
			pp_noprint,
			pp_dbg_list,
			dbg_measure_runtime,
			flags_total
		};

		static const char* vmflagsStrings[];

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

		uint pc = static_cast<uint>(-1); // Relies on overflowing. To change?
		uint stackptr = 1;

		bool hasInitialized = false, isRunning = false;

		uint32_t stackSize = 512;
		uint32_t flags = 0;
	};
}

#endif
