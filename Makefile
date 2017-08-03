IDIR =include
CC=gcc
CFLAGS=-Wall -pedantic -march=core2 -I$(IDIR) -std=c++14

SRCDIR=src
ODIR=obj
LDIR =lib

LIBS=-lm

#_DEPS = hellomake.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = hellomake.o hellofunc.o
_OBJ = test.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
