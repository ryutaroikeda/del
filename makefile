CFLAGS=-Wall -pedantic -Wextra -Wshadow -g -O2 -DNDEBUG -Isrc $(OPTFLAGS)
SOURCE=$(wildcard src/*.c)
OBJECT=$(patsubst %.c,%.o,$(SOURCE))

TRASH=bin/trash
all: build $(TRASH) 
	
.PHONY: release dev clean build test install uninstall

dev: CFLAGS=-Wall -pedantic -Wextra -Wshadow -Werror -g -Isrc $(OPTFLAGS)
dev: all

release: all

$(TRASH): src/trash.c
	$(CC) $(CFLAGS) -o $@ $<

build:
	@mkdir -p bin

clean:
	rm -rf $(OBJECT)

test:
	test/test.sh

install: $(TRASH) 
	@cp $(TRASH)  ~/bin/trash
	@chmod u+x ~/bin/trash

uninstall:
	@rm ~/bin/trash

