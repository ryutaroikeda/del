CFLAGS=-Wall -pedantic -Wextra -Wshadow -g -O2 -DNDEBUG -Isrc $(OPTFLAGS)
SOURCE=$(wildcard src/*.c)
OBJECT=$(patsubst %.c,%.o,$(SOURCE))

TRASH=bin/trash
INCINERATE=bin/incinerate
TARGET=bin/trash
all: build $(TRASH) $(INCINERATE)
	
.PHONY: release dev clean build test install uninstall

dev: CFLAGS=-Wall -pedantic -Wextra -Wshadow -Werror -g -Isrc $(OPTFLAGS)
dev: all

release: all

$(TRASH): src/trash.c
	$(CC) $(CFLAGS) -o $@ $<

$(INCINERATE): src/incinerate.c
	$(CC) $(CFLAGS) -o $@ $<
	
build:
	@mkdir -p bin

clean:
	rm -rf $(OBJECT)

test/test: test/test.o
	$(CC) $(CFlAGS) -o test/test $<

test: test/test
	./test/test

install: $(TRASH) $(INCINERATE)
	mv $(TRASH)  ~/bin/trash
	mv $(INCINERATE) ~/bin/incinerate
	chmod u+x ~/bin/trash
	chmod u+x ~/bin/incinerate

uninstall:
	@rm ~/bin/trash
	@rm ~/bin/incinerate


