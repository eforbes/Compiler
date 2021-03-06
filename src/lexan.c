/*
 Evan Forbes
 Dr. Shenoi
 CS 4013
 Fall 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "machines/ws.h"
#include "machines/id.h"
#include "machines/long_real.h"
#include "machines/real.h"
#include "machines/int.h"
#include "machines/assignop.h"
#include "machines/punctuation.h"
#include "machines/relop.h"
#include "machines/addop.h"
#include "machines/mulop.h"
#include "machines/catch_all.h"

#include "symbol_table.h"
#include "reserved_words.h"
#include "lexerr.h"
#include "token.h"

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#define LINE_MAX 72

char buffer[LINE_MAX];
int b = 0, f = 0; // indexes within the buffer

FILE *input_file, *output_file, *token_file, *address_file;

void open_files(char *input_file_name) {
	char outputFileName[strlen(input_file_name)+6];
	strcpy(outputFileName, input_file_name);
	strcat(outputFileName, ".lexan");

	char tokenFileName[strlen(input_file_name)+6];
	strcpy(tokenFileName, input_file_name);
	strcat(tokenFileName, ".token");

	char addressFileName[strlen(input_file_name)+5];
	strcpy(addressFileName, input_file_name);
	strcat(addressFileName, ".addr");

	input_file = fopen(input_file_name, "r");
	output_file = fopen(outputFileName, "w");
	token_file = fopen(tokenFileName, "w");
	address_file = fopen(addressFileName, "w");
}

char* load_line(char *input_file_name) {
	if(input_file==NULL && input_file_name != NULL) {
		open_files(input_file_name);
	}
	if(input_file == NULL || output_file == NULL) {
		fprintf(stderr, "Can't open input or output file");
		exit(1);
	} else {
		return fgets(buffer, LINE_MAX, input_file);
	}
}

int current_machine = 0;

Token *run_next_machine() {
	switch(current_machine) {
		case 0: return run_ws();
		case 1: return run_id();
		case 2: return run_long_real();
		case 3: return run_real();
		case 4: return run_int();
		case 5: return run_assignop();
		case 6: return run_punctuation();
		case 7: return run_relop();
		case 8: return run_addop();
		case 9: return run_mulop();
		default: return run_catch_all();
	}
}

char *get_lexeme() {
	int length = f - b;
	char *lexeme = (char*) malloc((length + 1) * sizeof(char));
	memcpy(lexeme, &buffer[b], length);
	lexeme[length] = '\0';
	return lexeme;
}

int first_call = 1;
char* input_file_name;
int line_number = 1, lexerr_count = 0, first_eof=1;

Token *get_token() {
	if(first_call) {
		init_reserved_words();

		load_line(input_file_name);

		fprintf(output_file, "%d\t\t%s", line_number, buffer);

		first_call = 0;
	}

	if(feof(input_file) || first_eof == 0) {
		if(first_eof){
			fprintf(token_file,
						"%d\t%s\t%d\t%d\n",
						line_number,
						"EOF",
						TOK_EOF,
						0);

			fclose(input_file);
			fclose(output_file);
			fclose(token_file);
			fclose(address_file);

			first_eof = 0;
		}

		return token_new(TOK_EOF, 0);
	}

	while(buffer[f] == '\n') {
		load_line(input_file_name);
		line_number++;

		if(feof(input_file)) {
			return get_token();
		}

		fprintf(output_file, "%d\t\t%s", line_number, buffer);

		f = 0;
		b = 0;
	}

	current_machine = 0;
	Token *result = run_next_machine();
	while(result -> token == TOK_BLOCKED) {
		f = b; // reset f
		current_machine++; // move to next machine
		result = run_next_machine();
	}

	char *lexeme = get_lexeme();

	if(result -> token == TOK_LEXERR) {
		lexerr_count++;

		print_lexerr_text(output_file, lexeme, result -> attribute);
	}

	b = f; // advance b

	if(result -> token != TOK_WS) {
		if(result -> token != TOK_ID) {
			fprintf(token_file,
					"%d\t%s\t%d\t%d\n",
					line_number,
					lexeme,
					result->token,
					result->attribute);
		} else {
			fprintf(token_file,
					"%d\t%s\t%d\t%p\n",
					line_number,
					lexeme,
					result->token,
					result->lex);
		}
		return result;
	} else {
		return get_token();
	}

}

char next_char() {
	return buffer[f++];
}

void move_f_back() {
	f--;
}

//For testing lexan
//int main(int argc, char **argv) {
//	if(argc != 2) {
//		puts("File name required as argument");
//		return -1;
//	}
//
//	input_file_name = argv[1];
//
//	while(get_token()->token != TOK_EOF){}
//
//	printf("Lexical analysis completed for %s with %d errors\n",
//			input_file_name,
//			lexerr_count);
//
//	return 0;
//}
