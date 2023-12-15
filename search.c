#include "stdio.h"
#include "defs.h"

void SearchPosition(S_BOARD* pos){

}


int IsRepetition(const S_BOARD* pos){
    int index = 0;
    //can't repeat position before last time fifty move was reset
    for(index = pos->hisPly - pos->fiftyMove; index < pos->hisPly-1; ++index){
        ASSERT(index >= 0 && index < MAXGAMEMOVES);
        if(pos->posKey == pos->history[index].posKey){
            return TRUE;
        }
    }
    return FALSE;
}