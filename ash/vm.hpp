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
		null = 0, end, push, pop, sget, sset, sgetrel, ssetrel, add, incr, sub, decr, mul, jmp, jz, jnz, jlz, jhz, rjmp, print, dup, dupo, store, load, create, call, ret, lstore, lload, lcreate, OPTOTAL
	};

	static const char* instructionStrings[] =
	{
		"null", "end", "push", "pop", "sget", "sset", "sgetrel", "ssetrel", "add", "incr", "sub", "decr", "mul", "jmp", "jz", "jnz", "jlz", "jhz", "rjmp", "print", "dup", "dupo", "store", "load", "create", "call", "ret", "lstore", "lload", "lcreate"
	};

	struct instruction
	{
		uint32_t opcode;
		basetype value;
	};

	struct funcdata
	{
		uint32_t postcallpc; // pc of the instruction located after the matching function call
		uint32_t localptr;  // local variable table location
	};

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
			pp_list_vm_instructions,
			op_var_prealloc,
			flags_total
		};

		static const char* vmflagsStrings[]; // Matching strings for the VM flags

		void setFlag(vmflags flag, bool value = true); // Sets a VM flag.
		bool getFlag(vmflags flag) const // Checks if a given flag is enabled.
		{
			return static_cast<bool>((flags & (1 << flag)) >> flag);
		}

		void bindProgram(basetype* program, uint _programsize);

		void setStackSize(uint32_t size);
		uint32_t getStackSize() const { return stackSize; }

		instruction fetch(const basetype loc);

		void prepare();
		void run();

		// Note : instructionNext() is located in vm.cpp as a macro
		template<bool updateValue, bool increment = true>
		void instructionHeader(instruction* instr)
		{
			if (updateValue)
				instr->value = instructionArray[pc].value;

			if (increment)
				instr->opcode = instructionArray[++pc].opcode;
			else
				instr->opcode = instructionArray[pc].opcode;
		}

	private:
		inline void stackPush(cpuval val); // Pushes a value to the stack
		inline cpuval stackPopValue(); // Pops a value from the stack and returns it
		inline void stackPop(); // Pops a value from the stack but does not return it

		std::vector<instruction> instructionArray;

		basetype* program; // Pointer to the first byte of the program. An instruction is 32 bits for the opcode + 32 bits for the value. The size is specified by programsize.
		uint programsize;

		std::unique_ptr<cpuval[]> stack; // A stack of a predefined length.
		std::unique_ptr<funcdata[]> callstack; // A list of post function call PCs. Its size define the maximal theorical recursivity depth.
		std::vector<cpuval> globalVariables, localVariables; // Global and local variable vectors

		uint pc = 0, // The program counter which points at the currently interpreted instruction in the program.
		     stackptr = 0,
		     callptr = 0, // Call pointer, the current call depth
		     localtop = 0; // Refers to the top value of the local variable stack

		bool hasInitialized = false, isRunning = false;

		uint32_t stackSize = 8192, // Specifies the stack size (may be modified before preparing)
		         recursivityDepth = 4096, // Maximal recursivity depth
		         flags = 0; // Stores up to 32 flags (may be modified to 64 in the future); to be used with set/getFlag.
	};
}

#endif
