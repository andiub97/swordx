typedef struct List{ 
	char* parola; 
	int occorrenza;
	struct List* next;
}List;


List* createListElement();
void createListElement1(char*, int, List*);
void createListElement2(char*, List*);
void contains(List*, char*);
void incrementOcc(List*);
int isIgnored (char*, char**);
