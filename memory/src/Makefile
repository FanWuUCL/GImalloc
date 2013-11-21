all: 
	gcc `pkg-config --cflags glib-2.0` *.c -o memory `pkg-config --libs glib-2.0`
	mkdir ../bin
	mv memory ../bin/
clean: 
	rm ../bin/memory
