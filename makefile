CFLAGS=-Wall -pedantic -Wextra -Wshadow -O2 -DNDEBUG -Isrc $(OPTFLAGS)
SOURCE=$(wildcard src/*.c)
OBJECT=$(patsubst %.c,%.o,$(SOURCE))

TRASH=bin/del
all: build $(TRASH) 
	
.PHONY: release dev clean build test install uninstall 

dev: CFLAGS=-Wall -pedantic -Wextra -Wshadow -Werror -g -Isrc $(OPTFLAGS)
dev: all

release: all

$(TRASH): src/del.c
	$(CC) $(CFLAGS) -o $@ $<

build:
	@mkdir -p bin

clean:
	rm -rf $(OBJECT)

install: $(TRASH) 
	@cp $(TRASH)  ~/$(TRASH)
	@chmod u+x ~/$(TRASH)

uninstall:
	@rm ~/$(TRASH)

test: all install
	test/test.sh
