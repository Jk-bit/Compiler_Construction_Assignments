lex scientific_calculator.l
yacc -d scientific_calculator.y
gcc lex.yy.c y.tab.c -o scientific_calculator
