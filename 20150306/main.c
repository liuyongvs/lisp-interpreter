#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include"type.h"



int read_expr(const char *input, char **end,  Atom *result);
int read_list(const char *start, char **end, Atom *result);
int parse_simple(const char *start, char *end, Atom *result);

int lex(char *str, char **start, char **end);

void convertToUpperCase(char *str);



int read_expr(const char *input, char **end,  Atom *result)
{
	const char *token;
	Error err = lex(input, &token, end);
	if(err)
		return err;

	if(token[0] == '(')
		return read_list(*end, end, result);
//	else if(token[0] == ')')
//		return ERROR_SYNTAX;
	else
		return parse_simple(token, *end, result);
}


int read_list(const char *start, char **end, Atom *result)
{
	Error err;
	char *token;
	Atom p = nil;
	*result = nil;

	*end = start; //
	for(; ;)
	{
		
		err = lex(*end, &token, end);
		printf("%s\n", token);
		//err = lex(start, &token, end);
		if(err)
			return ERROR_SYNTAX;
		if(token[0] == ')')
			return ERROR_OK;

		Atom item;
		if(token[0] == '.' && *end - token == 1)
		{
			if(nilp(p))
				return ERROR_SYNTAX;
			err = read_expr(*end, end, &item);
			if(err)
				return err;
			cdr(p) = item;
			
			err = lex(*end, &token, end);
			/* when there is '.', must be the end*/
			if(token[0] != ')')
				return ERROR_SYNTAX;
			return ERROR_OK;
		}

		err = read_expr(token, end, &item);
		//err = read_expr(*end, end, &item);
		if(err)
			return ERROR_SYNTAX;
		if(nilp(p))
		{
			/* first item */
			*result = cons(item, nil);
			p = *result;
		}
		else
		{
			cdr(p) = cons(item, nil);
			p = cdr(p);
		}

	}
}

int parse_simple(const char *start, char *end, Atom *result)
{
	char *p;
	int value = strtoimax(start, &p, 10);
	/*it is integer*/
	if(p == end) {
		result->type = ATOM_INTEGER;
		result->value.integer = value;
		//return true;
		return ERROR_OK;
	}

	/*nil or symbol*/
	char *buf = malloc(end - start + 1);
	p = buf;
	while(start != end) {
		*p++= *start++;
	}
	*p = '\0';
	/*convert to uppercase*/
	convertToUpperCase(buf);
	if(strcmp(buf, "NIL") == 0)
		*result = nil;
	else
		*result = make_symbol(buf);
	free(buf);
	return ERROR_OK;

}

void convertToUpperCase(char *s)
{
	while(*s != '\0')
	{
		*s = toupper((unsigned char)*s);
		s++;
	}
}

/* " (+ 12 34) "  */
// which is four tokens

int lex(char *str, char **start, char **end)
{
	const char *ws = " \t\n";
	const char *delim = "() \t\n";
	const char *prefix = "()";
	str += strspn(str, ws);

	if(str[0] == '\0')
	{
		*start = NULL;
		*end = NULL;
		return ERROR_SYNTAX;
	}
	*start = str;

	if(str[0] == '(' || str[0] == ')')
		*end = str + 1;
	else
		*end = str + strcspn(str, delim);
	return ERROR_OK;
}

int main()
{
	/* test for lexer*/
	//const char *input = "  ( + (  - 11 2)	y. )  ";
	const char *input = "( quote hello )"	
	char *end;
	Atom p;
	read_expr(input, &end, &p);
	print_expr(p);
	printf("\n");

	/*
	   Atom p;
	   p = make_integer(42);
	   print_expr(p);
	   printf("\n");


	   p = make_symbol("jacky lau");
	   print_expr(p);
	   printf("\n");


	   p = cons(make_integer(1), nil);
	   print_expr(p);
	   printf("\n");


	   p = nil;
	   print_expr(p);
	   printf("\n");

	   p = cons(make_integer(1), cons(make_integer(2), cons(make_integer(3), nil)));
	   print_expr(p);
	   printf("\n");


	   p = cons(make_integer(1), cons(make_integer(2), cons(make_integer(3), make_symbol("hello world"))));
	   print_expr(p);
	   printf("\n");


	   p = cons(make_symbol("X"), make_symbol("Y"));
	   print_expr(p);
	   printf("\n");


	   p = cons(make_symbol("+"), cons(cons(make_symbol("+"), cons(make_integer(2), cons(make_integer(3), nil))), cons(make_integer(4), nil)));
	   print_expr(p);
	   printf("\n");

	 */
}



