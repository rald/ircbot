#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include <stdlib.h>
#include <stddef.h>

#include "util.h"

#define KNOWLEDGE_FILE "knowledge.txt"

typedef struct Knowledge Knowledge;
struct Knowledge {
	char *inp;
	char *res;
};

Knowledge *Knowledge_New(char *inp,char *res);

void Knowledge_Free(Knowledge ***knowledges,size_t *nknowledges);

void Knowledge_Add(Knowledge ***knowledges,size_t *nknowledges,Knowledge *knowledge);

void Knowledge_Print(Knowledge **knowledges,size_t nknowledges);

void Knowledge_Load(Knowledge ***knowledges,size_t *nknowledges);

void Knowledge_Save(Knowledge **knowledges,size_t nknowledges);

char *Knowledge_Reply(Knowledge **knowledges,size_t nknowledges,char *inp);

#endif
