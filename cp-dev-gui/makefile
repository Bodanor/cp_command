CC=gcc
SRC = $(wildcard *.c)

cp-gui: $(SRC)

	$(CC) `pkg-config --cflags gtk+-3.0` $^ -o $@ `pkg-config --libs gtk+-3.0`
	mv $@ ..
