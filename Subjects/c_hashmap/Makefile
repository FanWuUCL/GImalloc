hashmap: main.c hashmap.c
	gcc -shared -fPIC malloc.c -o libmalloc.so
	gcc main.c hashmap.c -o hashmap -lmalloc -L. -Wl,-rpath=. -Wl,-rpath=..
