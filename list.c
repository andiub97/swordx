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


typedef struct List{ 
	char* parola; 
	int occorrenza;
	struct List* next;
}List; 
 
typedef struct Parlog{
	char* name;
	int cw;
	int iw;
	double time;
	struct Parlog* next;
}Parlog;

static int recursive_flag;
static int follow_flag;
static int alpha_flag;
static int sort_flag;

List* createListElement( char* , List* );
void createListElement2(char* , List* );
Parlog* createLogFile1();
Parlog* createLogFile(char*, int, int, Parlog*);
void incrementOcc(List* );
void contains( List* , char* );
void UpdateListLog (char* , List* );
void lettura (char*, List*);
void StampaFile ( char*, List* );
void printHelp();
void checkName(char*, List*  );
int isDirectory(char*);
int isRegular(char*);
int isLink(char*);
int fileInDir(char*, List* );
List* controlloStringhe(List*, char[], int, int, char**);
int isIgnored (char*, char**);
void readIgnore();
void sort();
int contaNodi(List*);
int counter(List*);
void updateFile( List*, char*);
void writeLog();
List* confronto(char* , List*);
//int nOcc(List*);
int count [2] = {0,0};
//void verificaFile( List*, List*);


static char* wordToIgnore= NULL;

static char *fileToExclude;
static int numMin = 0;
static char **wordsToIgnore = NULL;
static char *logFile= NULL;
static Parlog* firstLogFile;
static char* updateFileOp;
char* outputFile = "swordx.out" ;



Parlog* createLogFile1(){
	Parlog* n = malloc(sizeof(Parlog));
	n -> name = NULL;
	n -> cw = 0;
	n -> iw = 0;
	n -> next = NULL;
	return n;
}

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
	return n;
	}
}




List* createListElement( char* str, List* l) {
  List* newList = malloc(sizeof(List));
		newList->parola = NULL;
		newList-> next = NULL;
	return newList;
}


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
	
	//printf("\n %s \n", newList -> parola);
}


void incrementOcc(List* l){
	l->occorrenza += 1;
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

// int nOcc (List* l){
// 	int nOccorrenze = 0;
// 	List* app =  l->next;
// 	while(app != NULL){
// 		nOccorrenze += app->occorrenza;
// 		app = app->next;
// 	}
// 	return nOccorrenze;
// } 

void lettura (char* file, List* list){
	char str[100];
	FILE *fd;
	fd = fopen(file, "r");	
	if( fd==NULL )
	{
		perror("Errore in apertura del file");
		exit(1);
	}
			while (fscanf (fd, "%s", str) != EOF ){
				controlloStringhe(list, str, alpha_flag, numMin, wordsToIgnore);
				counter(list);
				//printf("%d", lenght(wordsToIgnore));
				//printf("%d", nOcc(list));

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
		if(strlen(str)>numMin){
			if((wordsToIgnore!=NULL) && isIgnored(str, wordsToIgnore)){
				return NULL;
			}else{ contains(list,str); return list;}
		}else count[1] += 1; count[0] -= 1; return NULL;
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








//void verificaFile( List* list, List* app){
	//List* app1= list->next;
	//while(app1 != NULL){
		//app1=app1->next;
		//printf("%s", app1->parola);
		//int cmp = strcmp(app->parola, app1->parola);
			//if(cmp != 0){
				//app1=app1->next;
				//printf("%s", app->parola);
			//}else{
				////printf("%s", app->parola);
				//int occ = app->occorrenza;
				//printf("%s", app->parola);
				//int occ1 = app1->occorrenza;
				//int occ2=occ+occ1;
				//app1=NULL;
				//app->occorrenza = occ2;
				//app1=app1->next;
				////verificaFile(list);
			//}
	//}
//}
 




int main(int argc, char* argv[]){
	int c;
	char** files;
	List* l = createListElement(NULL, NULL);
	Parlog* firstLogFile = createLogFile1();
	
	
	while (1)
	{
		  static struct option long_options[] =
		  {
			  ///* These options set a flag. */
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
				  
				  printf ("option %s", long_options[option_index].name);
				  
				  if (optarg)
					printf (" with arg %s", optarg);
				
				  printf ("\n");
				  break;
			
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
		  			updateFileOp = optarg;
					break;
		  case '?':
					//getopt_long already printed an error message.
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
		printf ("input files: \n ");
		
		while (optind < argc)
		{			
			files[z] = argv[optind];
			optind++;
			printf ("%s \n", files[z]);
			z++;
		}
		for(int i=0; i<z;i++)
		{
			checkName(files[i], l);

		}
		free(files);
		if(updateFileOp==NULL){
		StampaFile(outputFile, l); 
		} else { updateFile(l, outputFile); }
		sort();
		//printfn("%d", count);
		if (logFile != NULL) 
		{ 
			writeLog();
		}
		else("There aren't input files");
		exit (0);
	}

}


void checkName(char* filename, List* l){
	///* se NON è una directory fa l'update della lista passa a updateList il file, se no
	 //*  apre la directory, controlla i file e fa l'updateList su ognuno di loro*/
		if(isDirectory(filename) == 0){
				if(logFile == NULL){
					lettura(filename, l);
				}else{ UpdateListLog(filename, l);
				}
		}else{
			fileInDir(filename, l);
			
		}
}




int counter(List* l){
	if (l->next != NULL){
		count[0] += 1;
	}else{count[1] += 1;}
}


int contaNodi(List*l){
	int c = 0;
	List* app = l->next;
	while(app != NULL){
		c++;
		app= app->next;
	}
	return c;
}
		

void sort()// -s -o fileOut test test1 ...
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


  void writeLog(){
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
		fprintf(fp, "%f\n ", app -> time);
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
			if((follow_flag==1) && isLink(filename)){
				if(logFile == NULL) {lettura(filename,l);} else UpdateListLog(filename,l); 
			}
			if(recursive_flag==1){
				if(!isLink(filename)){
					if(isDirectory(filename)) fileInDir(filename,l);
				}
			}
			if(!isLink(filename) && !isDirectory(filename)){
				if(logFile == NULL) {lettura(filename,l);} else UpdateListLog(filename,l); 
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



void updateFile( List* l, char* outputFile){
	char str [100];
	FILE* fp = fopen(outputFile,"r");
	if (fp == NULL){
		perror("errore in apertura del file");
	}
	while (fscanf (fp, "%s", str) != EOF ){
		//fare funzione che controlla se la stringa str è nella lista l
		// e fare variazioni
		confronto(str, l);
		//printf("%s", str);
		//printf("%s", "+");
		//printf("%d",up );
	}
	printf("\n");
		FILE* fp1 = fopen(outputFile,"w");
		List* app = l -> next;
		while(app != NULL){
			fprintf( fp1, "%s", app->parola);
			fprintf( fp1," ");
			fprintf( fp1,"%d", app->occorrenza);
			fprintf(fp1,"\n");
			app=(app->next);
		}
}
//non legge app-> next
List* confronto(char* str, List* l){
	printf("%s", str);
	List* app = l;
	while (l->next != NULL){
	printf("%s", l->parola);
	l=l->next;
	}
	return app;
/* 	printf("%s", l->parola);
	List* app = l;
 	while(app->next!= NULL){
		int i = strcmp(str, app->parola);
		if(i==1){
			printf ("%s" , "+");
			printf ("%d" , app->occorrenza);
		}else{
		}
	} */
}




 
void readIgnore(){
	wordsToIgnore = malloc(sizeof(char**));
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
			wordsToIgnore = (char**) realloc(wordsToIgnore, sizeof(char*)*i+1); 
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
	printf("\t\t -update <file> : if swordx.out (or another output file specified) is present, write on file <file>,\n");
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

int isRegular(char* path)
{
	struct stat path_stat;
    stat(path, &path_stat);
    
    return S_ISREG(path_stat.st_mode);
}
    
int isLink(char* path)
{
	 struct stat path_stat;
	 lstat(path, &path_stat);
	 
	 return S_ISLNK(path_stat.st_mode);
}	 
