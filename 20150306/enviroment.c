#include"type.h"
#include"atom.h"
#include<stdio.h>

/* create enviroment with lisp data struct, because the bindings can be symbol, integer or lisp itself
 * so we can't use hashtable to replace it
 */
Atom env_create(Atom parent)
{
	return cons(parent, nil);
}

int env_set(Atom env, Atom symbol, Atom value)
{
	Atom p = cdr(env);
	Atom rear= env;
	while(!nilp(p))
	{
		Atom first = car(p);
		if(strcmp(car(first).value.symbol, symbol.value.symbol) == 0)
		{
			cdr(first) = value;
			return ERROR_OK;
		}
		rear = p;
		p = cdr(p);
	}

	Atom pair = cons(symbol, value);
	/* the same as the list inserting from tail, the book is the same as list inserting from head*/
	cdr(rear) = cons(pair, nil);
	return ERROR_OK;
}

int env_find(Atom env, Atom symbol, Atom *result)
{
	Atom parent = car(env);
	Atom p = cdr(env);
	while(!nilp(p))
	{
		Atom first = car(p);
		if(strcmp(car(first).value.symbol, symbol.value.symbol) == 0)
		{
			*result = cdr(first);
			return ERROR_OK;
		}
		p = cdr(p);
	}

	/* have n't understand page 20*/
	/* this means the symbol doesn't exist*/
	if(nilp(parent))
		return ERROR_UNBOUND;
}

int eval_expr(Atom expr, Atom env, Atom *result)
{
	if(expr.type == ATOM_SYMBOL)
		return env_find(env, expr, result);
	else if(expr.type == ATOM_INTEGER)
	{
		*result = expr;
		return ERROR_OK;
	}
	else if(expr.type == ATOM_NIL)
	{
		*result = nil;
		return ERROR_OK;
	}
	else
	{
		/* expr's type is ATOM_PAIR */
		Atom op = car(expr);
		Atom args = cdr(expr);

		if(strcmp(op.value.symbol, "QUOTE") == 0)
		{
			/*the argments doesn't equal one, will be error*/
			if(nilp(args) || !nilp(cdr(args)))
				return ERROR_ARGS;
			*result = car(args);
			return ERROR_OK;
		}
		else if(strcmp(op.value.symbol, "DEFINE") == 0)
		{
			if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
				return ERROR_ARGS;
			Atom sym = car(args);
			if(sym.type != ATOM_SYMBOL)
				return ERROR_TYPE;
			*result = sym;

			/* add the new symbol(variable) to the enviroment*/
			Atom val;
			Error err = eval_expr(car(cdr(args)), env, &val);
			if(err)
				return err;
			/* if the symbol is exist, just update the value, you will see in function env_set*/
			return env_set(env, sym, val);
		}
	}
}
int main()
{
	Atom env = env_create(nil);
	/*test (parent (foo . 1) (bar . hello world) (baz (+ 1 2)))*/
	env_set(env, make_symbol("foo"), make_integer(1));
	env_set(env, make_symbol("bar"), make_symbol("hello world"));
	env_set(env, make_symbol("baz"), cons(make_symbol("+"), cons(make_integer(1), cons(make_integer(2), nil))));

	Atom result;
	env_find(env, make_symbol("foo"), &result);
	print_expr(result);
	printf("\n");

	env_find(env, make_symbol("bar"), &result);
	print_expr(result);
	printf("\n");

	env_find(env, make_symbol("baz"), &result);
	print_expr(result);
	printf("\n");

	/* atom : hy & ly, will be error*/
	Atom expr = make_symbol("hy");
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* (quote (x . y)) may be error because (x . y) is list ?*/ 
	expr = cons(make_symbol("QUOTE"), cons(cons(make_symbol("x"), make_symbol("y")), nil));
	print_expr(expr);
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* (define foo 1) */
	expr = cons(make_symbol("DEFINE"), cons(make_symbol("foo"), cons(make_integer(1), nil)));
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* (define foo (quote bar))*/
	expr = cons(make_symbol("DEFINE"), cons(make_symbol("foo"), cons(cons(make_symbol("QUOTE"), cons(make_symbol("bar"), nil)), nil)));
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* now foo's value is modified, there is not true*/
	env_find(env, make_symbol("foo"), &result);
	print_expr(result);
	printf("\n");

	print_expr(env);
	printf("\n");
}
