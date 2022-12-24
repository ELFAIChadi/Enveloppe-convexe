IDIR = include
ODIR = obj
SDIR = src/c
JDIR = src/main/java

CC = gcc
CFLAGS = -Wall -c -I$(IDIR)

LIB = $(JDIR)/libmylibrary.dylib

#Modifiez la variable JAVA_HOME avec le chemin d'installation Java de votre machine
JAVA_HOME = /Users/vincentle/Library/Java/JavaVirtualMachines/openjdk-18.0.1/Contents/Home
JINCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin

_DEPS = util.h application_MyClass.h algo.h geometry.h heap.h list.h sort.h tree.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = util.o algo.o geometry.o heap.o list.o sort.o tree.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(OBJ) $(ODIR)/application_MyClass.o
	$(CC) -dynamiclib -o $(LIB) $^

$(ODIR)/application_MyClass.o : $(SDIR)/application_MyClass.c $(DEPS)
	$(CC) $(CFLAGS) $(JINCLUDES) -o $@ $<

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(LIB)