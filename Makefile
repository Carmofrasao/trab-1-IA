# Anderson Aparecido do Carmo Frasão (GRR20204069)

CFLAGS  = -Wall -g
CC = gcc 

#-----------------------------------------------------------------------------#
all : main

main: main.o

#-----------------------------------------------------------------------------#

clean :
	$(RM) *.o

#-----------------------------------------------------------------------------#

purge:
	$(RM) main *.o