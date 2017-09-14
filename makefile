
_OBJ = \
CommandReadBuffer.o \
IRUartCommand.o \
test_sdk_uart.o 

IDIR =./
CC=g++
CFLAGS=-I$(IDIR)
CXXFLAGS=-std=c++11

ODIR=obj
LDIR =../lib

LIBS=-lcurl -lpthread -lstdc++


OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(CXXFLAGS)

test_sdk_uart: $(OBJ) *.a
	$(CC) -o $@ $^ $(LIBS) $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o test_sdk_uart
