MOCDIR	=
OBJDIR	= .

INCLUDE =
MODFLAGS =
CFLAGS =	-c -O -I/home/jason/kernel/linux/include
#-c -O3 -W -Wall -Wstrict-prototypes -Wmissing-prototypes

HEADERS =
SOURCES =	\
	./socket_shift_kernel_module.c
UI	=

all: socket_shift_kernel_module.o

socket_shift_kernel_module.o:	socket_shift_kernel_module.c
	gcc-2.95 $(INCLUDE) $(MODFLAGS) $(CFLAGS) socket_shift_kernel_module.c

clean:
	rm *.o;
