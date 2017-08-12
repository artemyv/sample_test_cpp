IDIR = include
SDIR = src
ODIR = obj
LDIR = lib

CC=g++

V = 0
ACTUAL_CC := $(CC)
CC_0 = @echo "Compiling $^ -> $@"; $(ACTUAL_CC)
CC_1 = $(ACTUAL_CC)
CC = $(CC_$(V))


CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++14 -I$(IDIR)


LIBS=-lm -lpthread

#_DEPS = hellomake.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = hellomake.o hellofunc.o
CPP_FILES := $(wildcard $(SDIR)/*.cpp)
OBJ_FILES := $(addprefix $(ODIR)/,$(notdir $(CPP_FILES:.cpp=.o)))


MKDIR_P = mkdir -p

.PHONY: directories

all: directories test

directories: ${ODIR}

${ODIR}:
	${MKDIR_P} ${ODIR}


test: $(OBJ_FILES)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
