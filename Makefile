all: matcher
matcher: matcher.c
	gcc -O3 -Wall -o matcher matcher.c
