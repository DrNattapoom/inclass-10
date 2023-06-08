CC=gcc
CFLAGS=-std=gnu17
TARGETS=echo_server # send_hello

all: $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

echo_server: echo_server.o pcsa_net.o
	$(CC) -o $@ $^

# send_hello: send_hello.o pcsa_net.o
#	$(CC) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
