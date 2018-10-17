#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include "token.h"

int nextState(int, char);

token_t scanner(char *str)
{
	token_t token;
	static int offset = 0;
	static int line = 1;
	int state = 0;
	int tokenLen = 0;

	token.inst[0] = '\0';

	// Call FSA until a final state is reached
	while ((state >= 0) && (state < 100))
	{
		char lookahead = str[offset];
		state = nextState(state, lookahead);

		if (state < 100)
		{
			if ((state != 0) && (state != 4))
			{
				token.inst[tokenLen++] = lookahead;
				token.inst[tokenLen] = '\0';
			}
			if (lookahead == '\n')
				line++;
			offset++;
		}
	}

	// Set token or exit with error based on FSA final state
	switch (state)
	{
		case -1:
			printf("Error: Invalid character on line %d\n", line);
			exit(1);
		case -2:
			printf("Error: Invalid token on line %d\n", line);
			exit(2);
		case 100:
			token.type = EOFtk;
			break;
		case 101:
			token.type = IDtk;
			break;
		case 102:
			token.type = PNCtk;
			break;
		case 103:
			token.type =  INTtk;
			break;
		default:
			printf("Error: Invalid state reached (%d)\n", state);
			exit(3);
	}
	token.line = line;

	printf("[ %s | \"%s\" | %d ]\n", tokenNames[token.type], token.inst, token.line);


	return token;
}

int nextState(int state, char c)
{
	int fsa[5][8] = {
		{  0,   1,  -2,   3,   2,   4, 100,  -1},
		{101,   1,   1,   1, 101, 101, 101,  -1},
		{102, 102, 102, 102, 102, 102, 102,  -1},
		{103, 103, 103,   3, 103, 103, 103,  -1},
		{  4,   4,   4,   4,   4,   0,   0,   4}
	};
	int trans;

	if (isspace(c))
		trans = 0;
	else if (islower(c))
		trans = 1;
	else if (isupper(c))
		trans = 2;
	else if (isdigit(c))
		trans = 3;
	else if (ispunct(c) && c != '#')
		trans = 4;
	else if (c == '#')
		trans = 5;
	else if (c == '\0')
		trans = 6;
	else
		trans = 7;

	return fsa[state][trans];
}
