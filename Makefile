#------------------------------------------------
# Makefile to build OSC-webgate on Raspberry Pi
#------------------------------------------------

SRC=./src/
OUT=OSC-webgate
SYMBOLS=-DLINUX

CC=${CC_PATH}gcc
AS=${CC_PATH}as

CFLAGS=$(SYMBOLS) -O3 -Wall -fmessage-length=0

LIBS=-lpthread
LIBDIR=
LDFLAGS=

OBJ=$(SRC)main.o \
$(SRC)datapool.o \
$(SRC)datapoolpredef.o \
$(SRC)datapooluser.o \
$(SRC)cgi.o \
$(SRC)cgi_json.o \
$(SRC)mongoose.o \
$(SRC)osc.o \
$(SRC)OSC-client.o \
$(SRC)OSC-timetag.o \
$(SRC)ujsonpars.o \
$(SRC)utils.o

all: $(OUT)

$(OUT): $(OBJ)
	$(CC)  $(LIBDIR) $(LDFLAGS) $(OBJ) -o $(OUT) $(LIBS)

.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUT) *.o $(SRC)*.o *.map *.gdb
