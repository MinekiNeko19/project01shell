all:	main.o	Chespin.o
	gcc	-o	main	main.o	Chespin.o
main.o:	main.c	Chespin.h
	gcc	-c	main.c
Chespin.o:	Chespin.c	Chespin.h
	gcc	-c	Chespin.c

run:
	./main
