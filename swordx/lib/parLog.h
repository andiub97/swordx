typedef struct Parlog{
	char* name;
	int cw;
	int iw;
	double time;
	struct Parlog* next;
}Parlog;


Parlog* createLogFile1();
Parlog* createLogFile(char*, int, int, Parlog*);
