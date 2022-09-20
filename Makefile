build:
	gcc -shared ainet.c -o libainet.so -g3 -fPIC

clean:
	rm libainet.so
