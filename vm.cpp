#include "vm.hpp"

#include <cstdio>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
// CLion does not like the goto void* sorcellery.

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

	int32_t* VM::resolveRegister(uint8_t reg)
	{
		return &registers[reg];
	}

	void VM::run()
	{
		std::vector<void*> labelArray;
		labelArray.resize(programsize);

		for (uint i = 0; i < programsize; i += 2)
		{
			loinstr low = fetch(i);
			switch(low.opcode)
			{
				case null:  labelArray[i] = &&labelNull; break;
				case end:   labelArray[i] = &&labelEnd; break;
				case mov:   labelArray[i] = &&labelMov; break;
				case push:  labelArray[i] = &&labelPush; break;
				case load:  labelArray[i] = &&labelLoad; break;
				case store: labelArray[i] = &&labelStore; break;
				case add:   labelArray[i] = &&labelAdd; break;
				case sub:   labelArray[i] = &&labelSub; break;
				case mul:   labelArray[i] = &&labelMul; break;
				case jmp:   labelArray[i] = &&labelJmp; break;
				case jz:    labelArray[i] = &&labelJz; break;
				case jnz:   labelArray[i] = &&labelJnz; break;
				case rjmp:  labelArray[i] = &&labelRjmp; break;
				case print: labelArray[i] = &&labelPrint; break;
				case dup:   labelArray[i] = &&labelDup; break;
				default:    printf("Unrecognized instruction : %u", low.opcode); break;
			}

			const instrData tmpInstrData = {
				resolveRegister(low.reg1),
				resolveRegister(low.reg2),
				resolveRegister(low.reg3),
				program[(i + 1)] };

			dataArray[i] = tmpInstrData;
		}

		isRunning = true;

		labelBack:
		{
			pc += 2;
			runData = &dataArray[pc];
			goto *labelArray[pc];
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
			goto labelBack;
		}

		labelPush:
		{

			goto labelBack;
		}

		labelLoad:
		{
			goto labelBack;
		}

		labelStore:
		{
			goto labelBack;
		}

		labelAdd:
		{
			goto labelBack;
		}

		labelSub:
		{
			goto labelBack;
		}

		labelMul:
		{
			goto labelBack;
		}

		labelJmp:
		{
			goto labelBack;
		}

		labelJz:
		{
			goto labelBack;
		}

		labelJnz:
		{
			goto labelBack;
		}

		labelRjmp:
		{
			goto labelBack;
		}

		labelPrint:
		{
			goto labelBack;
		}

		labelDup:
		{
			goto labelBack;
		}
	}
}

#pragma clang diagnostic pop