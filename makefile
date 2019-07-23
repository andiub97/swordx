# compiler
CC =	gcc

# set obj folder
OBJD =	obj

# set lib folder
LIBD =	lib 

# -Wall for debug -I for the location of header files 
CFLAGS =	-Wall -I$(LIBD)


OBJ =$(OBJD)/swordx.o \
		$(OBJD)/list.o \
		$(OBJD)/parLog.o \
                                                                       
vpath %.c src
vpath %.h $(LIBD)

all:	swordx 

swordx: $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@ 

$(OBJD)/%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $< 

$(OBJD)/list.o: list.c list.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJD)/parLog.o: parLog.c parLog.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJD)/swordx.o: swordx.c list.h parLog.h
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY:
	clean
clean:
	rm swordx obj/*.o




