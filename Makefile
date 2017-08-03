IDIR = include
SDIR = src
ODIR = obj
LDIR = lib

CC=g++
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++14 -I$(IDIR)


LIBS=-lm -lpthread

#_DEPS = hellomake.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = hellomake.o hellofunc.o
_OBJ = test.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


MKDIR_P = mkdir -p

.PHONY: directories

all: directories test

directories: ${ODIR}

${ODIR}:
	${MKDIR_P} ${ODIR}



$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
