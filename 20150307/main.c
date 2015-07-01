#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include"type.h"
#include"atom.h"
#include"environment.h"

#define MAXLINE 1024

int read_expr(char *input, char **end,  Atom *result);
int read_list(char *start, char **end, Atom *result);
int parse_simple(char *start, char *end, Atom *result);

int lex(char *str, char **start, char **end);

void convertToUpperCase(char *str);



int read_expr(char *input, char **end,  Atom *result)
{
	char *token;
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


int read_list(char *start, char **end, Atom *result)
{
	Error err;
	char *token;
	Atom p = nil;
	*result = nil;

	*end = start; //
	for(; ;)
	{
		
		err = lex(*end, &token, end);
		//printf("%s\n", token);
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


int parse_simple(char *start, char *end, Atom *result)
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

const char *get_filename_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) 
	return "";
    return dot + 1;
}
void env_init(Atom *env)
{
	*env = env_create(nil);
	//env_set(env, make_symbol("IF"), make_function(builtin_if));

	env_set(*env, make_symbol("CAR"), make_function(builtin_car));
	env_set(*env, make_symbol("CDR"), make_function(builtin_cdr));
	env_set(*env, make_symbol("CONS"), make_function(builtin_cons));
	env_set(*env, make_symbol("+"), make_function(builtin_add));
	env_set(*env, make_symbol("-"), make_function(builtin_sub));
	env_set(*env, make_symbol("*"), make_function(builtin_mul));
	env_set(*env, make_symbol("/"), make_function(builtin_div));

	env_set(*env, make_symbol("<"), make_function(builtin_less));
	env_set(*env, make_symbol("="), make_function(builtin_equal));
	env_set(*env, make_symbol(">"), make_function(builtin_greater));

	env_set(*env, make_symbol("AND"), make_function(builtin_and));
	env_set(*env, make_symbol("OR"), make_function(builtin_or));
	env_set(*env, make_symbol("NOT"), make_function(builtin_not));

	env_set(*env, make_symbol("T"), make_symbol("T"));
}

int main(int argc, char **argv)
{
	Atom expr;
	Atom result;
	Atom env;

	Error err;
	char input[MAXLINE];
	char *end = input;

	env_init(&env);	
	//print_expr(env);

	FILE *fp = NULL;
	/* no arguments were passed */
	if(argc < 2)
		fp = stdin;
	else {
		fp = fopen(argv[1], "r");
		if(fp == NULL)
		{
		      fprintf(stderr, "Cannot open file %s\n", argv[1]);
		      return -1;
	   	}
		const char * file_extension = get_filename_extension(argv[1]);
		if(strcmp(file_extension, "liuyong") != 0)
		{
		      fprintf(stderr, "file extension must be .liuyong\n");
		      return -1;
	   	}
	}
	
	
	printf("> ");
	while(fgets(input, MAXLINE, fp) != NULL)
	{
		err = read_expr(input, &end, &expr);
		//print_expr(expr);
		if(!err)
			err = eval_expr(expr, env, &result);
		switch(err)
		{
			case ERROR_OK:
				print_expr(result);
				printf("\n");
				break;
			case ERROR_SYNTAX:
				printf("Syntax Error\n");
				break;
			case ERROR_UNBOUND:
				printf("Symbol Not Bound\n");
				break;
			case ERROR_TYPE:
				printf("Wrong Type\n");
				break;
			case ERROR_ARGS:
				printf("Wrong Nomber Of Arguments\n");
				break;
		}
		printf("> ");
	}
}

#if 0

int main()
{
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

}


#endif
