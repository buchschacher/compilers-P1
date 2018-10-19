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

token_t scanner(char *str)
{
	token_t token;
	static int offset = 0;
	static int line = 1;
	int state = 0;
	token.inst[0] = '\0';

	// Call FSA until a final state is reached
	while ((state >= 0) && (state <= 4))
	{
		char lookahead = str[offset];
		state = nextState(state, lookahead);

		// Build string and advance lookahead
		if (state < 100)
		{
			if ((state != 0) && (state != 4))
			{
				token.inst[strlen(token.inst) + 1] = '\0';
				token.inst[strlen(token.inst)] = lookahead;
			}
			if (lookahead == '\n')
			{
				line++;
			}
			offset++;
		}
	}

	// Build token
	token.line = line;
	token.type = termType(state, token.inst, line);

	return token;
}

/* Return next state of FSA based on current state and lookahead character */
int nextState(int state, char c)
{
	const int states = 5;
	const int paths = 8;
	int fsa[states][paths] = {
		{  0,   1,  -2,   3,   2,   4, 100,  -1},
		{101,   1,   1,   1, 101, 101, 101,  -1},
		{102, 102, 102, 102, 102, 102, 102,  -1},
		{103, 103, 103,   3, 103, 103, 103,  -1},
		{  4,   4,   4,   4,   4,   0,   0,   4}
	};
	int path;

	if (isspace(c))
		path = 0;
	else if (islower(c))
		path = 1;
	else if (isupper(c))
		path = 2;
	else if (isdigit(c))
		path = 3;
	else if (ispunct(c) && c != '#')
	{
		char valid[] = "=<>:+-*/%.(),{};[]";
		if (strchr(valid, c) != NULL)
			path = 4;
		else
			path = 7;
	}
	else if (c == '#')
		path = 5;
	else if (c == '\0')
		path = 6;
	else
		path = 7;

	return fsa[state][path];
}

/* Pass the terminal state of the FSA and return the token type or error message */
tokenID termType(int state, char *inst, int line)
{
	switch (state)
	{
		case -1:
			printf("Error: Invalid character on line %d\n", line);
			exit(4);
		case -2:
			printf("Error: Invalid token on line %d\n", line);
			exit(5);
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
