all:
	gcc -shared -fPIC malloc.c -o libmalloc.so
	gcc space.c -o space -lm -lmalloc -L. -Wl,-rpath=. -Wl,-rpath=..
