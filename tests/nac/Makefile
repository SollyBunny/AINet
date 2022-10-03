NAME="nac"

release: build run

debug: debugbuild debugrun

libainet.so:
ainet.h:

build: *
	gcc main.c -o $(NAME) -L. -lainet -I../.. -O3 -s

debugbuild: *
	gcc main.c -o $(NAME) -L. -lainet -I../.. -Wall -g 

run: *
	LD_LIBRARY_PATH=. ./$(NAME)

debugrun: *
	rm vgcore* -f
	LD_LIBRARY_PATH=. valgrind ./$(NAME)

clean:
	rm vgcore* -f
	rm out.svg -f
	rm out.net -f

link:
	rm ainet.h libainet.so -f
	ln -s "`pwd`/../../ainet.h" "`pwd`/ainet.h"
	ln -s "`pwd`/../../libainet.so" "`pwd`/libainet.so"
