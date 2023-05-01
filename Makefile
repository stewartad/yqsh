CC           := gcc
CFLAGS       := -Wall -Wextra -g -Wno-unused-parameter -Wno-unused-function -Wdouble-promotion -Wconversion -Wno-sign-conversion -fsanitize=address -fsanitize=undefined

TARGETDIR    := bin
BUILDDIR     := obj
SRCDIR       := src
TESTDIR      := test

TARGET       := yqsh
OBJS         := $(BUILDDIR)/yqsh.o

$(TARGET): $(OBJS) $(SRCDIR)/main.c
	mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$@ $^

test: $(OBJS) $(TESTDIR)/test.c 
	mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$@ $^ -I$(SRCDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(BUILDDIR)
	$(CC) -c $(CFLAGS) $(SRCDIR)/$*.c -o $(BUILDDIR)/$*.o

.PHONY: clean

clean:
	rm -f $(BUILDDIR)/*.o $(TARGETDIR)/yqsh $(TARGETDIR)/test
