SOURCES=$(wildcard *.c)
NAME=ttce1tt5
all:
	gcc $(SOURCES) -o $(NAME) -pthread -O1