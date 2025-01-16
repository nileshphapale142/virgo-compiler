# Virgo Compiler

Work in progress

## Grammar

$$
\begin{array}{rcl}
    \langle program \rangle & ::= & \langle stmt-list \rangle \\
    \langle stmt-list \rangle & ::= & \langle stmt \rangle \space | \space \langle comment \rangle \space | \space \langle stmt-list \rangle \space \langle stmt \rangle  \space | \space \langle stmt-list \rangle \space \langle comment \rangle\\
    \langle stmt \rangle & ::= & \langle print \rangle  \space | \space \langle println \rangle \\
    \langle print \rangle & ::= & \text{print}(\langle expr \rangle); \\
    \langle println \rangle & ::= & \text{println}(\langle expr \rangle); \\
    \langle expr \rangle & ::= & \langle factor \rangle  \space | \space \langle factor \rangle \space + \space \langle expr \rangle \space | \space \langle factor \rangle \space - \space \langle expr \rangle \\
    \langle factor \rangle & ::= & \langle term \rangle \space | \space \langle term \rangle \space * \space  \langle factor \rangle  \space | \space \langle term \rangle \space / \space \langle factor \rangle  \\
    \langle commet \rangle & ::= & // \langle any-character \rangle  ^*\\
    \langle term \rangle & ::= & \space UNSIGNED\_INTEGER\_LITERAL
\end{array}
$$


## How to use?

### Prerequisites
- Cmake

### Build the compiler

1. **Configure the build system** 
```bash
cmake -S . -B build
```

2. **Build  the project**
```bash
cmake --build build
```

### Compilation and Execution Process

Follow these steps to compile and execute your program:

1. **Create a `.vi` file**
   - Write your code in a `.vi` file, ensuring it adheres to the language syntax and grammar.

2. **Compile the program**
   - Compile the program by running the following command:
     ```bash
     <path-to-virgo-executable> <filename.vi>
     ```

    If you're working from the project root directory, the path to the Virgo executable will be:
    ```bash
    build/virgo <filename.vi>
    ```
 

3. **Execute the compiled program**
   - Once compiled, execute the program using:
     ```bash
     ./filename
     ```

