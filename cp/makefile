CC=gcc
CARGS = -lpthread
SRC = $(wildcard *.c)

cp-cli: $(SRC)

	$(CC) $(CARGS) $^ -o $@
	mv $@ ..
