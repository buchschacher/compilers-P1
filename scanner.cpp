#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include "token.h"
#include <string>
#include <map>

using namespace std;

int nextState(int, char);
tokenID stateType(int, char*, int);
tokenID idOrKw(char*);

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
	token.line = line;
	token.type = stateType(state, token.inst, line);
	//idkw(token.inst);
	// Check for reserved words

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
	{
		char valid[] = "=<>:+-*/%.(),{};[]";
		if (strchr(valid, c) != NULL)
			trans = 4;
		else
			trans = 7;
	}
	else if (c == '#')
		trans = 5;
	else if (c == '\0')
		trans = 6;
	else
		trans = 7;

	return fsa[state][trans];
}

tokenID stateType(int state, char *inst, int line)
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
			return idOrKw(inst);
		case 102:
			return PNCtk;
		case 103:
			return INTtk;
		default:
			printf("Error: Invalid state reached (%d)\n", state);
			exit(6);
	}
}

tokenID idOrKw(char *inst)
{
	string cppinst = inst;
	map<string, tokenID> kw;
	kw.insert(pair<string, tokenID>("begin", BGNtk));
	kw.insert(pair<string, tokenID>("end", ENDtk));
	kw.insert(pair<string, tokenID>("iter", ITERtk));
	kw.insert(pair<string, tokenID>("void", VOIDtk));
	kw.insert(pair<string, tokenID>("var", VARtk));
	kw.insert(pair<string, tokenID>("return", RTNtk));
	kw.insert(pair<string, tokenID>("read", READtk));
	kw.insert(pair<string, tokenID>("print", PRNTtk));
	kw.insert(pair<string, tokenID>("program", PROGtk));
	kw.insert(pair<string, tokenID>("cond", CONDtk));
	kw.insert(pair<string, tokenID>("then", THENtk));
	kw.insert(pair<string, tokenID>("let", LETtk));
	map<string, tokenID>::iterator it;

	it = kw.find(cppinst);
	if (it != kw.end())
		return it->second;
	else
		return IDtk;
}
