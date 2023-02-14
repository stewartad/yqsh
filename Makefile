CC=gcc
CFLAGS=-Wall -g
TARGET_DIR=bin

yqsh: 
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $(TARGET_DIR)/yqsh yqsh.c

clean:
	rm yqsh