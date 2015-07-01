#ifndef __ENVIROMENT_H
#define __ENVIROMENT_H
/* enviroment */
Atom env_create(Atom parent);
int env_set(Atom env, Atom symbol, Atom value);
int env_find(Atom env, Atom symbol, Atom *result);

int is_list_expr(Atom expr);
int eval_expr(Atom expr, Atom env, Atom *result);
int apply(Atom op, Atom args, Atom *result);

/*  builtin function*/
int builtin_car(Atom args, Atom *result);
int builtin_cdr(Atom args, Atom *result);
int builtin_cons(Atom args, Atom *result);

/*  Arithmetic */
int builtin_add(Atom args, Atom *result);
int builtin_sub(Atom args, Atom *result);
int builtin_mul(Atom args, Atom *result);
int builtin_div(Atom args, Atom *result);

/*  boolean and short-circuit evaluation*/
int builtin_equal(Atom args, Atom *result);
int builtin_less(Atom args, Atom *result);
int builtin_greater(Atom args, Atom *result);

int builtin_and(Atom args, Atom *result);
int builtin_or(Atom args, Atom *result);
int builtin_not(Atom args, Atom *result);
#endif
