# Virgo Compiler

Work in progress

## Grammar

$$
\begin{array}{rcl}
    \langle program \rangle & ::= & \langle stmt-list \rangle \\
    \langle stmt-list \rangle & ::= & \langle stmt \rangle \space | \space \langle stmt-list \rangle \space \langle stmt \rangle \\
    \langle stmt \rangle & ::= & \langle print \rangle \\
    \langle print \rangle & ::= & \text{print}(\langle expr \rangle); \\
    \langle expr \rangle & ::= & \langle term \rangle  \space | \space \langle term \rangle \space + \space \langle expr \rangle\\
    \langle term \rangle & ::= & \space UNSIGNED\_INTEGER
\end{array}
$$