#include "vm.hpp"

#define ALLOW_TIME_MEASURE

#include <cstdio>

#ifdef ALLOW_TIME_MEASURE
#include <chrono>
#endif

#define instructionNext() goto *labels[instr->opcode];

namespace ash
{
	VM::VM() {}
	VM::VM(basetype* _program, uint _programsize)
	{
		bindProgram(_program, _programsize);
	}

	VM::~VM() {}

	const char* VM::vmflagsStrings[] =
	{
		"pp_noprint",
		"pp_dbg_list",
		"dbg_measure_runtime",
		"pp_list_vm_instructions",
		"op_var_prealloc",
		"op_jit"
	};

	void VM::setFlag(vmflags flag, bool value)
	{
		flags |= value << flag;
	}

	void VM::bindProgram(basetype* _program, uint _programsize)
	{
		program = _program;
		programsize = _programsize;
	}

	void VM::prepare()
	{
		instructionArray.resize(programsize);
		if (getFlag(op_var_prealloc)) // Reserves a hardcoded amount of variables to optimize variable creation
			variables.reserve(8192);
		stack = std::unique_ptr<cpuval[]>(new cpuval[stackSize]);

		if (getFlag(pp_list_vm_instructions))
        {
            printf("Available instructions (%u) :\n", OPTOTAL);
            for (uint i = 0; i < OPTOTAL; i++)
            {
                printf("- %s (%u)\n", instructionStrings[i], i);
            }
        }

		hasInitialized = true;
	}

	inline instruction VM::fetch(const basetype loc)
	{
		return instruction{ program[loc],
                            program[loc + 1] };
	}

	void VM::setStackSize(uint32_t size)
	{
		stackSize = size;
	}

	inline void VM::stackPush(cpuval val)
	{
		stack[++stackptr] = val;
	}

	inline cpuval VM::stackPopValue()
	{
		return stack[stackptr--];
	}

	inline void VM::stackPop()
	{
		--stackptr;
	}

	void VM::run()
	{
		void* labels[OPTOTAL + 1] = { &&lNull, &&lEnd, &&lPush, &&lPop, &&lAdd, &&lIncr, &&lSub, &&lDecr, &&lMul, &&lJmp, &&lJz, &&lJnz, &&lJlz, &&lJhz, &&lRjmp, &&lPrint, &&lDup, &&lDupO, &&lStore, &&lLoad, &&lCreate };

		const bool noPrint = getFlag(pp_noprint),
				   measureTime = getFlag(dbg_measure_runtime),
				   printInstructions = getFlag(pp_dbg_list);

#ifdef ALLOW_TIME_MEASURE // TODO : Find a more elegant way so we don't add overhead when not using -dbg_measure_runtime
		using namespace std::chrono;
		auto measureStart = system_clock::now();
#else
		if (measureTime)
		{
			puts("Error : The VM was not compiled with ALLOW_TIME_MEASURE. Time measurement is not possible.");
		}
#endif

		for (uint i = 0; i < programsize; ++i)
		{
			instruction instr = fetch(i * 2);

			if (noPrint && instr.opcode == print)
				instr.opcode = pop;

			if (printInstructions)
				printf("[%u] %s (%d) %d\n", i, instructionStrings[instr.opcode], instr.opcode, instr.value);

			instructionArray[i] = instr;
		}

		isRunning = true;

		instruction* instr = new instruction;
		instr->opcode = instructionArray[0].opcode;
		instructionNext();

		lNull:
		{
			instructionHeader<false>(instr);
			puts("Null instruction encountered.");
			instructionNext();
		}

		lEnd:
#ifdef ALLOW_TIME_MEASURE
            delete instr;
			if (measureTime)
			{
				auto timeElapsed = duration<float, std::milli>(system_clock::now() - measureStart);
				printf("Time elapsed : %fms\n", timeElapsed.count());
			}
#endif
			return;

		lPush: instructionHeader<true>(instr);
			stackPush(instr->value);
			instructionNext();

		lPop: instructionHeader<false>(instr);
			stackPop();
			instructionNext();

		lAdd: instructionHeader<false>(instr);
			stackPush(stackPopValue() + stackPopValue());
			instructionNext();

		lIncr: instructionHeader<false>(instr);
			++stack[stackptr];
			instructionNext();

		lSub: {	instructionHeader<false>(instr);
			cpuval b = stackPopValue();
			stackPush(stackPopValue() - b);
			instructionNext();
		}

		lDecr: instructionHeader<false>(instr);
			--stack[stackptr];
			instructionNext();

		lMul: instructionHeader<false>(instr);
			stackPush(stackPopValue() * stackPopValue());
			instructionNext();

		lJmp: instructionHeader<true>(instr);
			pc = instr->value;
			instructionHeader<false, false>(instr); // Update the pc
			instructionNext();

		lJz: instructionHeader<true>(instr);
			if (stackPopValue() == 0)
			{
				pc = instr->value;
				instructionHeader<false, false>(instr);
			}
			instructionNext();

		lJnz: instructionHeader<true>(instr);
			if (stackPopValue() != 0)
			{
				pc = instr->value;
				instructionHeader<false, false>(instr);
			}
			instructionNext();

		lJlz: instructionHeader<true>(instr);
		    if (stackPopValue() < 0)
			{
				pc = instr->value;
				instructionHeader<false, false>(instr);
			}
			instructionNext();

		lJhz: instructionHeader<true>(instr);
            if (stackPopValue() < 0)
			{
				pc = instr->value;
				instructionHeader<false, false>(instr);
			}
			instructionNext();

		lRjmp: instructionHeader<true>(instr);
			pc += instr->value;
			instructionHeader<false, false>(instr);
			instructionNext();

		lPrint: instructionHeader<false>(instr);
			printf("%d\n", stackPopValue());
			instructionNext();

		lDup: { instructionHeader<true>(instr);
			const cpuval toRepeat = stack[stackptr];
			const uint target = stackptr + instr->value;
			while (stackptr < target)
			{
				stack[++stackptr] = toRepeat;
			}
			instructionNext();
		}

		lDupO: { instructionHeader<false>(instr);
			const cpuval val = stack[stackptr];
			stack[++stackptr] = val;
			instructionNext();
		}

		lLoad: instructionHeader<true>(instr);
            stackPush(variables[instr->value]);
			instructionNext();

		lStore: instructionHeader<true>(instr);
            variables[instr->value] = stackPopValue();
			instructionNext();

		lCreate: instructionHeader<true>(instr);
		    if (variables.size() <= instr->value)
                variables.resize(instr->value + 1);
			instructionNext();
	}
}
