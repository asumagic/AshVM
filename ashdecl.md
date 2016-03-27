AshVM function declarations

AshVM features its own function declaration behavior.

The stack before a function call taking 3 arguments and returning 3 arguments is defined so :

- >> ^ Stack top ^ <<
- Argument 1
- Argument 2
- Argument 3
- >> Stack pointer <<

The stack pointer may be invalid if no function was previously called.

Warning : This means you have to push the latest arguments first.

When ret is called by a function, it **must** cleanup the stack to prevent severe corruption :

- >> ^ Stack top ^ <<
- Argument 1
- Argument 2
- >> Stack pointer <<

When back to the function, the stack will only feature the returned arguments (if there is any).

Functions can use lcreate, lstore and lload to handle local variables. They are only available for one function.

Example with an unoptimized demo recursive fibonacci program :
```
push 5
call rfib # rfib(5)

# Recursive fibonacci program
rfib:
lcreate 0 # Create a local variable with id 0 (n)
lstore 0  # Loads the argument from the stack into the variable

lload 0   # a
push 2    # b
sub       # a - b
jhz 10    # if ((n + 2) > 0)

lload 0
ret       # ret n

lload 0   # a
load 1    # b
sub       # a - b
call rfib  # rfib(n-1)

lload 0   # a
load 2    # b
sub       # a - b
call rfib # rfib(n-2)

add       # rfib(n-1) + rfib(n-2)
ret
```