#ifndef TOKEN_H
#define TOKEN_H

enum tokenID {PNCtk, IDtk, INTtk, ERRtk, EOFtk};
const char tokenNames[][20] ={"Operator", "Identifier", "Number", "Error", "End of File"};

typedef struct token {
	tokenID type;
	char inst[9];
	int line;
} token_t;

#endif
