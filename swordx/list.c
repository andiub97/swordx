#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"



//ciao
//creo un nodo in testa alla lista che è null
List* createListElement() {
  List* newList = malloc(sizeof(List));
		newList->parola = NULL;
		newList-> next = NULL;
	return newList;
}


//creo un nodo a partire da una lista l(usato solo nel caso dell'option update)
void createListElement1(char* str, int occ, List* l){
	List* newList = malloc(sizeof(List));
	(newList-> parola) = malloc (sizeof(char*));
	strcpy(newList->parola, str);
	newList-> next = NULL;
	newList->occorrenza = occ;
	List* app = l;
	while(app->next != NULL){
		app = app->next; 
		}
	
	app->next = newList;
	
}


//creo gli altri elementi della lista puntata dal primo nodo nullo
void createListElement2(char* str, List* l){
	List* newList = malloc(sizeof(List));
	(newList-> parola) = malloc (sizeof(char*));
	strcpy(newList->parola, str);
	newList-> next = NULL;
	newList->occorrenza = 1;
	List* app = l;
	while(app->next != NULL){
		app = app->next; 
		}
	
	app->next = newList;
	
}







void contains( List* list, char* str) {
	if(list  == NULL){
		 createListElement2(str, list);
	}else{
		List* app = list-> next;
		while(app  != NULL){
			int cmp = strcmp(app->parola, str);
			if (cmp == 0){
				return incrementOcc(app);
			}else{
				app= app->next;
			}
		}
	createListElement2(str, list);
	}
}



void incrementOcc(List* l){
	l->occorrenza += 1;
}




int isIgnored(char* str, char** wordsToIgnore){
	int i = 0;
	while(wordsToIgnore[i]!= NULL){
		if(strcmp(str,wordsToIgnore[i]) == 0){
			return 1;
		}
		i++;
		
	}return 0;
}


