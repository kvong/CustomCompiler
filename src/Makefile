IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR) -w

ODIR=.obj
TEMP= .temp .filter1 *.asm# all temp files

_DEPS = cnode.h cqueue.h stack.h codeGen.h staticSem.h squeuenode.h squeue.h parsenode.h tnode.h tqueue.h testScanner.h dstring.h scanner.h token.h parser.h testTree.h# all .h files
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = cnode.o cqueue.o stack.o codeGen.o staticSem.o squeuenode.o squeue.o main.o parsenode.o tnode.o tqueue.o testScanner.o dstring.o scanner.o token.o parser.o testTree.o# all .o files
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

TARGET = compiler
VM = vm

all: $(TARGET) $(VM)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(VM):
	$(CC) -o virMach.c $(VM)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(TARGET)  $(TEMP)
