#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parLog.h"

//creo primo nodo nullo
Parlog* createLogFile1(){
	Parlog* n = malloc(sizeof(Parlog));
	n -> name = NULL;
	n -> cw = 0;
	n -> iw = 0;
	n -> next = NULL;
	return n;
}


//creo altri nodo a partire dal primo
Parlog* createLogFile(char* filename, int cw1, int iw1, Parlog* firstLogFile ){
	Parlog* n = malloc(sizeof(Parlog));
	n -> name = strdup(filename);
	n -> cw = cw1;
	n -> iw = iw1;
	n -> next = NULL;
	 if(firstLogFile != NULL){
		Parlog* app = firstLogFile;
		while(app->next != NULL){
			app = app -> next;
		}
		app -> next = n; 
	}
		return n;

}

