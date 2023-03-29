CC=gcc
CFLAGS=-Wall -g
TARGET_DIR=bin

yqsh: yqsh.o main.c
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $(TARGET_DIR)/$@ $^

test: yqsh.o test.c
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $(TARGET_DIR)/$@ $^

yqsh.o: yqsh.c yqsh.h

clean:
	rm *.o
	rm $(TARGET_DIR)/yqsh
	rm $(TARGET_DIR)/test

