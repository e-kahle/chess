#include "stdio.h"
#include "defs.h"





int GetPvLine(const int depth, S_BOARD* pos){
    ASSERT(depth < MAXDEPTH);
    int move = ProbePvTable(pos);
    int count = 0;
    while(move != NOMOVE && count < depth){
        ASSERT(count < MAXDEPTH);
        if(MoveExists(pos, move)){
            MakeMove(pos, move);
            pos->PvArray[count++] = move;
        } else{
            break;
        }
        move = ProbePvTable(pos);
    }
    //this might cause bugs later if I read ply from fen
    while(pos->ply > 0){
        TakeMove(pos);
    }
    return count;
}


const int HashSize = 0x100000 * 16;




void ClearHashTable(S_HASHTABLE* table){
    S_HASHENTRY* tableEntry;
    for(tableEntry = table->pTable; tableEntry < table->pTable + table->numEntries; tableEntry++){
        tableEntry->posKey = 0ULL;
        tableEntry->move = NOMOVE;
        tableEntry->depth = 0;
        tableEntry->score = 0;
        tableEntry->flags = 0;
    }
    table->newWrite = 0;
}

void InitHashTable(S_HASHTABLE* table){
    
    table->numEntries = HashSize / sizeof(S_HASHENTRY);
    table->numEntries -=2;
    //printf("Address of table->pTable before free: %p\n", (void*)table->pTable);
    if(table->pTable != NULL){
        //printf("hi");
        free(table->pTable);
    }
    // free(table->pTable);
    // printf("hi");
    table->pTable = (S_HASHENTRY *) malloc(table->numEntries * sizeof(S_HASHENTRY));
    ClearHashTable(table);
    printf("HashTable init complete with %d entries \n", table->numEntries);

}

int ProbePvTable(const S_BOARD* pos){
    int index = pos->posKey % pos->HashTable->numEntries;
    ASSERT(index >= 0 && index <= pos->HashTable->numEntries -1);

    if( pos->HashTable->pTable[index].posKey == pos->posKey){
        return pos->HashTable->pTable[index].move;
    }
    return NOMOVE;
}

// void StoreHashEntry( S_BOARD* pos, const int move, int score, const int flags, const int depth){
//     int index = pos->posKey % pos->HashTable->numEntries;
//     ASSERT(index >= 0 && index <= pos->HashTable->numEntries -1 );
//     ASSERT(depth >= 1 && depth < MAXDEPTH);
//     ASSERT(flags >= HFALPHA && flags <= HFEXACT);
//     ASSERT(score >= -INFINITE && score <= INFINITE);
//     ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

//     if(pos->HashTable->pTable[index].posKey == 0){
//         pos->HashTable->newWrite++;
//     }else{
//         pos->HashTable->overWrite++;
//     }

//     if(score > ISMATE) score += pos->ply;
//     else if(score < -ISMATE) score -= pos->ply;

//     pos->HashTable->pTable[index].move = move;
//     pos->HashTable->pTable[index].posKey = pos->posKey;
//     pos->HashTable->pTable[index].flags = flags;
//     pos->HashTable->pTable[index].score = score;
//     pos->HashTable->pTable[index].depth = depth;
// }

// int ProbeHashEntry(S_BOARD* pos, int* move, int* score, int alpha, int beta, int depth ){
//     int index = pos->posKey % pos->HashTable->numEntries;
//     ASSERT(index >= 0 && index <= pos->HashTable->numEntries -1 );
//     ASSERT(depth >= 1 && depth < MAXDEPTH);
//     ASSERT(alpha < beta);
//     ASSERT(alpha >= -INFINITE && alpha <= INFINITE);
//     ASSERT(beta >= -INFINITE && beta <= INFINITE);
//     ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

//     if(pos->HashTable->pTable[index].posKey == pos->posKey){
//         *move = pos->HashTable->pTable[index].move;
//         if(pos->HashTable->pTable[index].depth >= depth){
//             pos->HashTable->hit++;

//             ASSERT(pos->HashTable->pTable[index].depth >= 1 && pos->HashTable->pTable[index].depth < MAXDEPTH);
//             ASSERT(pos->HashTable->pTable[index].flags >= HFALPHA && pos->HashTable->pTable[index].flags <= HFEXACT);

//             *score = pos->HashTable->pTable[index].score;
//             if(*score > ISMATE) *score -= pos->ply;
//             else if(*score < -ISMATE) *score += pos->ply;

//             switch(pos->HashTable->pTable[index].score){
//                 //ASSERT(*score == pos->HashTable->pTable[index].score);
//                 ASSERT(*score >= -INFINITE && *score <= INFINITE);

//                 case HFALPHA: if(*score <= alpha){
//                     *score = alpha;
//                     return TRUE;
//                     }
//                     break;
//                 case HFBETA: if(*score >= beta){
//                     *score = beta;
//                     return TRUE;
//                     }
//                     break;
//                 case HFEXACT:
//                     return TRUE;
//                     break;
//                 default: ASSERT(FALSE); break;
//             }
//         }
//     }
//     return FALSE;

// }
int ProbeHashEntry(S_BOARD *pos, int *move, int *score, int alpha, int beta, int depth) {

	int index = pos->posKey % pos->HashTable->numEntries;
	
	ASSERT(index >= 0 && index <= pos->HashTable->numEntries - 1);
    ASSERT(depth>=1&&depth<MAXDEPTH);
    ASSERT(alpha<beta);
    ASSERT(alpha>=-INFINITE&&alpha<=INFINITE);
    ASSERT(beta>=-INFINITE&&beta<=INFINITE);
    ASSERT(pos->ply>=0&&pos->ply<MAXDEPTH);
	
	if( pos->HashTable->pTable[index].posKey == pos->posKey ) {
		*move = pos->HashTable->pTable[index].move;
		if(pos->HashTable->pTable[index].depth >= depth){
			pos->HashTable->hit++;
			
			ASSERT(pos->HashTable->pTable[index].depth>=1&&pos->HashTable->pTable[index].depth<MAXDEPTH);
            ASSERT(pos->HashTable->pTable[index].flags>=HFALPHA&&pos->HashTable->pTable[index].flags<=HFEXACT);
			
			*score = pos->HashTable->pTable[index].score;
			if(*score > ISMATE) *score -= pos->ply;
            else if(*score < -ISMATE) *score += pos->ply;
			
			switch(pos->HashTable->pTable[index].flags) {
				
                ASSERT(*score>=-INFINITE&&*score<=INFINITE);

                case HFALPHA: if(*score<=alpha) {
                    *score=alpha;
                    return TRUE;
                    }
                    break;
                case HFBETA: if(*score>=beta) {
                    *score=beta;
                    return TRUE;
                    }
                    break;
                case HFEXACT:
                    return TRUE;
                    break;
                default: ASSERT(FALSE); break;
            }
		}
	}
	
	return FALSE;
}

void StoreHashEntry(S_BOARD *pos, const int move, int score, const int flags, const int depth) {

	int index = pos->posKey % pos->HashTable->numEntries;
	
	ASSERT(index >= 0 && index <= pos->HashTable->numEntries - 1);
	ASSERT(depth>=1&&depth<MAXDEPTH);
    ASSERT(flags>=HFALPHA&&flags<=HFEXACT);
    ASSERT(score>=-INFINITE&&score<=INFINITE);
    ASSERT(pos->ply>=0&&pos->ply<MAXDEPTH);
	
	if( pos->HashTable->pTable[index].posKey == 0) {
		pos->HashTable->newWrite++;
	} else {
		pos->HashTable->overWrite++;
	}
	
	if(score > ISMATE) score += pos->ply;
    else if(score < -ISMATE) score -= pos->ply;
	
	pos->HashTable->pTable[index].move = move;
    pos->HashTable->pTable[index].posKey = pos->posKey;
	pos->HashTable->pTable[index].flags = flags;
	pos->HashTable->pTable[index].score = score;
	pos->HashTable->pTable[index].depth = depth;
}