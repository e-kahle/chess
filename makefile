SOURCES=$(wildcard *.c)
NAME=ttce1tt
all:
	gcc $(SOURCES) -o $(NAME) -O1