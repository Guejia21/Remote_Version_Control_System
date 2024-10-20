all: rversions.o rversionsd.o protocol.o sha256.o
	gcc -o server server.o protocol.o
	gcc -o client client.c protocol.o
%.o: %.c
	gcc -c -o $@ $<
clean:
	rm -rf *.o rversions rversionsd docs
clean-repo:
	rm -rf .versions
doc:
	doxygen