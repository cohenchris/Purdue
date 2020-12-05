all: daytime-server tls-server use-dlopen hello.so

daytime-server: daytime-server.c
	$(CC) -o $@ $^

tls-server: tls-server.c
	$(CC) `pkg-config --cflags openssl` -o $@ $^ `pkg-config --libs openssl`
	@echo "Don't forget to create cert.pem and key.pem"

use-dlopen: use-dlopen.c
	$(CC) -o $@ $^ -ldl

hello.so: hello.c
	$(CC) -c -fPIC $^
	ld -G -o hello.so hello.o

clean:
	rm -f tls-server daytime-server use-dlopen hello.so hello.o
