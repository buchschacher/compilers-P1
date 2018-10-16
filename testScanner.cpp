#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "scanner.h"

/* Read from file pointer into a string, then call scanner until an EOF token is returned*/
void testScanner(FILE *fp)
{
	const int bufflen = 128;
	char buff[bufflen];
	char *content;
	int contentlen = 1;
																					
	// Allocate memory
	content = (char*)malloc(sizeof(char));
	if (content == NULL)
	{
		printf("Could not allocate memory\n");
		exit(1);
	}
	content[0] = '\0';

	// Read file line by line
	while(fgets(buff, bufflen, fp) != NULL)
	{
		// Reallocate memory
		char *oldPtr = content;
		contentlen = contentlen + strlen(buff);
		content = (char*)realloc(content, contentlen);

		// Free memory on failur
		if(content == NULL)
		{
			printf("Cloud not reallocate memory\n");
			free(oldPtr);
			exit(2);
		}

		// Concatenate
		strcat(content, buff);
	}

	printf("====BOF====\n%s====EOF====\n", content);

	// Return tokens until EOF
	while (scanner(content) != 200);
}
