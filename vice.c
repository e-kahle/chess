#include "defs.h"
#include <stdio.h>
#include "stdlib.h"
#define FENTEST "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 2 1 "
#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define PERFT "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
void ShowSqAtBySide(const int side, const S_BOARD* pos){
    int rank = 0;
    int file = 0;
    int sq = 0;
    printf("\n \n Squares attacked by: %c\n", SideChar[side]);
    for(rank = RANK_8; rank >= RANK_1; --rank){
        for(file = FILE_A; file <= FILE_H; ++file){
            sq = FR2SQ(file, rank);
            if(SqAttacked(sq, side, pos) == TRUE){
                printf("X");
            }else{
                printf("-");
            }

        }
        printf("\n");
    }
    printf("\n\n");
}
void PrintBin(int move){
    int index = 0;
    printf("As binary:\n");
    for(index = 27; index >= 0; index--){
        if((1<<index) & move) printf("1");
        else printf("0");
        if(index != 28 && index %4 ==0) printf(" ");
    }
    N(2);
}
int main(){
    AllInit();
    S_BOARD board[1];
    // ParseFen(START_FEN, board);
    // PrintBoard(board);
    // ParseFen(FEN1, board);
    // PrintBoard(board);
    // ParseFen(FEN2, board);
    // PrintBoard(board);
    // ParseFen(FEN3, board);
    // PrintBoard(board);
    ParseFen(PERFT, board);
    PrintBoard(board);
    
   ASSERT(CheckBoard(board));
   int move = 0;
   int from = A2; int to = H7;
   int cap = wR; int prom = bK;
   move = ((from) | (to << 7) | (cap << 14) | (prom << 20));

//    printf("n dec: %d hex: %X\n", move, move);
//    PrintBin(move);
   printf("from:%d to:%d cap:%d prom:%d\n", FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move));
    printf("Algebraic from: %s \n", SqStr(from));
    printf("Algebraic to: %s \n", SqStr(to));
    printf("Algebraic move: %s \n", PrMove(move));

//    printf("is PST: %s\n", (move & MFLAGPS)? "Yes" : "No");
    // ShowSqAtBySide(BLACK, board);
    // ShowSqAtBySide(WHITE, board);
    // printf("\nwP:\n");
    // PrintBitBoard(board->pawns[WHITE]);
    // printf("\nbP:\n");
    // PrintBitBoard(board->pawns[BLACK]);
    // printf("\nall P:\n");
    // PrintBitBoard(board->pawns[BOTH]);

    // int PieceOne = rand();
    // int PieceTwo = rand();
    // int PieceThree = rand();
    // int PieceFour = rand();
    // printf("PieceOne: %X\n", PieceOne);
    // printf("PieceTwo: %X\n", PieceTwo);
    // printf("PieceThree: %X\n", PieceThree);
    // printf("PieceFour: %X\n", PieceFour);
    // int Key = PieceOne ^ PieceTwo ^ PieceThree ^ PieceFour;
    // int TempKey = PieceTwo;
    // TempKey ^= PieceThree;
    // TempKey ^= PieceFour;
    // TempKey ^= PieceOne;

    // printf("Key: %X \n", Key);
    // printf("TempKey: %X \n", TempKey);
    // TempKey ^= PieceThree;
    // printf("(Three Out) TempKey: %X \n", TempKey);
    // TempKey ^= PieceThree;
    // printf("(Three Back In) TempKey: %X \n", TempKey);


    // int index;
    // for( index = 0; index < BRD_SQ_NUM; ++index){
    //     if(index%10 == 0) printf("\n");
    //     printf("%5d", Sq120ToSq64[index]);
    // }
    // printf("\n");
    // printf("\n");
    // for(index = 0; index < 64; ++index){
    //     if (index%8 == 0) printf("\n");
    //     printf("%5d", Sq64ToSq120[index]);
    // }
    // printf("\n");
    // printf("\n");
    // U64 playBitBoard = 0ULL;
    // printf("Start: \n\n");
    // PrintBitBoard(playBitBoard);
    // playBitBoard |= (1ULL << SQ64(B2));
    // printf("Added B2 \n");
    // PrintBitBoard(playBitBoard);
    // playBitBoard |= (1ULL << SQ64(G2));
    // playBitBoard |= (1ULL << SQ64(G8));
    // PrintBitBoard(playBitBoard);
    // int num = 2;
    // int nuts = 4;
    // ASSERT(num == nuts);
    // U64 playBitBoard = 0ULL;
    // playBitBoard |= (1ULL << SQ64(D2));
    // playBitBoard |= (1ULL << SQ64(D3));
    // playBitBoard |= (1ULL << SQ64(D4));
    // PrintBitBoard(playBitBoard);
    // int count = CNT(playBitBoard);
    // printf("COUNT: %d \n", count);
    // int index = POP(&playBitBoard);
    // printf("index: %d \n", index);
    // PrintBitBoard(playBitBoard);
    // count = CNT(playBitBoard);
    // printf("COUNT: %d \n", count);
    // int sq64 = 0;
    // while(playBitBoard){
    //     sq64 = POP(&playBitBoard);
    //     printf("popped: %d \n", sq64);
    //     PrintBitBoard(playBitBoard);
    // }
    // for(int index = 0; index < 64; index++){
    //     printf("INDEX: %d \n", index);
    //    PrintBitBoard(SetMask[index]);
    //    printf("\n");
    // }
    // SETBIT(playBitBoard, 61);
    // PrintBitBoard(playBitBoard);
    // CLRBIT(playBitBoard, 61);
    // PrintBitBoard(playBitBoard);
    return 0;
}