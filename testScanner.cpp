#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "token.h"
#include "scanner.h"

/* Read from file pointer into a string, then call scanner until an EOF token is returned*/
void testScanner(FILE *fp)
{
	const int buffLen = 128;
	char buff[buffLen];
	char *content;
	int contentLen = 1;
																					
	// Innitialize memory for allocation
	content = (char*)malloc(sizeof(char));
	if (content == NULL)
	{
		printf("Could not allocate memory\n");
		exit(2);
	}
	content[0] = '\0';

	// Read file one line or buffLen characters at a time
	while (fgets(buff, buffLen, fp) != NULL)
	{
		// Reallocate memory
		char *oldPtr = content;
		contentLen = contentLen + strlen(buff);
		content = (char*)realloc(content, contentLen);
		if(content == NULL)
		{
			// Free memory on fail
			printf("Cloud not reallocate memory\n");
			free(oldPtr);
			exit(3);
		}
		strcat(content, buff);
	}

	// Return tokens until EOF
	token_t token = scanner(content);
	while (token.type != EOFtk)
	{
		printf("%s\t%s\t%d\n", tokenNames[token.type], token.inst, token.line);
		token = scanner(content);
	}

	return;
}
