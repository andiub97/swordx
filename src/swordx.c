#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>
#include "list.h"
#include "parLog.h"



static int recursive_flag;
static int follow_flag;
static int alpha_flag;
static int sort_flag;

List* createListElement();
void UpdateListLog (char* , List* );
void lettura (char*, List*);
void StampaFile ( char*, List* );
void printHelp();
void checkName(char*, List*  );
int isDirectory(char*);
int isLink(char*);
int isRegular(char*);
int fileInDir(char*, List* );
List* controlloStringhe(List*, char[], int, int, char**);
void readIgnore();
void sort();
void counter(List*);
void copy( char*,List* );
void writeLog(char*);
void confronto(List* , List*, char*);
int confrontaOccorrenza(int, int);
int count [2] = {0,0};


static char* wordToIgnore= NULL;

static char *fileToExclude = NULL;
static int numMin = 0;
static char **wordsToIgnore = NULL;
static char *logFile= NULL;
static Parlog* firstLogFile = NULL;
static char *updateFile;
char* outputFile = "swordx.out" ;






void lettura (char* file, List* list){
	char str[100];
	List* app;
	FILE *fd;
	fd = fopen(file, "r");	
	if( fd==NULL )
	{
		perror("Errore in apertura del file");
		exit(1);
	}
			while (fscanf (fd, "%s", str) != EOF ){
				app=controlloStringhe(list, str, alpha_flag, numMin, wordsToIgnore);
				counter(app);
			}
}



List* controlloStringhe(List* list,char s[],int alpha_flag,int numMin, char** wordsToIgnore){
	char* str = calloc(strlen(s), sizeof(char));
	int i = 0;
	int z = 0;
		while(s[i] != '\0'){
			if(isalpha(s[i])){
				str[z] = tolower(s[i]);
			}else if(isdigit(s[i]))
					{
						if(alpha_flag == 0){
							str[z]=s[i];
						}else
							return NULL;				
					}
			i++;
			z++;
		}
		if(strlen(str) >= numMin){
			if((wordsToIgnore!=NULL) && isIgnored(str, wordsToIgnore)){
				return NULL;
			}else{ 
			contains(list,str); 
				return list;}
		}else   
			return NULL;
}


void counter(List* l){
	if (l != NULL){
		count[0] += 1;
	}else{count[1] += 1;}
}




int main(int argc, char* argv[]){
	int c;
	char** files;
	// Ho deciso di creare un puntatore al primo elemento di ciascuna lista nullo
	List* l = createListElement();
	List* v = createListElement(); 	// L'uso di questa lista dipende dalla presenza dell'opzione update
	
	
	while (1)
	{
		  static struct option long_options[] =
		  {
			  /* These options set a flag. */
			  {"recursive", no_argument, &recursive_flag, 1},
			  {"follow", no_argument, &follow_flag, 1},
			  {"alpha", no_argument, &alpha_flag, 1},
			  {"sortbyoccurrency", no_argument, &sort_flag, 1},
			   //These options set a flag.
			   //We distinguish them by their indices. 
			  {"help",  no_argument, 0, 'h'},
			  {"explude",  required_argument, 0, 'e'},
			  {"min",  required_argument, 0, 'm'},
			  {"ignore",  required_argument, 0, 'i'},
			  {"log",  required_argument, 0, 'l'},
			  {"output",  required_argument, 0 , 'o'},
			  {"update", required_argument, 0, 'u'},
			  {0, 0, 0, 0}
		  };
		  
        int option_index = 0;
		c = getopt_long_only (argc, argv, "he:m:i:l:o:u:", long_options, &option_index);
	  
		/* Detect the end of the options. */
		if (c == -1) break;
        
		switch (c)
		{
		  case 0:
				  /* If this option set a flag, do nothing else now. */
				  if (long_options[option_index].flag != 0) break;
				  			
		  case 'h':
					printHelp();
					break;   
			
		  case 'e':
					fileToExclude = optarg;
					break;
		  case 'm':
					numMin = atoi(optarg);
					break;
		  case 'i':
					wordToIgnore = optarg;
					readIgnore();
					break; 
		  case 'l':
					logFile = optarg;
					break;
		  case 'o':
					outputFile = optarg; 
					break;
		  case 'u':
		  			updateFile = optarg;
					break;
		  case '?':
					break;
		  default:
					abort ();
		}
	}




if (optind < argc)
	{
		int size = (argc - optind);
		files = calloc(size, sizeof(char**));
		int z = 0;
		
		while (optind < argc)
		{			
			files[z] = argv[optind];
			optind++;
			z++;
		}
		for(int i=0; i<z;i++)
		{
			checkName(files[i], l);
		}
		free(files);
		if(updateFile!=NULL){
			// legge il file della precedente esecuzione e crea la lista da confrontare con la nuova
			copy(outputFile, v);
			confronto(v,l,outputFile);
		}else{
			StampaFile(outputFile,l);
		}
		sort();
		if (logFile != NULL) 
			{ 
				writeLog(logFile);
			}else{}
		
	}else{
	printf("There aren't input files");
		exit (0);
	}

}


void checkName(char* filename, List* l){
	/* se NON è una directory e l'opzione logFile non è presente, fa la lettura, altrimenti se l'opzione è presente 
	   aggiorna la lista dei log; se è una directory, questa viene passata a fileInDir*/
		if(isDirectory(filename) == 0){
			if (isRegular(filename) == 1){
				if(logFile == NULL){
					lettura(filename, l);
				}else{ UpdateListLog(filename, l);
				}
			}
		}else{
			fileInDir(filename, l);
			
		}
}
 



void sort()
{
	if (sort_flag == 0)
	{
		int size = ((strlen(outputFile) * 2) + 9); /* 9 -> 1 + 8 (length di " sort -o " )*/
		char inout[size];
		strcpy(inout, "sort -o ");
		strcat(inout, outputFile);
		strcat(inout, " ");
		strcat(inout, outputFile);
		system(inout);
	}
	/*SORT BY OCCURRENCE*/
	else
	{
		int size = ((strlen(outputFile) * 2) + 17); /* 17 -> 1 + 16 (length di " sort -n -o " )*/
		char inout[size];
		strcpy(inout, "sort -k 2 -n -o ");
		strcat(inout, outputFile);
		strcat(inout, " ");
		strcat(inout, outputFile);
		system(inout);
	}
}



void StampaFile ( char* print, List* list){
	FILE* scritt = fopen(print, "w+");
		if (!scritt){
			exit(-1);
		}
	List* app = list -> next;
		while(app != NULL){
			fprintf( scritt, "%s", app->parola);
			fprintf(scritt, " ");
			fprintf(scritt, "%d", app->occorrenza);
			fprintf(scritt,"\n");
			app=(app->next);
		}
	fflush(scritt);
	fclose(scritt);
} 


  void writeLog(char* logFile){
	FILE* fp;
	fp = fopen(logFile, "w");
	if(fp == NULL){
		perror("errore in apertura del file");
		exit(1);
	}
	Parlog* app = firstLogFile;
	while(app != NULL){
		fprintf(fp, "%s ", app -> name);
		fprintf(fp, "%d ", app -> cw);
		fprintf(fp, "%d ", app -> iw);
		fprintf(fp, "%f\n", app -> time);
		app = (app->next);
  	}
	
	fflush(fp);
	fclose(fp);
}
 


int fileInDir(char* path, List* l){
	struct dirent *de;
	DIR *dr = opendir (path);
	while((de = readdir(dr)) != NULL){
		if ((fileToExclude != NULL) && (strcmp(de -> d_name, fileToExclude) == 0))//explude file
		{}
		else if (de-> d_name[0] !='.'){
			int sizePath = strlen(path) + strlen(de -> d_name) +1;
			char filename[sizePath];
			strcpy(filename, path);
			strcat(filename, "/");
			strcat(filename, de -> d_name);
			if((follow_flag==1) && isLink(filename)){//follow option
				if(logFile == NULL) {checkName(filename,l);} else UpdateListLog(filename,l); 
			}
			if(recursive_flag==1){//recursive option
				if(!isLink(filename)){
					if(isDirectory(filename)) fileInDir(filename,l);
				}
			}
			if(!isLink(filename) && !isDirectory(filename)){
				if(isRegular(filename)){
					if(logFile == NULL) {lettura(filename,l);} else UpdateListLog(filename,l); 
				}
			}
		}
	}
	closedir(dr);
	return 0;
}


void UpdateListLog (char* filename, List* l){
	clock_t t = clock();
	lettura(filename, l);
	Parlog* n = createLogFile(filename, count[0], count[1], firstLogFile);
	if(firstLogFile == NULL){ firstLogFile = n; }
	clock_t end = clock();
	double time_spent = (double)(end - t) / CLOCKS_PER_SEC;
	n->time= time_spent;
	count[0] = 0;
	count[1] = 0;
}



void copy( char* print, List* v){
	char str[100]; char str1[100];
	char *s;  
	int  occ;
	FILE* scritt = fopen(print, "r");
	if (!scritt){
		exit(-1);
	}
	while (fscanf (scritt, "%s %s", str, str1) != EOF){
			s = str;
			occ = atoi(str1);
			createListElement1(s,occ,v);
	} 
	
}

int confrontaOccorrenza(int occ, int occ1){
	int i = 0;
	i = occ1-occ; 
	return i;
} 


void confronto(List* v, List* l, char* print){
	FILE* scritt = fopen(print, "w+");
	if (!scritt){
		exit(-1);
	}
	List* app = l->next;
	List* app1 = v->next;
	int i;
	while (app1 != NULL){
		while(app != NULL){
			if(strcmp(app1->parola, app->parola) == 0){
				i = confrontaOccorrenza(app1->occorrenza, app->occorrenza);
				app1->occorrenza = 0;
				app->occorrenza = 0;
				if(i>0) {fprintf(scritt, "%s %s%s%d%s %s", app->parola, "(", "+",i, ")", "\n");}
				else{
				fprintf(scritt, "%s %s%d%s %s", app->parola, "(", i, ")", "\n");
				}
			}
			app= app->next;
		}
		if(app1->occorrenza != 0){
		  fprintf(scritt, "%s %s%s%d%s %s", app1->parola, "(", "-", app1->occorrenza, ")", "\n");
		}
		app1 = (app1->next);	
		app = l->next;
	}
		while(app != NULL){
			if(app->occorrenza != 0){
				fprintf(scritt, "%s %s%s%d%s %s", app->parola, "(", "+", app->occorrenza, ")", "\n");
			}
		app=app->next;
		}
	fflush(scritt);
	fclose(scritt);
}



 
void readIgnore(){
	wordsToIgnore = malloc(sizeof(char*)*1);
	int i=0;
	FILE *fd;
	char buf[100];
	fd = fopen(wordToIgnore,"r");
	if(fd == NULL){
		perror("Errore in apertura del file");
	}else{
		while (fscanf(fd,"%100s", buf  )==1){
			wordsToIgnore[i] = strdup(buf);
			i++;
			wordsToIgnore = (char**) realloc(wordsToIgnore, sizeof(char*)* i+1);
		}
	}
}


void printHelp(){
	printf("swordx [options] [inputs]\n");
	printf("\tswordx counts the occurrencies of each word input files and print them into a new output file.\n");
	printf("\n");
	printf("\t[inputs]\n");
	printf("\t\t files or Directory to read\n");
	printf("\t[options]\n");
	printf("\t\t -h | -help : print help message \n");
	printf("\t\t -r | -recursive : follow subdirectories \n");
	printf("\t\t -o <file> | -output <file> : write the result in the <file> (sword.out by default)\n");
	printf("\t\t -f | -follow : follow links \n");
	printf("\t\t -e <file> | -explude <file> : exclude <file> from the statistic \n");
	printf("\t\t -a | -alpha : consider alphabetic characters only \n");
	printf("\t\t -m <num> | -min <num> : consider words with at least <num> characters\n");
	printf("\t\t -i <file> | -ignore <file> : exclude words contained in <file> are ignored\n");
	printf("\t\t -s | -sortbyoccurrence : sort by number of occurrences (file is sorted by alphabetical order by default)\n");
	printf("\t\t -l <file> | -log <file> : write log informations in <file> \n");
	printf("\t\t\t the row sintax of this file is \n");
	printf("\t\t\t <name> cw iw time \n");
	printf("\t\t\t\t <name> : processed file\n");
	printf("\t\t\t\t cw : recorded words\n");
	printf("\t\t\t\t iw : ignored words\n");
	printf("\t\t\t\t time : file processing time \n");
	printf("\t\t -u <file> | -update <file> : if swordx.out (or another output file specified) is present, write on file <file>,\n");
	printf("\t\t  with the same output file format , the word list with the related changes respect the previous program execution.\n");
	printf("\n");
}
  


int isDirectory(char *path) 
{
   struct stat statbuf;
   
   if (stat(path, &statbuf) != 0)
       return 0;
   
   return S_ISDIR(statbuf.st_mode);
}

    
int isLink(char* path)
{
	 struct stat path_stat;
	 lstat(path, &path_stat);
	 
	 return S_ISLNK(path_stat.st_mode);
}	 

int isRegular(char* path)
{
	struct stat path_stat;
    stat(path, &path_stat);
    
    return S_ISREG(path_stat.st_mode);
}