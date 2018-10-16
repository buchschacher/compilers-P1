#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

int driver(int, char);

int scanner(char *str)
{	
	const int tkMaxLen = 9;
	char tk[tkMaxLen];
	int tkLen = 0;
	int state = 0;
	static int offset = 0;
	static int line = 1;

	tk[0] = '\0';

	while ((state = driver(state, str[offset])) < 100)
	{
		//printf("(%d)--%c-->", state, str[offset]);
		if (str[offset] == '\n')
			line++;
		if ((state > 0) && (state != 4))
		{
			tk[tkLen] = str[offset];
			tk[tkLen + 1] = '\0';
			tkLen++;
		}
		offset++;
	}
	printf("Token: [%d, %s, %d]\n", state, tk, line);

	return state;
}

int driver(int state, char c)
{
	int transition = -1;
	int nextState[5][8] = {
		{  0,   1,  -1,   2,   3,   4, 200,  -2},
		{101,   1,   1,   1, 101, 101, 200,  -2},
		{102, 102, 102,   2, 102, 102, 200,  -2},
		{103, 103, 103, 103, 103, 103, 200,  -2},
		{  4,   4,   4,   4,   4,   0, 200,   4}};

	if (isspace(c))
	{
		transition = 0;
	}
	else if (islower(c))
	{
		transition = 1;
	}
	else if (isupper(c))
	{
		transition = 2;
	}
	else if (isdigit(c))
	{
		transition = 3;
	}
	else if (ispunct(c) && (c != '#'))
	{
		transition = 4;
	}
	else if (c == '#')
	{
		transition = 5;
	}
	else if (c == '\0')
	{
		transition = 6;
	}
	else
	{
		transition = 7;
	}

	return nextState[state][transition];
}
