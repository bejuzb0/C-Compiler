#include "sym_table.h"
char datatype[5][20] = {"int", "char", "float", "double", "long"};
int datatypesize[5] = {sizeof(int), sizeof(char), sizeof(float),sizeof(double), sizeof(long)};
int bracket = 0;
void parser(FILE* fr, FILE* fw) {
    for(;;) {
        token *s,*t,*u,*v,*w;
		t = getNextToken(fr);
		if(t->type==-1)
			break;
		else if(t->type == 0)
			continue;
        else if(t->type == 12) bracket++;
        else if(t->type == 13) bracket--;
		else if(t->type == 1) { // int
            fprintf(fw, "%c%s%c%d%c%d%c%d%c\n", '<', t->lexemename, ',', t->rw, ',', t->cl, ',', t->type, '>');
            while((s = getNextToken(fr)) && s->type ==0);
            fprintf(fw, "%c%s%c%d%c%d%c%d%c\n", '<', s->lexemename, ',', s->rw, ',', s->cl, ',', s->type, '>');

            if(s->type == 2) { //main
                while((u = getNextToken(fr)) && u->type ==0);
                fprintf(fw, "%c%s%c%d%c%d%c%d%c\n", '<', u->lexemename, ',', u->rw, ',', u->cl, ',', u->type, '>');

                if(u->type == 10) { //(
                    //is a function
                    int arg_count = 0;
                    char buff[50];
                    memset(buff, '\0', 50);
                    
                    while((v = getNextToken(fr)) && v->type != 11) {
                        if(v->type == 0)
                            continue;
                        fprintf(fw, "%c%s%c%d%c%d%c%d%c\n", '<', v->lexemename, ',', v->rw, ',', v->cl, ',', v->type, '>');
                        while((w = getNextToken(fr)) && w->type ==0);
                        for(int i=0; i<5; i++) {
                            if(!strcmp(v->lexemename, datatype[i])) {
                                w->typestr = v->lexemename;
                                w->tokensize = datatypesize[i];
                            }
                        }
                        w->scope = 'L';
                        INSERT(ptrToObjToken(w));
                        strcat(buff, " ");
                        strcat(buff, w->lexemename);
                        arg_count++;
                    }
                    
                    fprintf(fw, "%c%s%c%d%c%d%c%d%c\n", '<', v->lexemename, ',', v->rw, ',', v->cl, ',', v->type, '>');
                    s->typestr = "FUNC";
                    strcpy(s->arg_name,buff);
                    s->arg_no = arg_count;
                    s->retType = t->lexemename;
                    s->type = 14;
                    s->scope = 'G';
                    INSERT(ptrToObjToken(s));



                }
                else {
                    //just an integer;
                    s->typestr = t->lexemename;
                    for(int i=0; i<5; i++) {
                        if(!strcmp(t->lexemename, datatype[i])) {
                            s->typestr = t->lexemename;
                            s->tokensize = datatypesize[i];
                            }
                        }
                    if(bracket == 0) s->scope = 'G';
                    else s->scope = 'L';
                    INSERT(ptrToObjToken(s));
                    
                }

                

                
            }
            else {
                //Put it as a keyword
            }
            

        }
        else {
             fprintf(fw, "%c%s%c%d%c%d%c%d%c\n", '<', t->lexemename, ',', t->rw, ',', t->cl, ',', t->type, '>');
        }
			
	}
    Display();
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
*/