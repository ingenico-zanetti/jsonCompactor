compactor: compactor.c
	$(CC) -o compactor compactor.c

install:compactor
	sudo cp compactor /usr/local/bin

