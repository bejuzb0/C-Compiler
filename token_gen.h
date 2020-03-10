#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
typedef struct A {
	char* lexemename;
	unsigned int rw, cl;
	unsigned int type;
	char *typestr;
	char *retType;
	int tokensize;
	char scope;
	int arg_no;
	char *arg_name;
	char* nametype;
} token;

char keyword[32][15] = {	"auto", "break", "case", "char", "const",
							"continue", "default", "do", "double", "else",
							"enum", "extern", "float", "for", "goto",
							"if", "int", "long", "register", "return",
							"short", "signed", "sizeof", "static", "struct",
							"switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

int check(char str[]){
	for(int i=0;i<32;i++){
		if(strlen(str)!=strlen(keyword[i]))
			continue;
		else if(!strcmp(str, keyword[i]))
			return i;
	}
	return -1;
}

int row=1;
int col=0;

void retract(FILE* f){
    fseek(f, -1, SEEK_CUR);
    col--;
}

token* makeToken(char name[], int r, int c, int type, int len, char nametype[]){
	
    token* t = (token*)malloc(sizeof(token));
    t->lexemename = (char*)malloc(len*sizeof(char));
    t->nametype = (char*)malloc(50*sizeof(char));
    memset(t->nametype, '\0', 50);
    strcpy(t->lexemename, name);
    t->nametype = nametype;
    t->rw = r;
	t->cl = c;
    t->type = type;
	t->arg_no = -99;
	t->typestr = '\0';
	t->tokensize = 1;
	t->retType = '\0';
	t->arg_name = (char*)malloc(50*sizeof(char));
    return t;
}

token* getNextToken(FILE* f){

	char buf[30];
	int len = 0;

	char c = getc(f);
	col++;
	if(c == '\'') {
		col++;
		buf[len++] = c;
		c = getc(f);
		col++;
		buf[len++] = c;
		c = getc(f);
		if(c == '\'') {
			col++;
			buf[len++] = c;
			buf[len] = '\0';
			return makeToken(buf, row, col-len+1, 8, len, "char");
		}

	}
	else if(c == EOF)
		return makeToken(buf, row, col, -1, 0, "EOF");
	else if( c == ',') {
		buf[len++] = c;
		col++;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 21, len, ",");
	}
	
    else if( c== ';') {
    	buf[len++] = c;
    	buf[len] = '\0';
    	col++;
    	return makeToken(buf, row, col-len+1, 20, len, ";");
    }
 
    else if(isalpha(c) || c=='_'){
		
		buf[len++] = c;
		c = getc(f);
		col++;
		while(isalpha(c) || c=='_' || isdigit(c)){
			buf[len++] = c;
			c = getc(f);
			col++;
		}
		retract(f);
		buf[len] = '\0';
		int l = check(buf);
		if(l!=-1) return makeToken(buf, row, col-len+1, 1, len, "ID");
		else return makeToken(buf, row, col-len+1, 2, len, "KW");
	}
 
	else if(isdigit(c)){
		
        int exp_flag = 0;
		buf[len++] = c;
		c = getc(f);
		col++;
		if(isdigit(c) || c == 'x' || c == 'X'){
			if( c == 'x'  || c == 'X') {
				col++;
				buf[len++] = c;
				c = getc(f);
				while(isdigit(c)) {
					buf[len++] = c;
					col++;
					c = getc(f);
				}
				buf[len] = '\0';
				return makeToken(buf, row, col-len+1, 8, len, "NUMBER");
			}

            while(isdigit(c)){
                buf[len++] = c;
                c = getc(f);
                col++;
            }
            if(c == '.'){
				buf[len++] = c;
                c = getc(f);
		        col++;
                while(isdigit(c)){
                    buf[len++] = c;
                    c = getc(f);
                    col++;
                }
                if(c == 'E' || c == 'e')
                    exp_flag = 1;
            }
            else if(c == 'E' || c == 'e')
                exp_flag = 1;
			else if(c == 'x' || c == 'X') {
				buf[len++] = c;
				col++;
				c = getc(f);
				while(isdigit(c)) {
					buf[len++] = c;
					c = getc(f);
					col++;
				}
			}
            if(exp_flag){
                c = getc(f);
		        col++;
                if(c == '+' || c == '-'){
                    buf[len++] = c;
                    c = getc(f);
                    col++;
                }
                while(isdigit(c)){
                    buf[len++] = c;
                    c = getc(f);
                    col++;
                }
            }
            retract(f);
            buf[len] = '\0';
            return makeToken(buf, row, col-len+1, 8, len, "NUMBER");
        }
		else {
			buf[len] = '\0';
			retract(f);
			return makeToken(buf, row, col-len+1, 8, len, "NUMBER");
		}
        
	}
	else if(c == '+' || c == '-') {
		buf[len++] = c;
		c = getc(f);
		col++;
		if( c == '+' || c == '-') {
			buf[len++] = c;
			buf[len] = '\0';
			return makeToken(buf, row, col-len+1, 17, len, "CREMENT");
		}
		retract(f);
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 5, len, "ARITHMETIC");
	}

    else if(c == '\"'){
		c = getc(f);
		col++;
		while(c != '\"'){
			buf[len++] = c;
			c = getc(f);
			col++;
		}
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 9, len, "STRING");
    }

	else if(c == '('){
		col++;
		buf[len++] = c;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 10, len, "LR");
    }

	else if(c == '[') {
		col++;
		buf[len++] = c;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 15, len, "LS");
	}

	else if(c == ']') {
		col++;
		buf[len++] = c;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 16, len, "RS");
	}

    else if(c == ')'){
		col++;
		buf[len++] = c;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 11, len, "RR");
    }

    else if(c == '{'){
		col++;
		buf[len++] = c;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 12, len, "LC");
    }

    else if(c == '}'){
		col++;
		buf[len++] = c;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 13, len, "RC");
    }

	else if(c == '='){
        c = getc(f);
        col++;
		if(c == '='){
			strcpy(buf, "==");
            return makeToken(buf, row, col-2+1, 3, 2, "EQUALS");
		}
		else{
            retract(f);
			strcpy(buf, "=");
            return makeToken(buf, row, col-1+1, 4, 1, "ASSIGNMENT");
		}
	}

	else if(c == '>' || c == '<' || c == '!'){
		buf[len++] = c;
		c = getc(f);
		col++;
		if(c == '=')
			buf[len++] = c;
        else retract(f);
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 3, len, "RELOP");
	}

	else if(c == '*' || c == '/' || c == '%'){
		buf[len++] = c;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 5, len, "ARITHMETIC");
	}

	else if(c == '|' || c == '&'){
		buf[len++] = c;
		c = getc(f);
		col++;
		if(c == '&' || c == '|') buf[len++] = c;
        else retract(f);
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 6, len, "LOGICAL");
	}

	else if(!isalpha(c) && !isdigit(c) && c!='\n' && c!='\t' && c!=' '){
		buf[len++] = c;
		buf[len] = '\0';
		return makeToken(buf, row, col-len+1, 7, len, "SS");	
	}
	else if(c == '\n') {
		row++;
		//printf("Row increased\n");

		col = 0;
		return makeToken(buf, row, 0, 0, 0, "NEWLINE");
	}

	else if(c == '\t'){
		col += 3;
		return makeToken(buf, 0, 0, 0, 0, "TAB");
	}
	else if(c == ' ')
		return makeToken(buf, 0, 0, 0, 0, "SPACE");
	else c = getc(f);

	/*else if(c == EOF)
		return makeToken(buf, 0, 0, -1, 0, "EOF");
	else {
		retract(f);
		return makeToken(buf ,0, 0, 0, 0, "SOMETHINGELSE");
	}*/

}

/*
1: Keyword
2: Identifier
3: Relational
4: assignment
5: arithmetic
6: logical
7: spec sym
8: number
9: string
10: (
11: )
12: {
13: }
14: FUNC
15: [
16: ]
17: crement
*/
