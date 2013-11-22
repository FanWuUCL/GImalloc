
all:
	gcc -shared -fPIC malloc.c -o libmalloc.so
	gcc fat_memory.c -lmalloc -L. -Wl,-rpath=. -o fat_memory
	chmod +x RGenerator.sh
	./RGenerator.sh
