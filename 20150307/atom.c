#include"atom.h"
#include<stdio.h>
#include<malloc.h>
#include <string.h>

/* will cause memory leak, so we will resolve it with gc*/
Atom cons(Atom car, Atom cdr)
{
	Atom p;
	p.type = ATOM_PAIR;
	p.value.pair = malloc(sizeof(struct Pair));
	car(p) = car;
	cdr(p) = cdr;
	return p;
}

Atom make_integer(int integer)
{
	Atom p;
	p.type = ATOM_INTEGER;
	p.value.integer = integer;
	return p;
}

Atom make_symbol(const char *s)
{
	Atom p;
	p.type = ATOM_SYMBOL;
	p.value.symbol = strdup(s);
	return p;
}

Atom make_function(fn function)
{
	Atom p;
	p.type = ATOM_FUNCTION;
	p.value.function = function;
	return p;
}


int make_closure(Atom env, Atom args, Atom body, Atom *result)
{
	if(!is_list_expr(args) || !is_list_expr(body))
		return ERROR_SYNTAX;
	Atom p;
	/*check all the args are all symbol*/
	p = args;
	while(!nilp(p))
	{
		if(car(p).type != ATOM_SYMBOL)
			return ERROR_TYPE;
		p = cdr(p);
	}

	*result = cons(env, cons(args, body));
	(*result).type = ATOM_CLOSURE;
	return ERROR_OK;
}


void print_expr(Atom p)
{
	switch(p.type)
	{
		case ATOM_NIL:
			printf("NIL");
			break;
		case ATOM_INTEGER:
			printf("%d", p.value.integer);
			break;
		case ATOM_SYMBOL:
			printf("%s", p.value.symbol);
			break;
		case ATOM_FUNCTION:
			printf("%p", p.value.function);
			break;
		case ATOM_PAIR:
			putchar('(');
			print_expr(car(p));
			Atom atom = cdr(p);
			while(!nilp(atom))
			{
				if(atom.type == ATOM_PAIR)
				{
					putchar(' ');
					print_expr(car(atom));
					atom = cdr(atom);
					//print_expr(atom);
				}
				else
				{
					printf(" . ");
					print_expr(atom);
					break;
				}
			}
			putchar(')');
			break;

		default:
			printf("type error\n");
			
	}
}

#if 0
int main()
{
	/* (1)*/
	Atom p = cons(make_integer(1), nil);
	print_expr(p);
	printf("\n");

	/* (1 2) */
	p = cons(make_integer(1), cons(make_integer(2), nil));
	print_expr(p);
	printf("\n");


	/* (1 . 2) */
	p = cons(make_integer(1), make_integer(2));
	print_expr(p);
	printf("\n");
	

	/* ((1 2) . 3) */
	p = cons(cons(make_integer(1), cons(make_integer(2), nil)), make_integer(3));
	print_expr(p);
	printf("\n");

	/* (x . ( 1 2 3)) result need be modified*/        //ERROR
	p = cons(make_symbol("x"), cons(make_integer(1), cons(make_integer(2), cons(make_integer(3), nil))));
	print_expr(p);
	printf("\n");

	/* (1  2 . 3) */
	p = cons(make_integer(1), cons(make_integer(2), make_integer(3)));
	print_expr(p);
	printf("\n");

}

#endif
