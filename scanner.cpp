#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include "token.h"

token_t driver(char*);

int scanner(char *str)
{
	// yep
	token_t nextToken;
	do {
		nextToken = driver(str);
		printf("[ %s | \"%s\" | %d ]\n", tokenNames[nextToken.type], nextToken.inst, nextToken.line);

	} while ((nextToken.type != EOFtk) && (nextToken.type != ERRtk));

	return 0;
}

token_t driver(char *inStr)
{
	int nextState[5][8] = {
		{  0,   1,  -2,   3,   2,   4, 100,  -1},
		{101,   1,   1,   1, 101, 101, 101,  -1},
		{102, 102, 102, 102, 102, 102, 102,  -1},
		{103, 103, 103,   3, 103, 103, 103,  -1},
		{  4,   4,   4,   4,   4,   0,   0,   4}
	};

	static int offset = 0;
	static int line = 1;
	int state = 0;
	int trans = 0;

	int tokenLen = 0;

	token_t tempTk;
	tempTk.inst[0] = '\0';

	while ((state >= 0) && (state < 100))
	{
		// Lookahead at next character
		char lookahead = inStr[offset];
		if (isspace(lookahead))
			trans = 0;
		else if (islower(lookahead))
			trans = 1;
		else if (isupper(lookahead))
			trans = 2;
		else if (isdigit(lookahead))
			trans = 3;
		else if (ispunct(lookahead) && lookahead != '#')
			trans = 4;
		else if (lookahead == '#')
			trans = 5;
		else if (lookahead == '\0')
			trans = 6;
		else
			trans = 7;

		//printf("(%d)--'%c'-->\n", state, lookahead);

		state = nextState[state][trans];
		if (state < 100)
		{
			if ((state != 0) && (state != 4))
			{
				tempTk.inst[tokenLen] = lookahead;
				tokenLen++;
				tempTk.inst[tokenLen] = '\0';
			}
			offset++;
		}
	}

	switch (state)
	{
		case -1:
			printf("Error: Invalid character on line %d\n", line);
			exit(1);
		case -2:
			printf("Error: Invalid token on line %d\n", line);
			exit(2);
		case 100:
			tempTk.type = EOFtk;
			break;
		case 101:
			tempTk.type = IDtk;
			break;
		case 102:
			tempTk.type = PNCtk;
			break;
		case 103:
			tempTk.type =  INTtk;
			break;
		default:
			printf("Error: Invalid state reached (%d)\n", state);
			exit(3);
	}
	tempTk.line = line;
	return tempTk;
}
