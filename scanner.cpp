#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <map>

#include "token.h"

using namespace std;

int getNextState(int, char);
tokenID termType(int);
tokenID keywordLookup(char*);

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
		nextState = getNextState(state, nextChar);
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
			if ((token.type = termType(nextState)) == IDtk)
				token.type = keywordLookup(token.inst);
			return token;
		}
		else
		{
			if (nextChar == '\n')
				token.line = ++line;
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

int getNextState(int state, char nextChar)
{
	// FSA diagram represented as a table
	const int states = 5;
	const int paths = 8;
	const int fsa[states][paths] = {
		{  0,   1,  -2,   3,   2,   4, 100,  -1},
		{101,   1,   1,   1, 101, 101, 101,  -1},
		{102, 102, 102, 102, 102, 102, 102,  -1},
		{103, 103, 103,   3, 103, 103, 103,  -1},
		{  4,   4,   4,   4,   4,   0,   0,   4}
	};

	// Process next character into column index for FSA table
	int path;
	if (isspace(nextChar))
		path = 0;
	else if (islower(nextChar))
		path = 1;
	else if (isupper(nextChar))
		path = 2;
	else if (isdigit(nextChar))
		path = 3;
	else if (ispunct(nextChar) && nextChar != '#')
	{
		char valid[] = "=<>:+-*/%.(),{};[]";
		if (strchr(valid, nextChar) != NULL)
			path = 4;
		else
			path = 7;
	}
	else if (nextChar == '#')
		path = 5;
	else if (nextChar == '\0')
		path = 6;
	else
		path = 7;

	return fsa[state][path]; 
}

/* Pass the terminal state of the FSA and return the token type or error message */
tokenID termType(int state)
{
	if (state == 100)
		return EOFtk;
	if (state == 101)
		return IDtk;
	if (state == 102)
		return PNCtk;
	if (state == 103)
		return INTtk;
	printf("Error: Invalid state reached (%d)\n", state);
	exit(6);
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
		return it->second;
	else
		return IDtk;
}
