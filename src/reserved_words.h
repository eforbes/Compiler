
#ifndef RESERVED_WORDS_H_
#define RESERVED_WORDS_H_

#include "token.h"

struct ReservedWordNode {
	char *word;
	int token;
	int attr;
	struct ReservedWordNode *next;
};

typedef struct ReservedWordNode ReservedWordNode;

ReservedWordNode *reserved_word_node_new(char *sym, int t, int a, ReservedWordNode *next);

void init_reserved_words();
Token *get_reserved_token(char *sym);

#endif /* RESERVED_WORDS_H_ */
