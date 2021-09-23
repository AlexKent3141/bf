# bf
Small interpreter for the Brainfuck programming language.

I became interested in Brainfuck while investigating the [Code of the Rings](https://www.codingame.com/multiplayer/optimization/code-of-the-rings) problem on the CodinGame website. A CMake variable can be set to configure the interpreter to match the variant of Brainfuck described in that problem.

## Command line options
```
    -h, --help                show this help message and exit
    -n, --num_cells=<int>     number of cells
    -m, --cell_max=<int>      maximum value of a cell
    -a, --ascii               Assume ASCII encoding for output
```

## Usage
When the interpreter opens, a Brainfuck machine is created with the default state (unless overridden via the command line). You can execute Brainfuck code directly in the interpreter and the machine's state is updated accordingly. At any point the machine's state can be interrogated by entering 'p' or the machine's state can be reset by entering 'r'.

Here's an example session:
```
$ ./bf.exe -n 20 -m 30
#: p
v: ...  0  0  0  0  0  0  0 ...
p: ... 17 18 19  0  1  2  3 ...
                ^

#: +[>+]-

#: p
v: ... 29 29 29 29 29 29 29 ...
p: ... 17 18 19  0  1  2  3 ...
                ^

#: >>>----

#: p
v: ... 29 29 29 25 29 29 29 ...
p: ...  0  1  2  3  4  5  6 ...
                ^

#: r

#: p
v: ...  0  0  0  0  0  0  0 ...
p: ... 17 18 19  0  1  2  3 ...
                ^

#: q
```
