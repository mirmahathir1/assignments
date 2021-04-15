/* stack.c */

/* This program has a buffer overflow vulnerability. */
/* Our task is to exploit this vulnerability */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int bof(char *str)
{
    char buffer[24];

    /* The following statement has a buffer overflow problem */ 
    strcpy(buffer, str);
	printf("Returning from BOF\n");

    return 1;
}


int foo()
{
	printf("Sensitive Information Leaked\n");
	return 1;
}


int main(int argc, char **argv)
{
    char str[300];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    printf("Inside Main\n");
    fread(str, sizeof(char), 300, badfile);
    bof(str);

    printf("Returned Properly\n");
    return 1;
}
