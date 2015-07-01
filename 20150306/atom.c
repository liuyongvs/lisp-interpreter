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


void print_expr(Atom p)
{
	Atom atom;
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
		case ATOM_PAIR:
			putchar('(');
			print_expr(car(p));
			atom = cdr(p);
			while(!nilp(atom))
			{
				if(atom.type == ATOM_PAIR)
				{
					putchar(' ');
					print_expr(car(atom));
					atom = cdr(atom);
				}
				else
				{
					printf(" . ");
					print_expr(atom);
					break;
				}
			}
			//		print_expr(atom);
			putchar(')');
			break;

	}
}

