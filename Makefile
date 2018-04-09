all:
	gcc -Wall -Werror -fsanitize=address learn.c -o learn

clean:
	rm -rf learn
