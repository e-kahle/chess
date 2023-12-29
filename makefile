SOURCES=$(wildcard *.c)
NAME=ttce1p2
all:
	gcc $(SOURCES) -o $(NAME) -pthread -O1