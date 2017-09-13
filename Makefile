IDIR = include
SDIR = src
ODIR = obj
LDIR = lib

#CC=clang++-4.0
CC=g++
#CC=g++-7

V = 0
ACTUAL_CC := $(CC)
CC_0 = @echo "Compiling $^ -> $@"; $(ACTUAL_CC)
CC_1 = $(ACTUAL_CC)
CC = $(CC_$(V))


#CFLAGS=-Wall -Wextra -Werror -Wuninitialized -pedantic -std=c++14 -I$(IDIR)

#CFLAGS= -g -std=c++14 -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option -I$(IDIR)
ifeq ($(CC),g++)
	CFLAGS= -g -std=c++14 -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option -I$(IDIR)
else
	CFLAGS= -g -std=c++14 -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option -I$(IDIR)
endif
#CFLAGS= -g -std=c++14 -Wall -fexceptions -fPIC

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
