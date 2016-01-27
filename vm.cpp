#include "vm.hpp"

#define ALLOW_TIME_MEASURE

#include <cstdio>

#ifdef ALLOW_TIME_MEASURE
#include <chrono>
#endif

namespace ash
{
	VM::VM() {}

	VM::~VM() {}

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
		stack = std::unique_ptr<cpuval[]>(new cpuval[stackSize]);

		hasInitialized = true;
	}

	inline instruction VM::fetch(const basetype loc)
	{
		return instruction{ program[loc], program[loc + 1] };
	}

	void VM::setStackSize(uint32_t size)
	{
		stackSize = size;
	}

	inline void VM::stackPush(cpuval val)
	{
		stack[++stackptr] = val;
		if (stackptr == stackSize)
			puts("Stack overflow!");
	}

	inline cpuval VM::stackPopValue()
	{
		if (stackptr == 0)
			puts("Stack underflow!");
		return stack[stackptr--];
	}

	inline void VM::stackPop()
	{
		--stackptr;
	}

	void VM::run()
	{
		void* labels[OPTOTAL] = { &&lNull, &&lEnd, &&lPush, &&lPop, &&lAdd, &&lIncr, &&lSub, &&lDecr, &&lMul, &&lJmp, &&lJz, &&lJnz, &&lRjmp, &&lPrint, &&lDup, &&lDupO };

		const bool noPrint = getFlag(flag_noprint);
		const bool measureTime = getFlag(dbg_measure_runtime);
		const bool printInstructions = getFlag(dbg_list_preprocessed);

#ifdef ALLOW_TIME_MEASURE // TODO : Find a more elegant way so we don't add overhead when not using -dbg_measure_runtime
		using namespace std::chrono;
		auto measureStart = high_resolution_clock::now();
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
				printf("%s %d\n", instructionStrings[instr.opcode], instr.value);

			instructionArray[i] = instr;
		}

		isRunning = true;

		lBack:
			pc++;
		lBackNoIncrement:
			const instruction& instr = instructionArray[pc];
			value = instr.value;
			goto *labels[instr.opcode];

		lNull:
		{
			puts("Encountered null instruction.");
			goto lBack;
		}

		lEnd:
#ifdef ALLOW_TIME_MEASURE
			if (measureTime)
			{
				auto timeElapsed = duration<float, std::milli>(high_resolution_clock::now() - measureStart);
				printf("Time elapsed : %fms\n", timeElapsed.count());
			}
#endif
			return;

		lPush:
			stackPush(value);
			goto lBack;

		lPop:
			stackPop();
			goto lBack;

		lAdd:
			stackPush(stackPopValue() + stackPopValue());
			goto lBack;

		lIncr:
			++stack[stackptr];
			goto lBack;

		lSub:
		{
			cpuval b = stackPopValue();
			stackPush(stackPopValue() - b);
			goto lBack;
		}

		lDecr:
			--stack[stackptr];
			goto lBack;

		lMul:
			stackPush(stackPopValue() * stackPopValue());
			goto lBack;

		lJmp:
			pc = value;
			goto lBackNoIncrement;

		lJz:
			if (stackPopValue() == 0)
			{
				pc = value;
				goto lBackNoIncrement;
			}
			goto lBack;

		lJnz:
			if (stackPopValue() != 0)
			{
				pc = value;
				goto lBackNoIncrement;
			}
			goto lBack;

		lRjmp:
			pc += value;
			goto lBack;

		lPrint:
			printf("%d\n", stackPopValue());
			goto lBack;

		lDup:
		{
			const cpuval toRepeat = stack[stackptr];
			const uint target = stackptr + value;
			while (stackptr < target)
			{
				stack[++stackptr] = toRepeat;
			}
			goto lBack;
		}

		lDupO:
		{
			const cpuval val = stack[stackptr];
			stack[++stackptr] = val;
			goto lBack;
		}
	}
}