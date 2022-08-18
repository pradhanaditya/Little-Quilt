Use the following sequence of commands:

lex a3.l
yacc -b parse -dv a3.y
g++ lex.yy.c parse.tab.c quilt.cpp exp.cpp quadruple.cpp -o compiler -ll -ly
./compiler




Changelog:

v5.0
--------------------------------------------------------------------------------------------------
- Removed the unnecessary 'id' attribute and it's initialization from the derived ASTs constructors.
- The Abstract Syntax Tree drawn in the output behaves exactly as per the reference implementation.
- Incorrect declaration fix (in the generated CPP Code) for the input "a" or "b".
- Some minor code optimizations.

