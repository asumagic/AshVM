# AshVM
AshVM is a simple and portable Virtual Machine written in C++ that interpretes Ash bytecode.
It will be able to load bytecode directly from files (and a simple ashasm parser will come after then).

It requires a C++11 compiler. Because of the label reference based interpretation (for a noticeable speed gain, around 50%, compared to the first version), it may only run on g++ (x86, ARM, PPC should work, but according to the gcc webpage, AVR wouldn't), LLVM clang++ and possibly icc.

clang++ seems to optimize the VM slightly better than g++ (on a i5-4670K, measured to 5~10% on the countdown program)

When no time measuring is specified compile-time (see [ALLOW_TIME_MEASURE](https://github.com/AsuMagic/AshVM/blob/master/vm.cpp#L3)), the only dependencies are <stdint.h>, <memory> (for C++11 smart pointers) and <vector> (these probably will be dropped soon); Else <chrono> also is required.

As for now, the code is moving from a register-based implementation to a variable-based implementation which allows having a way larger set of instructions and an higher-level development (that also will make compiling from the incoming Ash programming language easier). It is usable, but don't except to create more complex programs easily (not because functions are not implemented yet, but also because the only way of storing stuff is the stack (which is very limiting, even having the DUPlicate instruction)).

Using the Virtual Machine :
```c++
int main()
{
	ash::VM virtualMachine; // Create a virtual machine
	virtualMachine.bindProgram(&program[0], getProgramSize(program)); // Binds an array called "program", see next piece of code and gets its size (getProgramSize is a macro to get program's size)
	virtualMachine.prepare(); // Prepare the virtual machine for running
	virtualMachine.run(); // Runs the virtual machine
	return 0;
}
```

Simple program counting from -10'000'000 to 0 (decrementing from 10'000'000 to 0 probably would be possible as well) :
```c++
basetype program[] =
{
	push, static_cast<basetype>(-10000000), // Push -10000000 on the stack
	incr, 0, // Increment the value on the stack
	dupo, 0, // Duplicates the value on the stack once (because jnz is going to destroy one)
	jnz, 1, // Jump to incr (2nd instruction) only when the value taken ontop of the stack is NOT zero
	end, 0, // Ends the program execution (The VM will crash else).
};
```

Note : It is very recommended to compile with -O3, unless debugging, because the compiler optimizations gives a huge performance boost (measured to ~800% on a i5-4670K on the count to 0 program).

It will be possible in the future to :
* Use, modify and create variables from bytecode, of different types
* Request, modify and create values from a C++ program via the ash::VM class
* Create functions within the bytecode, and call them using cdecl
* Call external C functions within the bytecode also using cdecl...
* ... And being able to add some via the ash::VM class
* Interprete bytecode on the fly
* Load bytecode from files
* Reload modified bytecode from a file while the program is running
