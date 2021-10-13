all:
	rm -f out*
	gcc -g main.c -lcurl
