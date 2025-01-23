 # Virgo Compiler

Work in progress:
**if statement**

## Grammar

$$
\begin{array}{rcl}
    \langle program \rangle & ::= & \langle stmt-list \rangle \\
    \langle stmt-list \rangle & ::= & \epsilon \space | \space \langle stmt \rangle \space  | \space \langle stmt-list \rangle \space \langle stmt \rangle \\
    \langle stmt \rangle & ::= & \langle print \rangle  \space | \space \langle println \rangle \space | \space \langle declaration \rangle \space | \space \langle scope \rangle \space | \space \langle condition \rangle \space | \space \langle comment \rangle\\
    \langle print \rangle & ::= & \texttt{"print("}\langle expr \rangle\texttt{");"} \\
    \langle println \rangle & ::= & \texttt{"println("}\langle expr \rangle\texttt{");"} \\
    \langle declaration \rangle & ::= & \texttt{"let"} \space \langle identifier \rangle \texttt{"="} \space \langle expr \rangle \texttt{";"}   \\
    \langle condition \rangle & ::= & \langle if \rangle \space \langle elif-chain \rangle  \space \langle else \rangle \\
    \langle if \rangle & ::= & \texttt{"if"} \space \langle expr \rangle \space \langle scope \rangle\\
    \langle elif-chain \rangle & ::= & \epsilon \space | 
  \space \langle elif \rangle \langle elif-chain \rangle \\
    \langle elif \rangle & ::= & \texttt{"elif"} \space \langle expr \rangle \space \langle scope \rangle\\
    \langle else \rangle & ::= & \epsilon \space | \space \texttt{"else"} \space \langle scope \rangle\\
   \langle scope \rangle & ::= & \texttt{"\{"} \langle stmt-list \rangle \texttt{"\}"}\\
    \langle expr \rangle & ::= & \langle factor \rangle  \space | \space \langle factor \rangle \space \texttt{"+"} \space \langle expr \rangle \space | \space \langle factor \rangle \space \texttt{"-"} \space \langle expr \rangle \\
    \langle factor \rangle & ::= & \langle term \rangle \space | \space \langle term \rangle \space \texttt{"*"} \space  \langle factor \rangle  \space | \space \langle term \rangle \space \texttt{"/"} \space \langle factor \rangle  \\
    \langle term \rangle & ::= &  \langle identifier \rangle \space | \space \text{unsigned\_integer\_literal} \\
    \langle identifier \rangle & ::= & \langle letter \rangle (\space \langle letter \rangle \space | \space \langle digit \rangle \space)^* \\
    
    \langle comment \rangle & ::= & \texttt{"//"} \langle any-character-except-new-line \rangle  ^* \space | \space \texttt{"/**"} \langle any-character \rangle^* \texttt{"**/"}\\

    \langle letter \rangle & ::= & \texttt{"a"} \space |\space \texttt{"b"} \space |\space \texttt{"c"} \space |\space \texttt{"d"} \space |\space \texttt{"e"} \space |\space \texttt{"f"} \space |\space \texttt{"g"} \space |\space \texttt{"h"} \space |\space \texttt{"i"} \space |\space \texttt{"j"} \space |\space \texttt{"k"} \space |\space \texttt{"l"} \space |\space \texttt{"m"} \space |\space \texttt{"n"} \space |\space \texttt{"o"} \space |\space \texttt{"p"} \space |\space \texttt{"q"} \space |\space \texttt{"r"} \space |\space \texttt{"s"} \space |\space \texttt{"t"} \space |\space \texttt{"u"} \space |\space \texttt{"v"} \space |\space \texttt{"w"} \space |\space \texttt{"x"} \space |\space \texttt{"y"} \space |\space \texttt{"z"}
  \\
  \langle digit \rangle & ::= & \texttt{"0"} \space |\space \texttt{"1"} \space |\space \texttt{"2"} \space |\space \texttt{"3"} \space |\space \texttt{"4"} \space |\space \texttt{"5"} \space |\space \texttt{"6"} \space |\space \texttt{"7"} \space |\space \texttt{"8"} \space |\space \texttt{"9"}
\\
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

2. **Build the project**

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
