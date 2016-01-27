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
		void* labels[OPTOTAL] = { &&labelNull, &&labelEnd, &&labelPush, &&labelPop, &&labelAdd, &&labelIncr, &&labelSub, &&labelDecr, &&labelMul, &&labelJmp, &&labelJz, &&labelJnz, &&labelRjmp, &&labelPrint, &&labelDup, &&labelDupO };

		const bool noPrint = getFlag(flag_noprint);
		const bool measureTime = getFlag(dbg_measure_runtime);
		const bool printInstructions = getFlag(dbg_list_preprocessed);

#ifdef ALLOW_TIME_MEASURE // TODO : Find a more elegant way so we don't add overhead when not using -dbg_measure_runtime
		using namespace std::chrono;
		auto measureStart = high_resolution_clock::now();
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

		labelBack:
			pc++;
		labelBackNoIncrement:
			const instruction& instr = instructionArray[pc];
			value = instr.value;
			goto *labels[instr.opcode];

		labelNull:
		{
			puts("Encountered null instruction.");
			goto labelBack;
		}

		labelEnd:
#ifdef ALLOW_TIME_MEASURE
			if (measureTime)
			{
				auto timeElapsed = duration<float, std::milli>(high_resolution_clock::now() - measureStart);
				printf("Time elapsed : %fms\n", timeElapsed.count());
			}
#endif
			return;

		labelPush:
			stackPush(value);
			goto labelBack;

		labelPop:
			stackPop();
			goto labelBack;

		labelAdd:
			stackPush(stackPopValue() + stackPopValue());
			goto labelBack;

		labelIncr:
			++stack[stackptr];
			goto labelBack;

		labelSub:
		{
			cpuval b = stackPopValue();
			stackPush(stackPopValue() - b);
			goto labelBack;
		}

		labelDecr:
			--stack[stackptr];
			goto labelBack;

		labelMul:
			stackPush(stackPopValue() * stackPopValue());
			goto labelBack;

		labelJmp:
			pc = value;
			goto labelBackNoIncrement;

		labelJz:
			if (stackPopValue() == 0)
			{
				pc = value;
				goto labelBackNoIncrement;
			}
			goto labelBack;

		labelJnz:
			if (stackPopValue() != 0)
			{
				pc = value;
				goto labelBackNoIncrement;
			}
			goto labelBack;

		labelRjmp:
			pc += value;
			goto labelBack;

		labelPrint:
			printf("%d\n", stackPopValue());
			goto labelBack;

		labelDup:
		{
			const cpuval toRepeat = stack[stackptr];
			const uint target = stackptr + value;
			while (stackptr < target)
			{
				stack[++stackptr] = toRepeat;
			}
			goto labelBack;
		}

		labelDupO:
		{
			const cpuval val = stack[stackptr];
			stack[++stackptr] = val;
			goto labelBack;
		}
	}
}