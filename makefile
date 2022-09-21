CXX=g++
CFLAGS=-Wall -lX11
CDEBUGFLAGS= -g $(CFLAGS)

BIN=bin
SRC=src/unix
EXEC=test.exe

CFILES=$(wildcard $(SRC)/*.cpp)

all : $(BIN)
	$(CXX) -o $(BIN)/$(EXEC) $(CFILES) $(CFLAGS)

run : all
	./$(BIN)/$(EXEC)

$(BIN) : 
	mkdir $(BIN)
	touch $(BIN)/.gitignore
	echo "*" > $(BIN)/.gitignore

debug : $(BIN)
	$(CXX) -o $(BIN)/$(EXEC) $(CFILES) $(CDEBUGFLAGS)

.PHONY: clean

clean :
	rm -rd $(BIN)
