# AshVM instruction list

This table groups all available AshVM instructions.

They give the matching AshASM name, the full name, a short description, a stack prestate and poststate (warning : those does not count possible other modifications on the stack as sget, sset would do), the stack value order (i.e. what the top of the stack and the second value of the stack matches to) and what the value is used for.

| AshASM  | Full name            | Description                                                                     | Stack prestate | Stack poststate | Value          | Pre stack value order |
|---      |---                   |---                                                                              |---             |---              |---             |---                    |
| null    | Null                 | Shouldn't be used - Prints a debugging message                                  | 0              | 0               | Unused         |                       |
| end     | Program end          | Stops the program execution.                                                    | 0              | N/A             | Unused         |                       |
| push    | Stack push           | Pushes a hardcoded value to the stack top.                                      | 0              | 1               | Pushed value   |                       |
| pop     | Stack pop            | Pops the value from the stack top.                                              | 1              | 0               | Unused         | Popped value          |
| sget    | Stack get            | Sets a value from the stack at a hardcoded position                             | 0              | 1               | Stack position |                       |
| sset    | Stack set            | Sets a value from the stack at a hardcoded position                             | 1              | 0               | Stack position | Value                 |
| sgetrel | Stack get            | Gets a value from the stack at a given position                                 | 2              | 0               | Unused         | Pos                   |
| ssetrel | Stack set            | Sets a value from the stack at a given position                                 | 2              | 0               | Unused         | Pos > Value           |
| dup     | Stack duplicate      | Duplicates a value from the stack a hardcoded amount of times.                  | 1              | >= 1            | ° of dup       | Value to duplicate    |
| dupo    | Stack duplicate once | Duplicates a value from the stack once.                                         | 1              | 2               | Unused         | Value to dup once     |
| add     | Add                  | Adds the two top values from the stack and pushes the result.                   | 2              | 1               | Unused         | Value > Value         |
| incr    | Increment            | Increments the top value of the stack.                                          | 1              | 1               | Unused         | Value to increment    |
| sub     | Substract            | Substracts the two top values from the stack so a - b is pushed.                | 2              | 1               | Unused         | b > a                 |
| decr    | Decrement            | Decrements the top value of the stack.                                          | 1              | 1               | Unused         | Value to decrement    |
| mul     | Multiply             | Multiply the two top value from the stack.                                      | 2              | 1               | Unused         | Value to multiply     |
| jmp     | Jumps                | Jumps to a hardcoded pc.                                                        | 0              | 0               | Final PC       |                       |
| jz      | Jumps if zero        | Jumps to a hardcoded pc if the value popped from the stack is zero.             | 1              | 0               | Final PC       | Tested =0             |
| jnz     | Jumps if not zero    | Jumps to a hardcoded pc if the value popped from the stack is not zero.         | 1              | 0               | Final PC       | Tested !=0            |
| jlz     | Jumps if < zero      | Jumps to a hardcoded pc if the value popped from the stack is lower than zero.  | 1              | 0               | Final PC       | Tested <0             |
| jhz     | Jumps if > zero      | Jumps to a hardcoded pc if the value popped from the stack is higher than zero. | 1              | 0               | Final PC       | Tested >0             |
| rjmp    | Relative jump        | Jumps a hardcoded amount of instructions.                                       | 0              | 0               | Jump count     |                       |
| print   | Integer print        | [Debug] Prints an integer popped from the stack.                                | 1              | 0               | Unused         | To print              |
| create  | Create variable      | Creates a global variable with a hardcoded ID.                                  | 0              | 0               | Variable ID    |                       |
| store   | Store to variable    | Stores a value popped from the stack to the variable with the matching ID.      | 1              | 0               | Variable ID    | To store              |
| load    | Load from variable   | Loads a value of the variable with the matching ID and pushes it to the stack.  | 0              | 1               |