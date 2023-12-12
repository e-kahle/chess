all:
	gcc vice.c init.c bitboards.c perft.c hashkeys.c board.c data.c attack.c io.c movegen.c validate.c makemove.c -o vice