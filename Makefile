all: rversions.o rversionsd.o server.o client.o protocol.o sha256.o
	gcc -o rversions rversions.o client.o protocol.o sha256.o
	gcc -o rversionsd rversionsd.o server.c protocol.o sha256.o
%.o: %.c
	gcc -c -o $@ $<
clean:
	rm -rf *.o rversions rversionsd docs
clean-repo:
	rm -rf .versions
doc:
	doxygen