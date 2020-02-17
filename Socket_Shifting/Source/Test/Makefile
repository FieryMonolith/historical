OBJDIR	= .

LIBS	= -lm
LIBPATH	= -L/usr/lib
#INCPATH	= -I. -I/usr/include -I/usr/include/sys
INCPATH	=
#ARGS	= -pg -fprofile-arcs -O3
#ARGS	= -O3

OBJECTS	= $(OBJDIR)/service.o $(OBJDIR)/tcp_server.o \
	$(OBJDIR)/tcp_client.o $(OBJDIR)/tcp_error.o

all: service

service:	$(OBJECTS)
	g++ $(ARGS) -o $@ $(LIBPATH) $(LIBS) $(OBJECTS)

$(OBJDIR)/service.o:	tcp_client.h tcp_server.h service.cpp tcp_shared.h
	g++ $(INCPATH) $(ARGS) -c -o $@ service.cpp

$(OBJDIR)/tcp_server.o:	tcp_server.h tcp_server.cpp tcp_error.h tcp_shared.h
	g++ $(INCPATH) $(ARGS) -c -o $@ tcp_server.cpp

$(OBJDIR)/tcp_client.o:	tcp_client.h tcp_client.cpp tcp_error.h tcp_shared.h
	g++ $(INCPATH) $(ARGS) -c -o $@ tcp_client.cpp

$(OBJDIR)/tcp_error.o:	tcp_error.h tcp_error.cpp tcp_shared.h
	g++ $(INCPATH) $(ARGS) -c -o $@ tcp_error.cpp

clean:
	rm *~; rm $(OBJDIR)/*.o; rm service;
