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
    info->quit = FALSE;
    pos->HashTable->pTable = NULL;
    InitHashTable(pos->HashTable, 64);
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    int ArgNum = 0;
    for(ArgNum = 0; ArgNum < argc; ++ArgNum){
        if(strncmp(argv[ArgNum], "NoBook", 6) == 0){
            EngineOptions->UseBook = FALSE;
            printf("Book OFF\n");
        }
    }
    printf("Welcome to Stinkfish! Type 'stinkfish' for console mode...\n");
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
        }else if(!strncmp(line, "stinkfish", 9)){
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
    free(pos->HashTable->pTable);
    CleanPolyBook();
    return 0;
    // S_BOARD board[1];
    // S_MOVELIST list[1];
    // S_SEARCHINFO info[1];
    // board->PvTable->pTable = NULL;
    // InitPvTable(board->PvTable);
    // ParseFen(WAC1, board);
    // // printf("eval: %d\n", EvalPosition(board));
    // // printf("mat white: %d \n", board->material[WHITE]);
    // //     printf("mat black: %d \n", board->material[BLACK]);

    // //PrintBoard(board);
    // //printf("HI");
    
    // //PerftTest(6, board);
    // char input[6];
    // int Move = NOMOVE;
    // int PvNum = 0;
    // int Max = 0;
    // while(TRUE){
    //     PrintBoard(board);
    //     printf("Please enter a move >");
    //     fgets(input, 6, stdin);
    //     if(input[0] == 'q'){
    //         break;
    //     } else if(input[0] == 't'){
    //         TakeMove(board);
    //     }else if(input[0] == 's'){
    //         info->depth = 6;
    //         info->starttime = GetTimeMs();
    //         info->stoptime = GetTimeMs() + 200000;
    //         SearchPosition(board, info);
    //         // if(input[1] < '1' || input[1] > '9') PerftTest(4, board);
    //         // else PerftTest(input[1]-'0', board);
    //         // Max = GetPvLine(4, board);

    //         // printf("PvLine of %d Moves: ", Max);
    //         // for(PvNum = 0; PvNum < Max; ++PvNum){
    //         //     Move = board->PvArray[PvNum];
    //         //     printf(" %s", PrMove(Move));
    //         // }
    //         // N(1);
    //     }
    //     else{
    //         Move = ParseMove(input, board);
    //         if(Move != NOMOVE){
    //             StorePvMove(board, Move);
    //             MakeMove(board, Move);
    //             // if(IsRepetition(board)){
    //             //     printf("REP SEEN\n");
    //             // }
    //         }else{
    //             printf("Move Not Parsed: %s\n", input);
    //         }
    //     }
    //     fflush(stdin);
    // }
    // free(board->PvTable->pTable);

    // GenerateAllMoves(board, list);
    // ASSERT(CheckBoard(board));
    // int MoveNum = 0;
    // int move = 0;
    // for(MoveNum = 0; MoveNum < list->count; ++MoveNum){
    //     move = list->moves[MoveNum].move;
    //     printf("MOVENUM %d\n", MoveNum);
    //     if(!MakeMove(board, move)){
    //         continue;
    //     }

    //     printf("\nMADE: %s\n", PrMove(move));
    //     PrintBoard(board);
    //     TakeMove(board);
    //     printf("\nTAKEN: %s\n", PrMove(move));
    //     PrintBoard(board);

    //     getchar();
    // }
    // ParseFen(START_FEN, board);
    // PrintBoard(board);
    // ParseFen(FEN1, board);
    // PrintBoard(board);
    // ParseFen(FEN2, board);
    // PrintBoard(board);
    // ParseFen(FEN3, board);
    // PrintBoard(board);
    // ParseFen(PERFT, board);
    // PrintBoard(board);
    // S_MOVELIST list[1];
    // GenerateAllMoves(board, list);
    // PrintMoveList(list);
//    ASSERT(CheckBoard(board));
//    int move = 0;
//    int from = A2; int to = H7;
//    int cap = wR; int prom = bK;
//    move = ((from) | (to << 7) | (cap << 14) | (prom << 20));

// //    printf("n dec: %d hex: %X\n", move, move);
// //    PrintBin(move);
//    printf("from:%d to:%d cap:%d prom:%d\n", FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move));
//     printf("Algebraic from: %s \n", SqStr(from));
//     printf("Algebraic to: %s \n", SqStr(to));
//     printf("Algebraic move: %s \n", PrMove(move));

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

// void ShowSqAtBySide(const int side, const S_BOARD* pos){
//     int rank = 0;
//     int file = 0;
//     int sq = 0;

//     printf("\n \n Squares attacked by: %c\n", SideChar[side]);
//     for(rank = RANK_8; rank >= RANK_1; --rank){
//         for(file = FILE_A; file <= FILE_H; ++file){
//             sq = FR2SQ(file, rank);
//             if(SqAttacked(sq, side, pos) == TRUE){
//                 printf("X");
//             }else{
//                 printf("-");
//             }

//         }
//         printf("\n");
//     }
//     printf("\n\n");
// }
// void PrintBin(int move){
//     int index = 0;
//     printf("As binary:\n");
//     for(index = 27; index >= 0; index--){
//         if((1<<index) & move) printf("1");
//         else printf("0");
//         if(index != 28 && index %4 ==0) printf(" ");
//     }
//     N(2);
// }