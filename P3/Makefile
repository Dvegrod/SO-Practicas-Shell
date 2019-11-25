CC=gcc
CFLAGS=-Wall -g

shell: shell.c dynlist.o p0commands.o p1commands.o p2commands.o p3commands.o exinf.h
	$(CC) -o shell shell.c dynlist.o p0commands.o p1commands.o p2commands.o p3commands.o $(CFLAGS)

dynlist.o: dynlist.c dynlist.h exinf.h
	$(CC) -c dynlist.c $(CFLAGS)

p0commands.o: p0commands.c p0commands.h exinf.h
	$(CC) -c p0commands.c $(CFLAGS)

p1commands.o: p1commands.c p1commands.h exinf.h
	$(CC) -c p1commands.c $(CFLAGS)

p2commands.o: p2commands.c p2commands.h exinf.h
	$(CC) -c p2commands.c $(CFLAGS)

p3commands.o: p3commands.c p3commands.h exinf.h
	$(CC) -c p3commands.c $(CFLAGS)

clean:
	rm *.o

clean_all:
	rm shell
	rm *.o
