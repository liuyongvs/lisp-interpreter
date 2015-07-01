#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, const char * argv[])
{
	printf("Who is cool? ");
	const char *name = readline(NULL);
	printf("%s is cool!\n\n", name);
	return 0;
}
