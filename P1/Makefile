CC=gcc
CFLAGS=-Wall

all: shell info listar

shell: shell.c dynlist.o p0commands.o p1commands.o exinf.h
	$(CC) -o shell shell.c dynlist.o p0commands.o p1commands.o $(CFLAGS)

info: info.c p1commands.o exinf.h
	$(CC) -o info info.c p1commands.o $(CFLAGS)

listar: listar.c p1commands.o exinf.h
	$(CC) -o listar listar.c p1commands.o $(CFLAGS)

dynlist.o: dynlist.c dynlist.h exinf.h
	$(CC) -c dynlist.c $(CFLAGS)

p0commands.o: p0commands.c p0commands.h exinf.h
	$(CC) -c p0commands.c $(CFLAGS)

p1commands.o: p1commands.c p1commands.h exinf.h
	$(CC) -c p1commands.c $(CFLAGS)

clean:
	rm *.h.gch

clean_obj:
	rm *.o
	rm *.h.gch

clean_all:
	rm shell
	rm listar
	rm info
	rm *.o
	rm *.h.gch
