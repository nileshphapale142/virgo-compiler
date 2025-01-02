# Virgo Compiler

Work in progress

## Grammar

$$
\begin{array}{rcl}
    \langle program \rangle & ::= & \langle stmt-list \rangle \\
    \langle stmt-list \rangle & ::= & \langle stmt \rangle \space | \space \langle stmt-list \rangle \space \langle stmt \rangle \\
    \langle stmt \rangle & ::= & \langle print \rangle \\
    \langle print \rangle & ::= & \text{print}(UNSIGNED\_INT); 
\end{array}
$$