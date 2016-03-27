AshVM instruction list

    VM functionalities
- null : Should not be used - Prints a debugging message (opcode is 0x0000)
- end : Ends the program execution (can print the execution time with -dbg_measure_runtime

    Stack handling
- push : Pushes a hardcoded value to the stack
- pop : Pops the top of the stack and ignores it (1=>0)
- dup : Duplicates the top value from the stack a given amount of times. (1=>x)
- dupo : Duplicates the top value from the stack once. (1=>2)

    Operations
- add : Pops two top values from the stack and adds them (2=>1)
- incr : Increments the top of the stack (1=>1)
- sub : Given a the top and b the second value, returns a - b (2=>1)
- decr : Decrements the top of the stack
- mul : Pops two top values from the stack and multiplies them (2=>1)

    Jumps
- jmp : Jumps to the given pc.
- jz : Pops a value from the stack and if it is zero, jumps to the given pc. (1=>0)
- jnz : Pops a value from the stack and if it is NOT zero, jumps to the given pc. (1=>0)
- jlz : Pops a value from the stack and if it is STRICTLY LOWER than zero, jumps to the given pc. (1=>0)
- jhz : Pops a value from the stack and if it is STRICTLY HIGHER than zero, jumps to the given pc. (1=>0)
- rjmp : Jumps a given amount of instructions.

    Debugging (do not rely on those, they are very subject to change / to be deprecated)
- print : Prints an integer

    Variables
- create : Creates a global variable with a given ID
- store : Pops a value from the stack and stores it to the variable with the given ID (does NOT create it; 1=>0)
- load : Loads a value from the variable with a given ID and pushes it to the stack (0=>1)