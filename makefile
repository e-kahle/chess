all:
	gcc vice.c init.c bitboards.c perft.c hashkeys.c pvtable.c board.c data.c uci.c evaluate.c attack.c io.c movegen.c validate.c search.c misc.c makemove.c -o stinkfish  