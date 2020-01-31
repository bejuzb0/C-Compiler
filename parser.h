#include "sym_table.h"
char datatype[5][20] = {"int", "char", "float", "double", "long"};
int datatypesize[5] = {sizeof(int), sizeof(char), sizeof(float),sizeof(double), sizeof(long)};

char type[20][50] = {"KEYWORD", "IDENTIFIER", "RELATIONAL", "ASSIGNMENT", "ARITHMETIC", "LOGICAL", "SPEC. SYM.", "NUMBER", "STRING", "LR", "RR", "LC", "RC", "LS", "RS"};


int bracket = 0;
void parser(FILE* fr, FILE* fw) {
    for(;;) {
        token *s,*t,*u,*v,*w,*x, *y;
		t = getNextToken(fr);
		if(t->type==-1)
			break;
		else if(t->type == 0)
			continue;
        else if(t->type == 12) bracket++;
        else if(t->type == 13) bracket--;
		else if(t->type == 1) { // int
            fprintf(fw, "%c%s%c%d%c%d%c%s%c\n", '<', t->lexemename, ',', t->rw, ',', t->cl, ',', type[t->type-1], '>');
            while((s = getNextToken(fr)) && s->type ==0);
            fprintf(fw, "%c%s%c%d%c%d%c%s%c\n", '<', s->lexemename, ',', s->rw, ',', s->cl, ',', type[s->type-1], '>');

            if(s->type == 2) { //main
                while((u = getNextToken(fr)) && u->type ==0);
                fprintf(fw, "%c%s%c%d%c%d%c%s%c\n", '<', u->lexemename, ',', u->rw, ',', u->cl, ',', type[u->type-1], '>');

                if(u->type == 10) { //(
                    //is a function
                    int arg_count = 0;
                    char buff[50];
                    memset(buff, '\0', 50);
                    
                    while((v = getNextToken(fr)) && v->type != 11) {
                        if(v->type == 0)
                            continue;
                        fprintf(fw, "%c%s%c%d%c%d%c%s%c\n", '<', v->lexemename, ',', v->rw, ',', v->cl, ',', type[v->type-1], '>');
                        while((w = getNextToken(fr)) && w->type ==0);
                        for(int i=0; i<5; i++) {
                            if(!strcmp(v->lexemename, datatype[i])) {
                                w->typestr = v->lexemename;
                                w->tokensize = (w->tokensize)*(datatypesize[i]);
                                break;
                            }
                        }
                        w->scope = 'L';
                        INSERT(ptrToObjToken(w));
                        strcat(buff, " ");
                        strcat(buff, w->lexemename);
                        arg_count++;
                    }
                    
                    fprintf(fw, "%c%s%c%d%c%d%c%s%c\n", '<', v->lexemename, ',', v->rw, ',', v->cl, ',', type[v->type-1], '>');
                    s->typestr = "FUNC";
                    strcpy(s->arg_name,buff);
                    s->arg_no = arg_count;
                    s->tokensize = -99;
                    s->retType = t->lexemename;
                    s->type = 14;
                    s->scope = 'G';
                    INSERT(ptrToObjToken(s));



                }
                else if(u->type == 15) {
                    while((x = getNextToken(fr)) && x->type == 0);
                    if(x->type == 8) {
                       
                        int num = atoi(x->lexemename);
                        s->tokensize = num;
                        for(int i=0; i<5; i++) {
                            if(!strcmp(t->lexemename, datatype[i])) {
                                s->typestr = strcat(t->lexemename, " array");
                                s->tokensize = (s->tokensize)*(datatypesize[i]);
                                break;
                            }
                        }
                        s->type = 2;
                        if(bracket == 0) s->scope = 'G';
                        else s->scope = 'L';
                        INSERT(ptrToObjToken(s));



                    }
                    else if(x->type == 16) {
                        while((x = getNextToken(fr)) && x->type != 12);
                        int num = 0;
                        
                        while(x->type != 13) {
                            while((x = getNextToken(fr)) && x->type == 0);
                            //printf("%s %d\n", x->lexemename, x->type);
                            if(x->type == 5 || x->type == 8) {
                                num++;
                            }
                        }
                        printf("%d", num);
                        s->tokensize = num;
                        for(int i=0; i<5; i++) {
                            if(!strcmp(t->lexemename, datatype[i])) {
                                s->typestr = strcat(t->lexemename, " array");
                                s->tokensize = (s->tokensize)*(datatypesize[i]);
                                break;
                            }
                        }
                        s->type = 2;
                        if(bracket == 0) s->scope = 'G';
                        else s->scope = 'L';
                        INSERT(ptrToObjToken(s));

                    }


                }
                else {
                    //just an integer;
                    s->typestr = t->lexemename;
                    for(int i=0; i<5; i++) {
                        if(!strcmp(t->lexemename, datatype[i])) {
                            s->typestr = t->lexemename;
                            s->tokensize = (s->tokensize)*(datatypesize[i]);
                            }
                        }
                    if(bracket == 0) s->scope = 'G';
                    else s->scope = 'L';
                    INSERT(ptrToObjToken(s));
                    
                }

                

                
            }
            else {
                //fprintf(fw, "%c%s%c%d%c%d%c%d%c\n", '<', s->lexemename, ',', s->rw, ',',s->cl, ',', s->type, '>');
            }
            

        }
        else {
             fprintf(fw, "%c%s%c%d%c%d%c%s%c\n", '<', t->lexemename, ',', t->rw, ',', t->cl, ',', type[t->type-1], '>');
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
15: [
16: ]
*/