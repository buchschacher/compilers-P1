#ifndef TOKEN_H
#define TOKEN_H

enum tokenID {PNCtk, IDtk, INTtk, ERRtk, EOFtk, BGNtk, ENDtk, ITERtk, VOIDtk, VARtk, RTNtk, READtk, PRNTtk, PROGtk, CONDtk, THENtk, LETtk};
const char tokenNames[][20] ={"Operator", "Identifier", "Number", "Error", "EndOfFile"};

typedef struct token
{
	tokenID type;
	char inst[9];
	int line;
} token_t;

#endif
