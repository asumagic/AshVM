AshVM function declarations

AshVM features its own function declaration behavior.

The stack before a function call taking 3 arguments is defined so :

- >> ^ Stack top ^ <<
- Argument 1
- Argument 2
- Argument 3
- The PC next after the function call
- The previous function's stack pointer
- >> Stack pointer <<

The stack pointer may be invalid if no function was previously called.

Precall generates the two lower instructions before calling up the function.

When ret is called by a function, it **must** cleanup the stack to prevent severe corruption :

- >> ^ Stack top ^ <<
- The PC next after the function call
- The previous function's stack pointer
- Returned arguments
- >> Stack pointer <<

When back to the function, the stack will only feature the returned arguments (if there is any).

Functions can use lcreate, lstore and lload to handle local variables. They are only available for one function.

Example with an unoptimized demo recursive fibonacci program :
```
0. precall # Prepares the function call by putting the 2 lower instructions (null/pc, null/sp) onto the stack
1. push 5
2. call rfib # rfib(5)

# Recursive fibonacci program
rfib:
3. lcreate 0 # Create a local variable with id 0 (n)
4. lstore 0  # Loads the argument from the stack into the variable

5. lload 0   # a
6. push 2    # b
7. sub       # a - b
8. jhz 11    # if ((n + 2) > 0)

9. lload 0
10. ret      # ret n

11. precall  # prepare rfib(n-1)
12. lload 0  # a
13. load 1   # b
14. sub      # a - b
15. call rfib # rfib(n-1)

16. precall  # prepare rfib(n-2)
17. lload 0  # a
18. load 2   # b
19. sub      # a - b
20. call rfib # rfib(n-2)

21. add      # rfib(n-1) + rfib(n-2)
22. ret
```