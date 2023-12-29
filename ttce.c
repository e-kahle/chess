#include "defs.h"
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "math.h"
#define PAWNTST "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1 "
#define PAWNTSTB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1 "
#define FENTEST "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 2 1 "
#define KNIGHTTEST "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1 "
#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define PERFT "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/1N6/6K1 b - - 0 1 "
#define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1 "
#define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1 "
#define CASTLE "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1 "
#define PERFT2 "4k3/8/8/8/8/8/8/4K2R w K - 0 1 "
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define V48 "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1 "
#define PERFT3 "8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1 "
#define WAC1 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"
#define FINE_70 "8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - -"
#define WAC_ "8/7p/5k2/5p2/p1p2P2/Pr1pPK2/1P1R3P/8 b - -"
#define LCT_1 "r3kb1r/3n1pp1/p6p/2pPp2q/Pp2N3/3B2PP/1PQ2P2/R3K2R w KQkq -"
void PrintStats(int w, int l, int d){
    double m = (double)w + ((double)d/2);
    int n = w + l + d;
    m/= n;
    double s = (double)w*((double)1 - m)*((double)1-m) 
            + (double)d*((double)1/2 - m)*((double)1/2 - m)
            + (double)l * m*m;
    s/= n-1;
    N(1);
    printf("The sample mean is: %lf", m);
    N(1);
    printf("The sample variance is: %lf", s);
    N(1);
    printf("With 95%% confidence, we can say that the probability of Stinkfish against the tested engine(s) is:");
    N(1);
    printf("[%lf - %lf]", m - 1.96*(s/sqrt(n)), m+ 1.96*(s/sqrt(n)));
}
int main(int argc, char* argv[]){
    AllInit();
    //printf("hi");
    
    S_BOARD pos[1];
    //printf("PolyKey: %lld", PolyKeyFromBoard(pos));
    S_SEARCHINFO info[1];
    info->threadNum = 4;
    info->quit = FALSE;
    HashTable->pTable = NULL;
    InitHashTable(HashTable, 64);
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    // TempHashTest(PERFT);
    // TempHashTest(WAC1);
    // exit(0);
    int ArgNum = 0;
    for(ArgNum = 0; ArgNum < argc; ++ArgNum){
        if(strncmp(argv[ArgNum], "NoBook", 6) == 0){
            EngineOptions->UseBook = FALSE;
            printf("Book OFF\n");
        }
    }
    printf("Welcome to Thomas the Chess Engine (TTCE) 1.0! Type 'ttce' for console mode...\n");
    char line[256];
    while(TRUE){
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if(!fgets(line, 256, stdin)){
            continue;
        }
        if(line[0] == '\n') continue;
        if(!strncmp(line, "uci", 3)){
            Uci_Loop(pos, info);
            if(info->quit == TRUE) break;
            continue;
        }else if(!strncmp(line, "xboard", 6)){
            XBoard_Loop(pos, info);
            if(info->quit == TRUE) break;
            continue;
        }else if(!strncmp(line, "ttce", 4)){
            Console_Loop(pos, info);
            if(info->quit == TRUE) break;
            continue;
        }else if(!strncmp(line, "quit", 4)){
            break;
        }
        else if(!strncmp(line, "stats", 5)){
            int w, l, d;
            printf("Enter the number of wins (<=1000000)\n");
            scanf("%d", &w);
            printf("Enter the number of losses (<=1000000)\n");
            scanf("%d", &l);
            printf("Enter the number of draws (<=1000000)\n");
            scanf("%d", &d);
            PrintStats(w, l, d);
            N(1);
            continue;
        }
    }
    free(HashTable->pTable);
    CleanPolyBook();
    return 0;
}

