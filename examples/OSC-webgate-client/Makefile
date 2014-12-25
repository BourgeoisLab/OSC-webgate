#-----------------------------------------------------
# Makefile to build OSC-webgate-client on Raspberry Pi
#-----------------------------------------------------

SRC=./
OUT=OSC-webgate-client
SYMBOLS=-DLINUX

CC=${CC_PATH}gcc
AS=${CC_PATH}as

CFLAGS=$(SYMBOLS) -O3 -Wall -fmessage-length=0

LIBS=
LIBDIR=
LDFLAGS=

OBJ=$(SRC)OSC-webgate-client.o \
$(SRC)OSC-webgate-client-api.o \
$(SRC)HTTPClient/HTTPClient.o \
$(SRC)HTTPClient/HTTPClientAuth.o \
$(SRC)HTTPClient/HTTPClientString.o \
$(SRC)HTTPClient/HTTPClientWrapper.o

all: $(OUT)

$(OUT): $(OBJ)
	$(CC)  $(LIBDIR) $(LDFLAGS) $(OBJ) -o $(OUT) $(LIBS)

.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUT) *.o $(SRC)*.o $(SRC)HTTPClient/*.o *.map *.gdb
