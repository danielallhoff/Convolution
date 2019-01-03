CC=gcc
CFLAGS=-I --help=target.

LIBS= -lm
NAME= convolution
SRC = convolution.c
ODIR = conv
OBJ = conv.o
ARGS = 1 7 1 4000

conv.o: $(SRC)
	$(CC) -o $(NAME) $(SRC) $(CFLAGS) $(LIBS)

conv: $(OBJ)
	$(CC) -o $(OBJ) $(CFLAGS) $(LIBS)

run: conv.o
	./$(NAME) $(ARGS)

.IPHONY: clean

clean:
	rm -f *.o $(NAME)
