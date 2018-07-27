IDIR = include

SDIR = src
ODIR = obj
LDIR = lib

#CC=clang++-4.0
#CC=g++
CC=g++-8

V = 0
ACTUAL_CC := $(CC)
CC_0 = @echo "Compiling $^ -> $@"; $(ACTUAL_CC)
CC_1 = $(ACTUAL_CC)
CC = $(CC_$(V))

#-v
CFLAGS=  -g -std=c++17  -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option -I$(IDIR)


DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CPP_FILES1 := $(wildcard $(SDIR)/*.cpp)

OBJ_FILES1 := $(addprefix $(ODIR)/,$(notdir $(CPP_FILES1:.cpp=.o)))


MKDIR_P = mkdir -p

.PHONY: directories

all: directories test

directories: ${ODIR}

${ODIR}:
	${MKDIR_P} ${ODIR}

test: $(OBJ_FILES1) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
