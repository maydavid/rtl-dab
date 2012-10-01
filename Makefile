VERSION = 0.0
CC = gcc
CFLAGS = -Wall -g -DVERSION=\"$(VERSION)\"
LDFLAGS = -lrtlsdr 

SRC_DIR = src/
INC_DIR = include/

rtldab: rtldab.o
	$(CC) -o rtldab rtldab.o $(LDFLAGS)

rtldab.o: $(SRC_DIR)rtldab.c $(INC_DIR)rtldab.h
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $(SRC_DIR)rtldab.c

clean: 
	rm -rf *.o