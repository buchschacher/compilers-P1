#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include "token.h"
#include <string>
#include <map>

using namespace std;

int nextState(int, char);
tokenID termType(int, char*, int);
tokenID keywordLookup(char*);
int charPath(char);

const int states = 5;
const int paths = 8;
int fsa[states][paths] = {
	{  0,   1,  -2,   3,   2,   4, 100,  -1},
	{101,   1,   1,   1, 101, 101, 101,  -1},
	{102, 102, 102, 102, 102, 102, 102,  -1},
	{103, 103, 103,   3, 103, 103, 103,  -1},
	{  4,   4,   4,   4,   4,   0,   0,   4}
};


token_t scanner(char *str)
{
	static int line = 1;
	static int offset = 0;
	static char nextChar = str[offset++];

	int state = 0;
	int nextState;
	token_t token;
	token.inst[0] = '\0';
	token.line = line;

	// Repeat until terminal state
	while ((state >= 0) && (state <= 4))
	{
		int path = charPath(nextChar);
		int nextState = fsa[state][path];

		if (nextState < 0)
		{
			switch (nextState)
			{
		 		case -1:
					printf("Error: Invalid character on line %d\n", line);
					exit(4);
				case -2:
					printf("Error: Invalid token on line %d\n", line);
					exit(5);
			}
		}
		else if (nextState > 4)
		{
			//printf("(%d)\n", nextState);
			switch (nextState)
			{
				case 100:
					token.type = EOFtk;
					return token;
				case 101:
					token.type = IDtk;
					return token;
				case 102:
					token.type = PNCtk;
					return token;
				case 103:
					token.type = INTtk;
					return token;
				default:
					printf("Error: Invalid state reached (%d)\n", state);
					exit(6);
			}
		}
		else
		{
			if (nextChar == '\n')
			{
				token.line = ++line;
			}
			if ((nextState != 0) && (nextState != 4))
			{
				token.inst[strlen(token.inst) + 1] = '\0';
				token.inst[strlen(token.inst)] = nextChar;
			}
			state = nextState;
			nextChar = str[offset++];
		}
	}
}

/* Return next state of FSA based on current state and lookahead character */
int charPath(char c)
{
	if (isspace(c))
		return 0;
	if (islower(c))
		return 1;
	if (isupper(c))
		return 2;
	if (isdigit(c))
		return 3;
	if (ispunct(c) && c != '#')
	{
		char valid[] = "=<>:+-*/%.(),{};[]";
		if (strchr(valid, c) != NULL)
			return 4;
		else
			return 7;
	}
	if (c == '#')
		return 5;
	if (c == '\0')
		return 6;
	return 7;
}


//=========================================

/* Pass the terminal state of the FSA and return the token type or error message */
tokenID termType(int state, char *inst, int line)
{
	switch (state)
	{
		case 100:
			return EOFtk;
		case 101:
			return keywordLookup(inst);
		case 102:
			return PNCtk;
		case 103:
			return INTtk;
		default:
			printf("Error: Invalid state reached (%d)\n", state);
			exit(6);
	}
}

/* Pass the instance from a token and determine if it is a reserve word and return
 * the corrisponding token type for the keyword or identifier */
tokenID keywordLookup(char *inst)
{
	// Initialize keyword list
	string sInst = inst;
	map<string, tokenID> keywordList;
	keywordList.insert(pair<string, tokenID>("begin", BGNtk));
	keywordList.insert(pair<string, tokenID>("end", ENDtk));
	keywordList.insert(pair<string, tokenID>("iter", ITERtk));
	keywordList.insert(pair<string, tokenID>("void", VOIDtk));
	keywordList.insert(pair<string, tokenID>("var", VARtk));
	keywordList.insert(pair<string, tokenID>("return", RTNtk));
	keywordList.insert(pair<string, tokenID>("read", READtk));
	keywordList.insert(pair<string, tokenID>("print", PRNTtk));
	keywordList.insert(pair<string, tokenID>("program", PROGtk));
	keywordList.insert(pair<string, tokenID>("cond", CONDtk));
	keywordList.insert(pair<string, tokenID>("then", THENtk));
	keywordList.insert(pair<string, tokenID>("let", LETtk));
	map<string, tokenID>::iterator it;

	// Check if identifier is in the keyword list
	it = keywordList.find(sInst);
	if (it != keywordList.end())
	{
		return it->second;
	}
	else
	{
		return IDtk;
	}
}
