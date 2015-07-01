
#include"atom.h"
#include"environment.h"
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
	return env_find(parent, symbol, result);
}

int is_list_expr(Atom expr)
{
	Atom p = expr;
	while(!nilp(p))
	{
		if(p.type != ATOM_PAIR)
			return false;
		p = cdr(p);
	}
	return true;	
}




int eval_expr(Atom expr, Atom env, Atom *result)
{
	Error err;
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
		if(!is_list_expr(expr))
			return ERROR_SYNTAX;
		Atom op = car(expr);
		Atom args = cdr(expr);
		/*we can use builtin_quote except "define" "if" , because of others need pass argument env*/
		if(op.type == ATOM_SYMBOL)
		{
			if(strcmp(op.value.symbol, "QUOTE") == 0)
			{
				/*the argments doesn't equal one, will be error*/
				if(nilp(args) || !nilp(cdr(args)))
					return ERROR_ARGS;
				/* here may check the args whether is list expression */    //ERROR
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
				err = eval_expr(car(cdr(args)), env, &val);
				if(err)
					return err;
				/* if the symbol is exist, just update the value, you will see in function env_set*/
				return env_set(env, sym, val);
			}

			else if(strcmp(op.value.symbol, "IF") == 0)
			{
				/*  ( IF test expr-ture expr-false) */
				if(nilp(args) || nilp(cdr(args)) || nilp(cdr(cdr(args))) || !nilp(cdr(cdr(cdr(args)))))
					return ERROR_ARGS;

				Atom expr_true = car(cdr(args));
				Atom expr_false = car(cdr(cdr(args)));
				Atom test = car(args);
				Atom cond;

				err = eval_expr(test, env, &cond);
				if(err)
					return err;

				if(nilp(cond))
					return eval_expr(expr_false, env, result);
				else
					return eval_expr(expr_true, env, result);
			}

			else if(strcmp(op.value.symbol, "LAMBDA") == 0)
			{
				return make_closure(env, car(args), cdr(args), result);
			}
		}


		/* just like the operator '+', we need to get the builtin_add function */
		err = eval_expr(op, env, &op);
		if(err)
			return err;
		/* evaluate arguments*/
//		args = copy_list(args);	
		Atom p = args;
	//	if(op.value.function != builtin_car && op.value.function != builtin_cdr)
		{
			while(!nilp(p))
			{
				/*this is why (+ x 2) can be true when x is bounded with integer. 
 					the statement below calculate the value of x */
				Error err = eval_expr(car(p), env, &car(p));
				if(err)
					return err;
				p = cdr(p);
			}
		}

		//print_expr(args);
		return apply(op, args, result);
	}
}


int apply(Atom func, Atom args, Atom *result)
{
	if(func.type == ATOM_FUNCTION)
	{
		return (*func.value.function)(args, result);
	}
	else if(func.type == ATOM_CLOSURE)
	{
		
		/* modify */
		//Atom env = car(func);
		Atom env = env_create(car(func));
		Atom args_name = car(cdr(func));
		Atom body = cdr(cdr(func));

		/**bind the arguments */
		Atom p = args_name;
		Atom q = args;
		while(!nilp(p))
		{
			if(nilp(q))
				return ERROR_ARGS;
			env_set(env, car(p), car(q));
			p = cdr(p);
			q = cdr(q);
		}
		if(!nilp(q))
			return ERROR_ARGS;
		//print_expr(args_name);
		//print_expr(args);
		//print_expr(body);

		/* evaluate the body*/
		//if(!nilp(body))
		//{
		Error err = eval_expr(car(body), env, result);
		if(err)
			return err;
		//body = cdr(body);
		//}

	}
}

int builtin_car(Atom args, Atom *result)
{
	if(nilp(args) || !nilp(cdr(args)))
		return ERROR_ARGS;
	else if(nilp(car(args)))
		*result = nil;
	else if(car(args).type != ATOM_PAIR)  /* car only can operate on nil and list expression*/
		return ERROR_TYPE;
	else
		*result = car(car(args));  /* should there check whether the args is list expression */
	return ERROR_OK;

}

int builtin_cdr(Atom args, Atom *result)
{

	if(nilp(args) || !nilp(cdr(args)))
		return ERROR_ARGS;
	else if(nilp(car(args)))
		*result = nil;
	else if(car(args).type != ATOM_PAIR)  /* cdr only can operate on nil and list expression*/
		return ERROR_TYPE;
	else
		*result = cdr(car(args));
	return ERROR_OK;
}

int builtin_cons(Atom args, Atom *result)
{

	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;
	*result = cons(car(args),car(cdr(args)));
	return ERROR_OK;
}



int builtin_add(Atom args, Atom *result)
{

	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));

	if(a.type != ATOM_INTEGER || b.type != ATOM_INTEGER)
		return ERROR_TYPE;
	*result = make_integer(a.value.integer + b.value.integer);
	return ERROR_OK;
}


int builtin_sub(Atom args, Atom *result)
{

	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));

	if(a.type != ATOM_INTEGER || b.type != ATOM_INTEGER)
		return ERROR_TYPE;
	*result = make_integer(a.value.integer - b.value.integer);
	return ERROR_OK;
}

int builtin_mul(Atom args, Atom *result)
{

	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));

	if(a.type != ATOM_INTEGER || b.type != ATOM_INTEGER)
		return ERROR_TYPE;
	*result = make_integer(a.value.integer * b.value.integer);
	return ERROR_OK;
}



int builtin_div(Atom args, Atom *result)
{

	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));

	if(a.type != ATOM_INTEGER || b.type != ATOM_INTEGER)
		return ERROR_TYPE;
	*result = make_integer(a.value.integer / b.value.integer);
	return ERROR_OK;
}


int builtin_equal(Atom args, Atom *result)
{
	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));

	if(a.type != ATOM_INTEGER || b.type != ATOM_INTEGER)
		return ERROR_TYPE;
	*result = (a.value.integer == b.value.integer) ? make_symbol("T") : nil;
	
	return ERROR_OK;
}


int builtin_less(Atom args, Atom *result)
{
	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));

	if(a.type != ATOM_INTEGER || b.type != ATOM_INTEGER)
		return ERROR_TYPE;
	*result = (a.value.integer < b.value.integer) ? make_symbol("T") : nil;
	return ERROR_OK;
}


int builtin_greater(Atom args, Atom *result)
{
	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));

	if(a.type != ATOM_INTEGER || b.type != ATOM_INTEGER)
		return ERROR_TYPE;
	*result = (a.value.integer > b.value.integer) ? make_symbol("T") : nil;
	return ERROR_OK;
}


int builtin_and(Atom args, Atom *result)
{

	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));
	Atom t = make_symbol("T");
//	if(!nilp(a) || !nilp(b))
//		return ERROR_TYPE;
	if(nilp(a))
		*result = nil;
	else 
	{
		if(nilp(b))
			*result = nil;
		else
			*result = t;
	}
	return ERROR_OK;
}

int builtin_or(Atom args, Atom *result)
{

	if(nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args))))
		return ERROR_ARGS;

	Atom a = car(args);
	Atom b = car(cdr(args));
	Atom t = make_symbol("T");
	if(nilp(a) && nilp(b))
		*result = nil;
	else
	{		
		*result = t;
	}
	return ERROR_OK;
}

int builtin_not(Atom args, Atom *result)
{
	if(nilp(args) || !nilp(cdr(args)))
		return ERROR_ARGS;
	Atom a = car(args);
	Atom t = make_symbol("T");
	if(nilp(a))
		*result = t;
	else
	{		
		*result = nil;
	}
	return ERROR_OK;

}

#if 0
int main()
{

	Atom env = env_create(nil);
	Atom result;
	Atom expr;
	/* (car (1 2 3)) */
	/* error in 86 lines*/
	expr = cons(make_function(builtin_cdr), cons(cons(make_integer(1), cons(make_integer(2), cons(make_integer(3), nil))), nil));
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* (car (1 2))*/
	expr = cons(make_function(builtin_car), cons(cons(make_integer(1), cons(make_integer(2), nil)), nil));
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* (+ 1 2)*/
	expr = cons(make_function(builtin_add), cons(make_integer(1), cons(make_integer(2), nil)));
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* (+ (- 1 2) 3)*/
	expr = cons(make_function(builtin_add), cons(cons(make_function(builtin_sub), cons(make_integer(1), cons(make_integer(2), nil))), cons(make_integer(3), nil)));
	print_expr(expr);
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");
}


#endif

#if 0

int main()
{
	Atom expr = cons(make_integer(1), make_integer(2));
	/* lisp is all about expression (s-expression), a expression can be a literal, symbol and
	   list consisting of an operator and one or more arguments. list expression reffers to list */
	if(!is_list_expr(expr))
	{
		print_expr(expr);
		printf(" is not list expression\n");
	}

	/** (x . (1 2 3)) */
	Atom p = cons(make_symbol("x"), cons(make_integer(1), cons(make_integer(2), cons(make_integer(3), nil))));
	print_expr(p);
	printf("\n");

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
	expr = make_symbol("hy");
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* (DEFINE foo 1 . 2) <=> (DEFINE foo (1 . 2))  (1 .2)is not list expression , just a pair*/
	expr = cons(make_symbol("DEFINE"), cons(make_symbol("foo"), cons(cons(make_integer(1), make_integer(2)), nil)));
	print_expr(expr);
	Error err = eval_expr(expr, env, &result);
	if(err == ERROR_SYNTAX)
		printf("(1 . 2) is not expression");


	/* (quote (1 2 3)) */
	expr = cons(make_symbol("QUOTE"), cons(cons(make_integer(1), cons(make_integer(2), cons(make_integer(3), nil))), nil));
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* (define foo 1) */
	expr = cons(make_symbol("DEFINE"), cons(make_symbol("foo"), cons(make_integer(100), nil)));
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");


	env_find(env, make_symbol("foo"), &result); 
	print_expr(result);
	printf("\n");

	/* (define foo (quote bar))*/
	expr = cons(make_symbol("DEFINE"), cons(make_symbol("foo"), cons(cons(make_symbol("QUOTE"), cons(make_symbol("bar"), nil)), nil)));
	print_expr(expr);
	printf("\n");
	eval_expr(expr, env, &result);
	print_expr(result);
	printf("\n");

	/* now foo's value is modified */
	env_find(env, make_symbol("foo"), &result);
	print_expr(result);
	printf("\n");

	print_expr(env);
	printf("\n");
}

#endif
