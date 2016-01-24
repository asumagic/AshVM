#ifndef VM_HPP_INCLUDED
#define VM_HPP_INCLUDED

#include <stdint.h>
#include <vector>

#define getProgramSize(program) sizeof(program) / (sizeof(uint32_t) * 2)

namespace ash
{
	typedef uint32_t basetype;
	typedef int32_t  cpuval;
	typedef unsigned int uint;

	enum instructions
	{
		null = 0, end, mov, push, pop, load, store, add, incr, sub, decr, mul, jmp, jz, jnz, rjmp, print, dup, dupo, OPTOTAL
	};

	enum commonRegisters
	{
		ax = 1, bx, cx, dx, ex
	};

	struct instruction
	{
		basetype low, high;
	};

	struct loinstr
	{
		uint8_t opcode, reg1, reg2, reg3;
	};

	struct instrData
	{
		uint8_t opcode;
		cpuval *reg1, *reg2, *reg3;
		uint32_t value;
	};

	constexpr basetype instrbase(instructions opcode, uint8_t reg1 = 0, uint8_t reg2 = 0, uint8_t reg3 = 0)
	{
		return (opcode << 24) | (reg1 << 16) | (reg2 << 8) << reg3;
	}

	class VM
	{
	public:
		VM();
		~VM();

		enum vmflags
		{
			op_drop_unused_calls,
			op_drop_unused_registers,
			op_morph_dup_dupo,
			flag_noprint
		};

		void setFlag(vmflags flag, bool value = true);
		bool getFlag(vmflags flag) const
		{
			return static_cast<bool>((flags & (1 << flag)) >> flag);
		}

		void bindProgram(basetype* program, uint _programsize);

		void setRegisterCount(uint8_t count);
		uint8_t getRegisterCount() const { return registerCount; }

		void setStackSize(uint32_t size);
		uint32_t getStackSize() const { return stackSize; }

		loinstr fetch(const basetype loc);

		void prepare();
		void run();

	private:
		int32_t* resolveRegister(uint8_t reg);

		inline void stackPush(cpuval val);
		inline cpuval stackPopValue();
		inline void stackPop();

		std::vector<instrData> dataArray;

		basetype* program;
		instrData* runData;
		uint programsize;

		cpuval* registers;
		cpuval* stack;

		uint pc = -1;
		uint stackptr = 1;

		bool hasInitialized = false, isRunning = false;

		uint8_t registerCount = 5;
		uint32_t stackSize = 512;
		uint32_t flags;
	};
}

#endif
