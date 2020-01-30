#include "rem_prepndel.h"
#include "token_gen.h"
#include <stdlib.h>

int main() {
    FILE* fr = fopen("try.c", "rw");
    FILE* fw = fopen("trywrite.c", "w");

    preprocessor_rem(fr,fw);

    fclose(fw);
    fclose(fr);
    fr = fopen("trywrite.c", "r"); 
    fw = fopen("trywrite2.c", "w");

    delimiter_rem(fr, fw);
    fclose(fr);
    fclose(fw);

    fr = fopen("trywrite2.c", "r"); 
    fw = fopen("trywrite3.c", "w");

    rem_comment(fr, fw);
    fclose(fr);
    fclose(fw);

    fr = fopen("trywrite3.c", "r");
	if(fr == NULL){
		printf("Cannot open file\n");
		exit(0);
	}
	fw = fopen("tokens.txt", "w");
	
	for(;;){
		token* t = getNextToken(fr);
		if(t->type==-1)
			break;
		else if(t->type == 0)
			continue;
		else
			fprintf(fw, "%c%s%c%d%c%d%c%d%c\n", '<', t->lexemename, ',', t->row, ',', t->col, ',', t->type, '>');
	}


}