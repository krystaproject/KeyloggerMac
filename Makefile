logger: logger.c keycodeToString.c
	gcc -Wall -o logger logger.c keycodeToString.c -I logger.h -framework ApplicationServices