# lisp-interpreter
implement a lisp interpreter with low level language in C.
1. design the data struct to represent the atom type, 
    ATOM_NIL,
		ATOM_SYMBOL,
		ATOM_INTEGER,
		ATOM_PAIR,
		ATOM_FUNCTION,
		ATOM_CLOSURE

2. lexer and parser

3. support basic functional programming operate such as car cdr cons nil 

4. extend to arithmetic (+, - , * ,/)

5. support boolean and if(which is a short-circuit evaluation), such as(< 1 2) (= (+ 1 2) (- 21 12))

6. expand to lambda expressions and closures(lambda (x) (* x x)). and simple syntatic sugar(such as ('(1 2)), which is a list)

7. use the mark sweep algorithm proposed by John McCarthy to implement a simle garbage collector .

more works deserve to be done, and i will do it later
1. support other type, such as float, big number, complex and so on

2. support variadic functions, such as(+ 1 2 3 4)

3. tail recursive optimization

4. environment use the hash table to speed up

5. alternative GC

