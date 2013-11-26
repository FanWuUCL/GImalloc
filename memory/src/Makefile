all: 
	gcc `pkg-config --cflags glib-2.0` profiler.c profiler_main.c public.c utility.c -o memory `pkg-config --libs glib-2.0`
	mv memory ../bin/
clean: 
	rm ../bin/memory
