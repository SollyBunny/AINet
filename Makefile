build:
	gcc -shared ainet.c -o libainet.so -g3 -fPIC

clean:
	rm libainet.so

install:
	sudo rm -f /usr/lib/libainet.so
	sudo ln -s "`pwd`/libainet.so" /usr/lib/libainet.so
	
