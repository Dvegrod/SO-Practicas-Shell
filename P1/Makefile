CC=gcc
CFLAGS=-Wall

shell: shell.c dynlist.o p0commands.o p1commands.o exinf.h
	$(CC) -o shell shell.c dynlist.o p0commands.o p1commands.o $(CFLAGS)

dynlist.o: dynlist.h dynlist.c
	$(CC) -c dynlist.h dynlist.c $(CFLAGS)

p0commands.o: p0commands.h p0commands.c
	$(CC) -c p0commands.h p0commands.c $(CFLAGS)

p1commands.o: p1commands.h p1commands.h
	$(CC) -c p1commands.h p1commands.c $(CFLAGS)

clean:
	rm *.h.gch