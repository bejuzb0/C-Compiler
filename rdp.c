#include "parser.h"
FILE *fr;
int error_flag = 0;
token *s;
int error_count = 0;
void parsed(char *str) {
	printf("%s Parsed\n", str);
}
void getToken() {
	if(error_flag == 0) {
		parsed(s->lexemename);
	}
	s = getNextToken(fr);
	while(s->type == 0)
		s = getNextToken(fr);
}
void error(char * mes) {
	error_flag = 1;
	error_count++;
	printf("\nExpected %s\n", mes);
	printf("Line: %d, Col: %d\n", s->rw, s->cl);
	if(error_count >= 3)
		exit(0);
	while(strcmp(s->lexemename, "}") != 0 && strcmp(s->lexemename, ";") != 0 && strcmp(s->lexemename, ")") != 0) {
		getToken();
	}
	//printf("%s", s->lexemename);
}
void success() {
	printf("\nProgram Parsed\n");
	exit(0);
}
void addop() {
	if(!strcmp(s->lexemename, "+") || !strcmp(s->lexemename, "-")) {
		getToken();
	}
	else {
		error("+ | -");
	}
}

void mulop() {
	if(!strcmp(s->lexemename, "*") || !strcmp(s->lexemename, "/") || !strcmp(s->lexemename, "%")) {
		getToken();
	}
	else {
		error("* | / | %");
	}
}

void relop() {
	if(!strcmp(s->lexemename, "==")|| !strcmp(s->lexemename, "!=") || !strcmp(s->lexemename, "<=") || !strcmp(s->lexemename, ">=") || !strcmp(s->lexemename, ">") || !strcmp(s->lexemename, "<")) {
		getToken();
	}
	else error("== | != | <= | >= | > | <");
}
void factor() {
	if(s->type == 2 || s->type == 8) {
		getToken();
	} 
	else {
		error("factor");
	}
}
void tprime() {
	if(!strcmp(s->lexemename, "*") || !strcmp(s->lexemename, "/") || !strcmp(s->lexemename, "%")) {
		mulop();
		if(error_flag == 1) return;
		factor();
		if(error_flag == 1) return;
		tprime();
	}
	return;
}
void term() {
	factor();
	//if(error_flag == 1) return;
	tprime();
}
void seprime() {
	if(!strcmp(s->lexemename, "+") || !strcmp(s->lexemename, "-")) {
		addop();
		if(error_flag == 1) return;
		term();
		if(error_flag == 1) return;
		seprime();
	}
	else return;
}


void simple_expn() {
	if(s->type == 2 || s->type == 8) {
		term();
		if(error_flag == 1) return;
		seprime();
	}
	else return;
}
void eprime() {
	if(!strcmp(s->lexemename, "==")|| !strcmp(s->lexemename, "!=") || !strcmp(s->lexemename, "<=") || !strcmp(s->lexemename, ">=") || !strcmp(s->lexemename, ">") || !strcmp(s->lexemename, "<")) {
		relop();
		if(error_flag == 1) return;
		simple_expn();
	}
	else return;
}
void expn() {
	simple_expn();
	if(error_flag == 1) return;
	eprime();
}

void assign_stat() {
	if(s->type ==2) {
		getToken();
		if(!strcmp(s->lexemename, "=")) {
			getToken();
			expn();
		}
		else  {
			error("=");
		}
	}
	else error("identifier");
}
void statement_list();
void looping_stat() {
	if(!strcmp(s->lexemename, "while") ) {
		getToken();
		if(!strcmp(s->lexemename, "(")) {
			getToken();
			expn();
			if(!strcmp(s->lexemename, ")")) {
				getToken();
				if(!strcmp(s->lexemename, "{")) {
					getToken();
					statement_list();
					if(!strcmp(s->lexemename, "}")) {
						error_flag = 0;
						getToken();
						return;
					}
					else {
						error("}");
						error_flag = 0;
						getToken();
						return;
					}
				}
				else error("{");
			}
			else {

				error(")");
				error_flag = 0;
				getToken();
				if(!strcmp(s->lexemename, "{")) {
					getToken();
					statement_list();
					if(!strcmp(s->lexemename, "}")) {
						error_flag = 0;
						getToken();
						return;
					}
					else {
						error("}");
						error_flag = 0;
						getToken();
						return;
					}
				}
				else error("{");
			}

		}
		else {
			error("(");
		}
	}
	else if(!strcmp(s->lexemename, "for")) {
		getToken();
		if(!strcmp(s->lexemename, "(")) {
			getToken();
			assign_stat();
			if(!strcmp(s->lexemename, ";")) {
				error_flag = 0;
				getToken();
				expn();
				if(!strcmp(s->lexemename, ";")) {
					error_flag = 0;
					getToken();
					assign_stat();

					if(error_flag == 1) return;

					if(!strcmp(s->lexemename, ")")) {
						getToken();
						if(!strcmp(s->lexemename, "{")) {
							getToken();
							statement_list();
							if(!strcmp(s->lexemename, "}")) {
								error_flag = 0;
								getToken();
								return;
							}
							else {
								error("}");
								error_flag = 0;
								getToken();
								return;
							}
						}
						else error("{");
					}
					else error(")");
				}
				else error(";");

			}
			else {
				error(";");
				
			}
		}
		else error("(");



	}
	else {
		error("while | for");
	}
}
void dprime() {
	if(!strcmp(s->lexemename, "else")) {
		getToken();
		if(!strcmp(s->lexemename, "{")) {
			getToken();
			statement_list();
			if(!strcmp(s->lexemename, "}")) {
				error_flag = 0;
				getToken();
				return;
			}
			else {
				error("}");
				error_flag = 0;
				getToken();
				return;
			}

		}
		else error("{");
	}
}

void case_list() {
	if(!strcmp(s->lexemename, "case")) {
		getToken();
		if(s->type == 8) {
			getToken();
			if(!strcmp(s->lexemename, ":")) {
				getToken();
				statement_list();
				if(!strcmp(s->lexemename, "break")) {
					getToken();
					if(!strcmp(s->lexemename, ";")) {
						getToken();
						case_list();	
					}
					else {
						error(";");
						error_flag = 0;
						getToken();
						case_list();
					}
				}
				else case_list();
			}
			else error(":");
		}
		else error("Number");
	}
	else return;
}
void default_case () {
	if(!strcmp(s->lexemename, "default")) {
		getToken();
		if(!strcmp(s->lexemename, ":")) {
			getToken();
			statement_list();
			if(!strcmp(s->lexemename, "break")) {
				getToken();
				if(!strcmp(s->lexemename, ";")) {
					getToken();
					return;	
				}
				else {
					error(";");
					error_flag = 0;
					getToken();
					return;
				}
			}
			else return;
			
		}
		else error(":");
	}
	else return;
}
void decision_stat() {
	if(!strcmp(s->lexemename, "if")) {
		getToken();
		if(!strcmp(s->lexemename, "(")) {
			getToken();
			expn();
			if(!strcmp(s->lexemename, ")")) {
				getToken();
				if(!strcmp(s->lexemename, "{")) {
					getToken();
					statement_list();
					if(!strcmp(s->lexemename, "}")) {
						getToken();
						dprime();
					}
					else {
						error("}");
						error_flag = 0;
						getToken();
						dprime();
					}

				}
				else error("{");
			}
			else {
				error(")");
				getToken();
				error_flag =0;
				if(!strcmp(s->lexemename, "{")) {
					getToken();
					statement_list();
					if(!strcmp(s->lexemename, "}")) {
						getToken();
						dprime();
					}
					else {
						error("}");
						error_flag = 0;
						getToken();
						dprime();
					}

				}
				else error("{");
			}
		}
		else {
			error("(");
		}
	}
	else if(!strcmp(s->lexemename, "switch")) {
		getToken();
		if(!strcmp(s->lexemename, "(")) {
	  		getToken();	
	  		if(s->type == 2 || s->type == 8) {
	  			getToken();
		  		if(!strcmp(s->lexemename, ")")) {
		  			getToken();
		  			if(!strcmp(s->lexemename,"{")) {
		  				getToken();
		  				case_list();
		  				default_case();
		  				//printf("%s", s->lexemename);
		  				if(!strcmp(s->lexemename, "}")) {
		  						getToken();
		  						return;
		  				}
		  				else {
		  					error("}");
		  					error_flag = 0;
		  					getToken();
		  					return;
		  				}
		  			}
		  			else error("{");
		  			
		  		}
		  		else error(")");
		  	}
	  		else error("identifier | num");
	  	}
	  	else error("(");

	}
	else error("if | switch");
}
void statement() {
	if(s->type == 2) {
		assign_stat();

		if(!strcmp(s->lexemename, ";")) {
			//error_flag = 0;
			getToken();
			return;
		}
		else {
			error(";");
			error_flag = 0;
			getToken();
			return;
		}
	}
	else if(!strcmp(s->lexemename, "if") || !strcmp(s->lexemename, "switch")) {
		decision_stat();
	}
	else if(!strcmp(s->lexemename, "while") || !strcmp(s->lexemename, "for")) {
		looping_stat();
	}
	else error("identifer | if | while | for | switch");
}

void statement_list() {
	//s = getNextToken(fr);
	if(s->type == 2 || !strcmp(s->lexemename, "if") || !strcmp(s->lexemename, "while") || !strcmp(s->lexemename, "for") || !strcmp(s->lexemename, "switch")) { // for loops etc
		statement();

		statement_list();
	}
	return;
}


void identifier_list() {

	if(s->type == 2) {
		getToken();
		if(!strcmp(s->lexemename, ",")) {
			getToken();
			identifier_list();
		}
		else if(!strcmp(s->lexemename, "[")) {
			getToken();
			if(s->type == 8) {
				getToken();
				if(!strcmp(s->lexemename, "]")) {
					getToken();
					if(!strcmp(s->lexemename, ",")) {
						getToken();
						identifier_list();
					}
					else return;
				}
			}
		}
		else {
			return;
		}
	}
	else {
		error("identifier");
	}
}
void data_type() {
	if(!strcmp(s->lexemename, "int") || !strcmp(s->lexemename, "char")) {
		getToken();
	}
	else {
		error("int | char");
	}
}
void declarations(){
	if(!strcmp(s->lexemename, "int") || !strcmp(s->lexemename, "char")) {
		data_type();
		identifier_list();
		if(!strcmp(s->lexemename,";")) {
			getToken();
			declarations();
		}
		else {
			//printf("%d", s->type);
			error(";");
			error_flag = 0;
			getToken();
			declarations();
		}
	}

}
void program() {
	printf("Program started\n");
	s = getNextToken(fr);
	while(s->type == 0) {
	  	s = getNextToken(fr);
	}

	if(!strcmp(s->lexemename, "main")) {
	  	getToken();
	  	if(!strcmp(s->lexemename, "(")) {
	  		getToken();	
	  		if(!strcmp(s->lexemename, ")")) {
	  			getToken();
	  			if(!strcmp(s->lexemename,"{")) {
	  				getToken();
	  				declarations();
	  				statement_list();
	  				//printf("%s", s->lexemename);
	  				if(!strcmp(s->lexemename, "}")) {
	  					error_flag = 0;
	  					parsed(s->lexemename);
	  					success();
	  					return;
	  				}
	  				else {
	  					error("}");
	  					error_flag = 0;
	  					parsed(s->lexemename);
	  					success();
	  					return;
	  				}
	  			}
	  			else error("{");
	  			
	  		}
	  		else error(")");
	  	}
	  	else error("(");

	  }
	  else error("main");
}
int main() {
	fr = fopen("input.c", "r");
	FILE *fw = fopen("rdpoutput.c", "w");
	parser(fr, fw);
	fclose(fr);
	fr = fopen("input.c", "r");
	program();
	fclose(fr);
}

/*
Handles 
a == 10;
a = b a;
a = a'A';
a = b+;
*/