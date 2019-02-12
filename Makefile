.PHONY: all clean

LDLIBS = -llua

all: luahup.so runhup

clean:
	rm -f *.o luahup.so runhup

stdhup.o: stdhup.c

stdhup-pic.o: stdhup.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $^

luahup.o: luahup.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $^

luahup.so: luahup.o stdhup-pic.o
	$(CC) $(LDFLAGS) -shared -o $@  $^ $(LDLIBS)

runhup.o: runhup.c
runhup: runhup.o stdhup.o
