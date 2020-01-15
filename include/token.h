#ifndef TOKEN_H
#define TOKEN_H


#define IDENT_ID 0
#define NUM_ID 1
#define OPER_ID 2
#define KEYW_ID 3
#define EOF_ID 4

#define ID_FS 10
#define DG_FS 11
#define OP_FS 12
#define KW_FS 13
#define EF_FS 14

#define KWTABLE_SIZE 12

extern const char* TK_NAMES[];
extern const int FSA_TABLE[7][7];
extern const char* KW_TABLE[];

typedef struct token_t {
	/* data */
	int tokenID;
	char* token;
	int line;
}token_t;

int fstoid(int);
void display_token(token_t);

#endif /* end of include guard: TOKEN_H */
