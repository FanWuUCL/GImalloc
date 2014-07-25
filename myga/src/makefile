GLIBFLAGS=`pkg-config --cflags glib-2.0`
GLIBLIBS=`pkg-config --libs glib-2.0`
SOURCES=main.c crossover.c mutation.c selection.c evaluation.c original.c public.c utility.c initialization.c
OBJECTS=main.o crossover.o mutation.o selection.o evaluation.o original.o public.o utility.o initialization.o
PRO_OBJECTS=profiler_main.o profiler.o utility.o public.o

all: myga memory clean

myga:	$(OBJECTS)
	gcc -o myga $(OBJECTS) -lm $(GLIBLIBS)

profiler: $(PRO_OBJECTS)
	gcc -o profiler $(PRO_OBJECTS) $(GLIBLIBS)

memory:	memory.o
	gcc -o memory memory.o $(GLIBLIBS)

%.o: %.c
	gcc -c $(GLIBFLAGS) $< -o $@

clean:
	rm -rf *.o
