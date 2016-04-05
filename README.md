# AshVM

## Description

AshVM is a simple to use, fast and portable 32-bit Virtual Machine written in C++ that interpretes Ash bytecode.
It is abled to load bytecode from files (note that this feature is disabled for now).

It requires a C++11 compiler. Because of the label reference based interpretation (for a noticeable speed gain, around 50%, compared to the first version), it may only run on g++ (x86, ARM, PPC should work, but according to the gcc webpage, AVR wouldn't), LLVM clang++ and possibly icc.

It is now built and tested with gcc6 which provides better optimization than clang for now.

When no time measuring is specified compile-time (see [ALLOW_TIME_MEASURE](https://github.com/AsuMagic/AshVM/blob/master/vm.cpp#L3)), the only dependencies are stdint, memory (for C++11 smart pointers, and that's likely not going to change) and vector (this probably will be dropped soon); Else chrono also is required.

The test fibonacci program runs around 3 times faster than a good Lua implementation (without functions) for the same input. In-depth benchmarks will be done with other bytecode virtual machines (angelscript, lua, squirrel?) when AshVM will be in a stable state.

## Usage

Loading a program with the VM is easy and quick :
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

Simple program counting from 10'000'000 to 0 (decrementing from 10'000'000 to 0 probably would be possible as well) :
```c++
basetype program[] =
{
	push, 10000000, // Push 10000000 on the stack
	decr, 0, // Increment the value on the stack
	dupo, 0, // Duplicates the value on the stack once (because jnz is going to destroy one)
	jnz, 1, // Jump to incr (2nd instruction) only when the value taken ontop of the stack is NOT zero
	end, 0, // Ends the program execution (The VM will crash else).
};
```

Note : It is very recommended to compile with -O3, unless debugging, because the compiler optimizations gives a huge performance boost (measured to ~800% on a i5-4670K on the count to 0 program).

`-march=native -mtune=native` possibly gives good performance enhancement on runtime and on preparation.

## Building (Linux)

Install the minimal requirements to build a C++ program plus cmake and GNU make, which can be done so using Debian/Ubuntu/Mint :

```
sudo apt-get install build-essentials cmake
```

Download the project as a .zip from github or fetch directly the files from git.

```cd /path/to/the/root/
mkdir build
cd build
cmake ..
make
```

## TODO list

It will be possible in the future to :
* Request, modify and create variables, handle the stack, debug from a C++ program via the ash::VM class
* Create functions within the bytecode, and call them using cdecl
* Call external C functions within the bytecode also using cdecl...
* ... And being able to bind them via the ash::VM class
* Interprete bytecode on the fly
* Reload modified bytecode from a file while the program is running
