# AshVM
AshVM is a simple and portable Virtual Machine written in C++ that interpretes Ash bytecode.
It will be able to load bytecode directly from files (and a simple ashasm parser will come after then).

It requires a C++11 compiler. Because of the label reference based interpretation (for a noticeable speed gain compared to the first version), it may only run on g++ (x86, ARM, PPC should work, but according to the gcc webpage, AVR wouldn't), LLVM clang++ and possibly icc.

When no time measuring is specified compile-time, the only dependencies are stdint, memory (for C++11 smart pointers) and vectors (these probably will be dropped soon); Else chrono also is required.

As for now, the code is moving from a register-based implementation to a variable-based implementation which allows having a way larger set of instructions and an higher-level development (that also will make compiling from the incoming Ash programming language easier). It is usable, but don't except to create more complex programs easily (not because functions are not implemented yet, but also because the only way of storing stuff is the stack (which is very limiting, even having the DUPlicate instruction)).

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
