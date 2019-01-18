#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

typedef struct List{ 
	char* parola; 
	int occorrenza;
	struct List* next;
}List;

