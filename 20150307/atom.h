#ifndef __ATOM_H
#define __ATOM_H

#include"type.h"
Atom cons(Atom car, Atom cdr);
Atom make_symbol(const char *s);
Atom make_integer(int integer);
Atom make_integer(int integer);
Atom make_function(fn function);
int make_closure(Atom env, Atom args, Atom body, Atom *result);
void print_expr(Atom p);

#endif
