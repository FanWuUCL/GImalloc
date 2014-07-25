all: 
	gcc `pkg-config --cflags glib-2.0` memory.c -o memory `pkg-config --libs glib-2.0`
	mv memory ../bin/
clean: 
	rm ../bin/memory
