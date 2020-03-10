/* Removes Delimiters, Comments and Preprocessor Directives */
#include <stdio.h>
#include <string.h>
void delimiter_rem(FILE *fr, FILE *fw) {
    char c = 'a';
    while(c != EOF) {
        c = fgetc(fr);
        if(c == ' ' || c == '\t') {
            fprintf(fw, "%c", c);
            while((c = fgetc(fr)) == ' ' || c == '\t');
            fprintf(fw, "%c", c);
        }
        else if(c != EOF) fprintf(fw, "%c", c);
    }
}

void rem_comment(FILE *fr, FILE *fw) {
    char c = 'a';
    while( c != EOF) {
        c = fgetc(fr);
        if( c == '/') {
            c = fgetc(fr);
            if(c == '/') {
                while((c = fgetc(fr)) != '\n' && c != EOF);
            }
            else if( c == '*') {
                int done = 0;
                while(done == 0) {
                    while((c = fgetc(fr)) != '*' && c != EOF);
                    c = fgetc(fr);
                    if( c == '/') done = 1;
                }
            }
            else if(c != EOF) fprintf(fw, "%c", c);
        }
        else if(c != EOF) fprintf(fw, "%c", c);

    }

}
void preprocessor_rem(FILE* fr, FILE* fw) {
    char c = 'a';
    while(c != EOF) {
        char buffer[256];
        memset(buffer, '\0', 256);
        int index = 0;
        c = fgetc(fr);
        if(c == '\"') {
            buffer[index++] = c;
            c = fgetc(fr);
            while(c != '\"') {
                buffer[index++] = c;
                c = fgetc(fr);
            }
            buffer[index] = c;
            fprintf(fw, "%s", buffer);
        }
        else if(c == '#') {
            buffer[index++] = c;
            while((c = fgetc(fr)) != ' ' && c != EOF)
                buffer[index++] = c;
            buffer[index] = '\0';
            if(c == EOF)
                break;
            if(strncmp(buffer, "#include", 8) == 0 || strncmp(buffer, "#define", 7) == 0) {
                while((c = fgetc(fr)) != '\n' && c != EOF) {
                   //printf("%c", c);
                }
            }
            else {
                fprintf(fw, "%s", buffer);
            }
        }
        else if( c != EOF) {
            fprintf(fw, "%c", c);
        }
    }

}