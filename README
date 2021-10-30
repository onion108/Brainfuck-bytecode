# Brainfuck Bytecode-Based Runner

## Installation

Execute:
```
make vm
make compiler
```

## Testing

Compile brainfuck source into brainfuck bytecode
```
bin/bfc [Source file] [Output file]
```
e.g.
```
bin/bfc tests/test.bf tests/test.bfc
```

Run a compiled source
```
bin/bfvm [Bytecode]
```

e.g.
```
bin/bfvm tests/test.bfc
```

## Bytecode Format
### Header(4-Bytes)
```
27 26 4A [Version Number(1-byte)]
```

### Content
Commands(Standard Brainfuck Commands - start with `0x1`):<br>
```
18: bf_output(.)
19: bf_input(,)
1A: bf_inc(+)
1B: bf_dec(-)
1C: bf_rshift(>)
1D: bf_lshift(<)
1E: bf_flag([)
1F: bf_jnz(])
```
and I also provides some additional bytecodes that not included in the Standard Brainfuck Commands, they're start with "EBF_" in the source code(src/vm/main.cpp). They are reserved but if you want to compile your own programming language into the brainfuck bytecode, you can use them. Also I MAY provide a compiler with extended brainfuck supporting. If I do so, here are the commands(Please check the source code by yourself to see the bytecode form):
```
? - give a random number to the current pointed memory.
x - Exit the program.
v - Push the value in the current pointed memory to the stack.
^ - Pop the value from stack to the current pointed memory.
@ - Call a function that the position is the content in the current pointed memory.
~ - Return the from the function.
```

