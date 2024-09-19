CC=gcc -g -Wall
INCLUDE=.


all:	rklog	

clean:
	rm -f *.o rklog

rklog.o:	rklog.c rklog.h types.h castles.h
		$(CC) -I $(INCLUDE) -c rklog.c

rklog:	rklog.o 
		$(CC) -I $(INCLUDE) -o rklog rklog.o
	       
       
#       -lefence

