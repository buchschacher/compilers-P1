#ifndef TOKEN_H
#define TOKEN_H

enum tokenID			{PNCtk, IDtk, INTtk, ERRtk, EOFtk, BGNtk, ENDtk, ITERtk, VOIDtk, VARtk, RTNtk, READtk, PRNTtk, PROGtk, CONDtk, THENtk, LETtk};
const char tokenNames[][20] = {"OPtk", "IDtk", "INTtk", "ERRtk", "EOFtk", "BEGINtk", "ENDtk", "ITERtk", "VOIDtk", "VARtk", "RETURNtk", "READtk", "PRINTtk", "PROGRAMtk", "CONDtk", "THENtk", "LETtk"};

typedef struct token
{
	tokenID type;
	char inst[9];
	int line;
} token_t;

#endif
