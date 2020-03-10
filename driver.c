#include "rem_prepndel.h"
#include <stdlib.h>
#include "parser.h"

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
<<<<<<< HEAD
    fw = fopen("trywrite3.c", "w");
=======
    fw = fopen("trywrite.c", "w");
>>>>>>> ddc9e83b9fefc177d69624bbc4e88d2d324b5b4c

    rem_comment(fr, fw);
    fclose(fr);
    fclose(fw);

<<<<<<< HEAD
    fr = fopen("trywrite3.c", "r");
=======
    fr = fopen("trywrite.c", "r");
>>>>>>> ddc9e83b9fefc177d69624bbc4e88d2d324b5b4c
	if(fr == NULL){
		printf("Cannot open file\n");
		exit(0);
	}
	fw = fopen("tokens.txt", "w");
    parser(fr, fw);
    fclose(fr);
    fclose(fw);
<<<<<<< HEAD
}
=======
}
>>>>>>> ddc9e83b9fefc177d69624bbc4e88d2d324b5b4c
