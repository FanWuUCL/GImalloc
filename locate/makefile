GLIBFLAGS=`pkg-config --cflags glib-2.0`
GLIBLIBS=`pkg-config --libs glib-2.0`

all: locate

locate:	locate.c
	gcc -o locate $(GLIBFLAGS) locate.c `pkg-config --libs glib-2.0`


