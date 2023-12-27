#include "defs.h"
#include "polykeys.h"

typedef struct {
    U64 key;
    unsigned short move;
    unsigned short weight;
    unsigned int learn;
} S_POLY_BOOK_ENTRY;

long NumEntries = 0;

S_POLY_BOOK_ENTRY* entries;

//convert custom piece to poly piece
const int PolyKindOfPiece[13] = {
    -1, 1, 3, 5, 7, 9, 11, 0, 2, 4, 6, 8, 10
};

void InitPolyBook(){
    EngineOptions->UseBook = FALSE;
    FILE* pFile = fopen("codekiddy.bin", "rb");
    if(pFile == NULL){
        printf("Book File Not Read\n");
    }else{
        fseek(pFile, 0, SEEK_END);
        long position = ftell(pFile);

        if(position < sizeof(S_POLY_BOOK_ENTRY)){
            printf("No Entries Found\n");
            return;
        }
        NumEntries = position / sizeof(S_POLY_BOOK_ENTRY);
        printf("%ld Entries found\n", NumEntries);

        entries = (S_POLY_BOOK_ENTRY*) malloc(NumEntries* sizeof(S_POLY_BOOK_ENTRY));

        rewind(pFile);

        size_t returnValue;
        returnValue = fread(entries, sizeof(S_POLY_BOOK_ENTRY), NumEntries, pFile);

        printf("fread() %ld Entries Read in from file\n", returnValue);

        if(NumEntries > 0) {
            EngineOptions->UseBook = TRUE;
        }
    }
}

void CleanPolyBook(){
    free(entries);
}

int HasPawnForCapture(const S_BOARD* board){
    int sqWithPawn = 0;
    int targetPce = (board->side == WHITE) ? wP : bP;
    if(board->enPas != NO_SQ){
        sqWithPawn = (board->side == WHITE) ?
            (board->enPas - 10) :
            (board->enPas + 10);
        if(board->pieces[sqWithPawn + 1] == targetPce){
            return TRUE;
        } else if(board->pieces[sqWithPawn - 1] == targetPce){
            return TRUE;
        }
    }
    return FALSE;
}

U64 PolyKeyFromBoard(const S_BOARD* board){
    int sq = 0, rank = 0, file = 0;
    U64 finalKey = 0;
    int piece = EMPTY;
    int polyPiece = 0;
    int offset = 0;
    //pieces
    for(sq = 0; sq < BRD_SQ_NUM; ++sq){
        piece = board->pieces[sq];
        if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD){
            ASSERT(piece >= wP && piece <= bK);
            polyPiece = PolyKindOfPiece[piece];
            rank = RanksBrd[sq];
            file = FilesBrd[sq];
            finalKey ^= Random64Poly[(64 * polyPiece + (8*rank) + file)];
        }
    }
    //castling
    offset = 768;
    if(board->castlePerm & WKCA) finalKey ^= Random64Poly[offset];
    if(board->castlePerm & WQCA) finalKey ^= Random64Poly[offset + 1];
    if(board->castlePerm & BKCA) finalKey ^= Random64Poly[offset + 2];
    if(board->castlePerm & BQCA) finalKey ^= Random64Poly[offset + 3];
    
    //enpas
    offset = 772;
    if(HasPawnForCapture(board) == TRUE){
        file = FilesBrd[board->enPas];
        finalKey ^= Random64Poly[offset + file];
    }

    //side
    if(board->side == WHITE){
        finalKey ^= Random64Poly[780];
    }

    return finalKey;
}

unsigned short endian_swap_u16(unsigned short x){
    x = (x >> 8) | (x << 8);
    return x;
}

unsigned int endian_swap_u32(unsigned int x){
    x = (x>>24) | ((x<<8) & 0x00FF0000) | ((x>>8) & 0x0000FF00) | (x << 24);
    return x;
}

U64 endian_swap_u64(U64 x)
{
    x = (x>>56) |
        ((x<<40) & 0x00FF000000000000) |
        ((x<<24) & 0x0000FF0000000000) |
        ((x<<8)  & 0x000000FF00000000) |
        ((x>>8)  & 0x00000000FF000000) |
        ((x>>24) & 0x0000000000FF0000) |
        ((x>>40) & 0x000000000000FF00) |
        (x<<56);
    return x;
}

int ConvertPolyMoveToInternalMove(unsigned short polyMove, S_BOARD* board){
    int ff = (polyMove >> 6) & 7 ;
    int fr =  (polyMove >> 9) & 7;
    int tf =  (polyMove >> 0) & 7;
    int tr = (polyMove >> 3) & 7;
    int pp = (polyMove >> 12) & 7;

    char moveString[6];
    if(pp == 0){
        sprintf(moveString, "%c%c%c%c", FileChar[ff], RankChar[fr], FileChar[tf], RankChar[tr]);
    }else{
        char promChar = 'q';
        switch(pp){
            case 1:
                promChar = 'n';
                break;
            case 2:
                promChar = 'b';
                break;
            case 3:
                promChar = 'r';
                break;
        }
        sprintf(moveString, "%c%c%c%c%c", FileChar[ff], RankChar[fr], FileChar[tf], RankChar[tr], promChar);
    }
    return ParseMove(moveString, board);
}

int GetBookMove(S_BOARD* board){
    int index = 0;
    S_POLY_BOOK_ENTRY* entry;
    unsigned short move;
    unsigned short weight;
    const int MAXBOOKMOVES = 32;
    int bookMoves[MAXBOOKMOVES];
    int bookMoveWeights[MAXBOOKMOVES];
    int tempMove = NOMOVE;
    int count = 0;
    int totalWeight = 0;
    U64 polyKey = PolyKeyFromBoard(board);
    for(entry = entries; entry < entries + NumEntries; ++entry){
        if(polyKey == endian_swap_u64(entry->key)){
            move = endian_swap_u16(entry->move);
            weight = endian_swap_u16(entry->weight);
            tempMove = ConvertPolyMoveToInternalMove(move, board);
            if(tempMove != NOMOVE){
                bookMoves[count] = tempMove;
                bookMoveWeights[count++] = weight;
                totalWeight += weight;
                if(count > MAXBOOKMOVES){
                    break;
                }
            }
            //printf("Key: %llx Index: %d Move: %c%c%c%c\n", endian_swap_u64(entry->key), start, FileChar[(move >> 6) & 7 ], RankChar[(move >> 9) & 7], FileChar[(move >> 0) & 7], RankChar[(move >> 3) & 7]);
        }
       
    }
    printf("Listing Book Moves:\n");
    for(index = 0; index < count; ++index){
        printf("BookMove %d : %s | weight: %u\n", index+1, PrMove(bookMoves[index]), bookMoveWeights[index]);
    }
    if(count != 0){
        if(count == 1) return bookMoves[0];
        double percents[count];
        double cumPercents[count];
        for(index = 0; index< count; ++index){
            percents[index] = ((double)bookMoveWeights[index])/((double)totalWeight);
            if(index > 0){
                percents[index] += percents[index-1];
            }
             printf("Percent Cutoff for %s : %lf\n", PrMove(bookMoves[index]), percents[index]);
        }
        
        double randPoint = (double)rand() / (double)RAND_MAX;
        index = 0;
        printf("RANDPOINT: %lf\n ", randPoint);
        while(randPoint > percents[index] && index < count-1){
            index++;
        }
        printf("index: %d\n", index);
        printf("Move: %s\n", PrMove(bookMoves[index]));
        return bookMoves[index];
    }else{
        return NOMOVE;
    }
    
}


