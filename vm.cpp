#include "vm.hpp"

#include <cstdio>

namespace ash
{
	VM::VM() {}

	VM::~VM()
	{
		if (hasInitialized)
		{
			delete[] registers;
			delete[] stack;
		}
	}

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
		dataArray.resize(programsize);

		registers = new cpuval[registerCount];
		stack = new cpuval[stackSize];

		hasInitialized = true;
	}

	loinstr VM::fetch(const basetype loc)
	{
		const basetype half = program[loc];
		return loinstr{ static_cast<uint8_t>(half >> 24),
						 static_cast<uint8_t>((half & 0x00FF0000) >> 16),
						 static_cast<uint8_t>((half & 0x0000FF00) >> 8),
						 static_cast<uint8_t>((half & 0x000000FF)) };
	}

	void VM::setRegisterCount(uint8_t count)
	{
		registerCount = count;
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

	int32_t* VM::resolveRegister(uint8_t reg)
	{
		if (reg == 0)
			return NULL;
		else
			return &registers[reg - 1];
	}

	void VM::run()
	{
		void* labels[OPTOTAL] = { &&labelNull, &&labelEnd, &&labelMov, &&labelPush, &&labelPop, &&labelLoad, &&labelStore, &&labelAdd, &&labelIncr, &&labelSub, &&labelDecr, &&labelMul, &&labelJmp, &&labelJz, &&labelJnz, &&labelRjmp, &&labelPrint, &&labelDup, &&labelDupO };

		bool noprint = getFlag(flag_noprint);
		bool opdup = getFlag(op_morph_dup_dupo);

		for (uint i = 0; i < programsize; i += 1)
		{
			const loinstr low = fetch(i * 2);
			const uint32_t value = program[((i * 2) + 1)];

			instrData idat = {
				low.opcode,
				resolveRegister(low.reg1),
				resolveRegister(low.reg2),
				resolveRegister(low.reg3),
			value };

			if (noprint && idat.opcode == print)
				idat.opcode = pop;

			if (opdup && idat.opcode == dup && idat.value == 1)
				idat.opcode = dupo;

			dataArray[i] = idat;
		}

		isRunning = true;

		labelBack:
		{
			pc++;
		labelBackNoIncrement:
			runData = &dataArray[pc];
			goto *labels[runData->opcode];

		}

		labelNull:
		{
			puts("Encountered null instruction.");
			goto labelBack;
		}

		labelEnd:
		{
			return;
		}

		labelMov:
		{
			*runData->reg1 = runData->reg2 ? *runData->reg2 : runData->value;
			goto labelBack;
		}

		labelPush:
		{
			stackPush(runData->value);
			goto labelBack;
		}

		labelPop:
		{
			stackPop();
			goto labelBack;
		}

		labelLoad:
		{
			stackPush(*runData->reg1);
			goto labelBack;
		}

		labelStore:
		{
			*runData->reg1 = stackPopValue();
			goto labelBack;
		}

		labelAdd:
		{
			stackPush(stackPopValue() + stackPopValue());
			goto labelBack;
		}

		labelIncr:
		{
			++stack[stackptr];
			goto labelBack;
		}

		labelSub:
		{
			cpuval b = stackPopValue();
			stackPush(stackPopValue() - b);
			goto labelBack;
		}

		labelDecr:
		{
			--stack[stackptr];
			goto labelBack;
		}

		labelMul:
		{
			stackPush(stackPopValue() * stackPopValue());
			goto labelBack;
		}

		labelJmp:
		{
			pc = runData->value;
			goto labelBackNoIncrement;
		}

		labelJz:
		{
			if (stackPopValue() == 0)
			{
				pc = runData->value;
				goto labelBackNoIncrement;
			}
			goto labelBack;
		}

		labelJnz:
		{
			if (stackPopValue() != 0)
			{
				pc = runData->value;
				goto labelBackNoIncrement;
			}
			goto labelBack;
		}

		labelRjmp:
		{
			pc += runData->value;
			goto labelBack;
		}

		labelPrint:
		{
			printf("%d\n", stackPopValue());
			goto labelBack;
		}

		labelDup:
		{
			const cpuval toRepeat = stack[stackptr];
			const uint target = stackptr + runData->value;
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