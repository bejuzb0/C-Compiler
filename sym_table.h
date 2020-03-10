#define TableLength 256
#include <stdio.h>
#include "token_gen.h"
#include <string.h>

struct ListElement {
	token tok;
	struct ListElement *next;
};


struct ListElement *TABLE[TableLength];

void Initialize() {
	for(int i=0; i<TableLength; i++)
		TABLE[i] = NULL;
}

void Display(){
	printf("Name\t\tType\t\tSize\t\tRet Type\tScope\t\tArgs\t\tArgsName\n");
	for(int i=0; i<100; i++) {
		printf("-");
	}
	printf("\n");
	for(int i=0; i<TableLength; i++) {
		struct ListElement *ele = TABLE[i];
		if(ele == NULL)
			continue;
		while(ele) {
			printf("%s\t\t%s\t\t%d\t\t%s\t\t%c\t\t%d\t\t%s\n", ele->tok.lexemename, ele->tok.typestr, ele->tok.tokensize, ele->tok.retType, ele->tok.scope, ele->tok.arg_no, ele->tok.arg_name );
			ele = ele->next;
		}
	}

}

int HASH(char *str) {
	int n = strlen(str);
	int hash = 0;
	for(int i=0; i<n; i++) {
		hash += str[i];
		hash = hash%TableLength;
	}
	return hash;
}
int SEARCH(char *str) {
	int n = strlen(str);
	int hash = HASH(str);
	
	struct ListElement *ele = TABLE[hash];
	if(ele == NULL)
		return 0;
	while(ele) {
		if(strcmp(ele->tok.lexemename, str) == 0)
			return 1;
		ele = ele->next;
	}
    
	return 0;

}
void INSERT(token tk) {
	if(SEARCH(tk.lexemename) == 1) 
		return;
	int val = HASH(tk.lexemename);
	struct ListElement *cur = (struct ListElement*)malloc(sizeof(struct ListElement));
	cur->tok = tk;
	cur->next = NULL;
	if(TABLE[val] == NULL)
		TABLE[val] = cur;
	else {
		struct ListElement *ele = TABLE[val];
		while(ele->next)
			ele = ele->next;
		ele->next = cur;
	}
	//printf("Value inserted %s\n", TABLE[val]->tok.lexemename);
}

token ptrToObjToken(token *tk) {
    token t;
    t.lexemename = tk->lexemename;
    t.nametype = tk->nametype;
    t.rw = tk->rw;
    t.cl = tk->cl;
    t.type = tk->type;
    t.arg_no = tk->arg_no;
    t.scope = tk->scope;
    t.tokensize = tk->tokensize;
    t.retType = tk->retType;
    t.typestr = tk->typestr;
    t.arg_name = tk->arg_name;
    return t;
}
