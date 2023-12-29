#include "stdio.h"
#include "defs.h"
#include "string.h"
#include "tinycthread.h"
#define MATE 29000
int rootDepth;
thrd_t workerThreads[MAXTHREADS];

static void CheckUp(S_SEARCHINFO* info){
    // check if time up, or interrupted from GUI
    if(info->timeset == TRUE && GetTimeMs() > info->stoptime){
        info->stopped = TRUE;
    }
    
}

static void PickNextMove(int moveNum, S_MOVELIST* list){
    S_MOVE temp;
    int index = 0;
    int bestScore = 0;
    int bestNum = moveNum;

    for(index = moveNum; index < list->count; ++index){
        if(list->moves[index].score > bestScore){
            bestScore = list->moves[index].score;
            bestNum = index;
        }
    }
    temp = list->moves[moveNum];
    list->moves[moveNum] = list->moves[bestNum];
    list->moves[bestNum] = temp;
}


static int IsRepetition(const S_BOARD* pos){
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

static void ClearForSearch(S_BOARD* pos, S_SEARCHINFO* info, S_HASHTABLE* table){
    int index = 0;
    int index2 = 0;
    for(index = 0; index < 13; ++index){
        for(index2 = 0; index2 < BRD_SQ_NUM; ++index2){
            pos->searchHistory[index][index2] = 0;
        }
    }
    for(index = 0; index < 2; ++index){
        for(index2 = 0; index2 < MAXDEPTH; ++index2){
            pos->searchKillers[index][index2] = 0;
        }
    }
    //printf("hi");
    // ClearPvTable(pos->PvTable);
    table->overWrite = 0;
    table->hit = 0;
    table->cut = 0;
    table->currentAge++;
    pos->ply = 0;

    //info->starttime = GetTimeMs();
    info->stopped = 0;
    info->nodes = 0;
    info->fh = 0;
    info->fhf = 0;
    
}

static int Quiescence(int alpha, int beta, S_BOARD* pos, S_SEARCHINFO* info){
    ASSERT(CheckBoard(pos));

    if((info->nodes & 2047) == 0){
        CheckUp(info);
        if(info->stopped == TRUE) return 0;
    }

    info->nodes++;

    if(IsRepetition(pos) || pos->fiftyMove >= 100){
        return 0;
    }
    if(pos->ply > MAXDEPTH -1){
        return EvalPosition(pos);
    }
    int Score = EvalPosition(pos);

    if(Score >= beta){
        return beta;
    }
    if(Score > alpha){
        alpha = Score;
    }

    S_MOVELIST list[1];
    GenerateAllCaps(pos, list);

    int MoveNum = 0;
    int Legal = 0;
    int OldAlpha = alpha;
    Score = -AB_BOUND;

    for(MoveNum = 0; MoveNum < list->count; ++MoveNum){
        PickNextMove(MoveNum, list);
        if(!MakeMove(pos, list->moves[MoveNum].move)){
            continue;
        }
        Legal++;
        Score = -Quiescence(-beta, -alpha, pos, info);
        TakeMove(pos);
        if(info->stopped == TRUE){
            return 0;
        }
        if(Score > alpha){
            if(Score >= beta){
                if(Legal == 1){
                    info->fhf++;
                }
                info->fh++;
                // printf("beta");
                return beta;
            }
            alpha = Score;
        }
    }

    return alpha;
}

static int AlphaBeta(int alpha, int beta, int depth, S_BOARD* pos, S_SEARCHINFO* info, int DoNull, S_HASHTABLE* table){
    ASSERT(CheckBoard(pos));
    if(depth == 0){
        return Quiescence(alpha, beta, pos, info);
    }

    if((info->nodes & 2047) == 0){
        CheckUp(info);
        if(info->stopped == TRUE) return 0;
    }
    
    info->nodes++;

    if((IsRepetition(pos) || pos->fiftyMove >= 100 )&& pos->ply){
        // printf("isrep");
        return 0;
    }
    if(pos->ply > MAXDEPTH -1){
        // printf("ply max depth");
        return EvalPosition(pos);
    }

    int InCheck = SqAttacked(pos->KingSq[pos->side], pos->side^1, pos);
    if(InCheck == TRUE){
        depth++;
    }
    
    int Score = -AB_BOUND;
    int PvMove = NOMOVE;

    if(ProbeHashEntry(pos, &PvMove, &Score, alpha, beta, depth, table) == TRUE){
        table->cut++;
        return Score;
    }

    if(DoNull && !InCheck && pos->ply && (pos->bigPce[pos->side] > 1) && depth >= 4){
        MakeNullMove(pos);
        Score = -AlphaBeta(-beta, -beta + 1, depth -4, pos, info, FALSE, table);
        TakeNullMove(pos);
        if(info->stopped == TRUE){
            return 0;
        }
        if(Score >= beta && abs(Score) < ISMATE){
            info->nullCut++;
            return beta;
        }
    }


    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);
    int MoveNum = 0;
    int Legal = 0;
    int OldAlpha = alpha;
    int BestMove = NOMOVE;
    Score = -AB_BOUND;
    int BestScore = -AB_BOUND;
    //PvMove = ProbePvTable(pos);
    if(PvMove != NOMOVE){
        for(MoveNum = 0; MoveNum < list->count; ++MoveNum){
            if(list->moves[MoveNum].move == PvMove){
                list->moves[MoveNum].score = 2000000;
                break;
            }
        }
    }
    for(MoveNum = 0; MoveNum < list->count; ++MoveNum){
        PickNextMove(MoveNum, list);
        if(!MakeMove(pos, list->moves[MoveNum].move)){
            continue;
        }
        Legal++;
        Score = -AlphaBeta(-beta, -alpha, depth-1, pos, info, TRUE, table);
        TakeMove(pos);
        if(info->stopped == TRUE){
            return 0;
        }
        if(Score > BestScore){
            BestScore = Score;
            BestMove = list->moves[MoveNum].move;
            if(Score > alpha){
                if(Score >= beta){
                    if(Legal == 1){
                        info->fhf++;
                    }
                    info->fh++;
                    // printf("beta");
                    if(!(list->moves[MoveNum].move & MFLAGCAP)){
                        pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
                        pos->searchKillers[0][pos->ply] = list->moves[MoveNum].move;
                    }
                    StoreHashEntry(pos, BestMove, beta, HFBETA, depth, table);

                    return beta;
                }
                alpha = Score;
                BestMove = list->moves[MoveNum].move;
                if(!(list->moves[MoveNum].move & MFLAGCAP)){
                    pos->searchHistory[pos->pieces[FROMSQ(BestMove)]][TOSQ(BestMove)] += depth;
                }
            }
        }
    }
    if(Legal == 0){
        if(SqAttacked(pos->KingSq[pos->side], pos->side ^1, pos)){
            //mate
            // printf("mate");
            return -AB_BOUND + pos->ply;
        }else{
            // printf("draw");
            return 0;
        }
    }
    if(alpha != OldAlpha){
        // StorePvMove(pos, BestMove);
        StoreHashEntry(pos, BestMove, BestScore, HFEXACT, depth, table);
    }else{
        StoreHashEntry(pos, BestMove, alpha, HFALPHA, depth, table);
    }
    // printf("alpha");
    return alpha;
}

int SearchPosition_Thread(void* data){
    S_SEARCH_THREAD_DATA* searchData = (S_SEARCH_THREAD_DATA *) data;
    S_BOARD* pos = malloc(sizeof(S_BOARD));
    memcpy(pos, searchData->originalPosition, sizeof(S_BOARD));
    SearchPosition(pos, searchData->info, searchData->ttable);
    free(pos);
    // printf("Freed\n");
    return 0;
}   

void IterativeDeepen(S_SEARCH_WORKER_DATA* workerData){
    workerData->bestMove = NOMOVE;
    int bestScore = -AB_BOUND;
    int currentDepth = 0;
    int pvMoves = 0;
    int pvNum = 0;
     //printf("hi");
    for(currentDepth = 1; currentDepth <= workerData->info->depth; ++currentDepth){
        rootDepth = currentDepth;
        bestScore = AlphaBeta(-AB_BOUND, AB_BOUND, currentDepth, workerData->pos, workerData->info, TRUE, workerData->ttable);

        if(workerData->info->stopped == TRUE){
            break;
        }
        if(workerData->threadNumber == 0){
            pvMoves = GetPvLine(currentDepth, workerData->pos, workerData->ttable);
            workerData->bestMove = workerData->pos->PvArray[0];
            printf("info score cp %d depth %d nodes %ld time %d pv",  bestScore, currentDepth, workerData->info->nodes, GetTimeMs() - workerData->info->starttime);

            for(pvNum = 0; pvNum < pvMoves; ++pvNum){
                printf(" %s", PrMove(workerData->pos->PvArray[pvNum]));
            }
            N(1);
        }
       
        //printf("Ordering %.2f\n", (info->fhf / info->fh));
    }
}

int StartWorkerThread(void* data){
    S_SEARCH_WORKER_DATA* workerData = (S_SEARCH_WORKER_DATA*) data;
    // printf("Thread: %d Starts\n", workerData->threadNumber);
    IterativeDeepen(workerData);
    // printf("Thread: %d Ends\n", workerData->threadNumber, workerData->depth);
    if(workerData->threadNumber == 0){
        printf("bestmove %s\n", PrMove(workerData->bestMove));
    }
    free(workerData);
}

void SetupWorker(int threadNum, thrd_t* workerTh, S_BOARD* pos, S_SEARCHINFO* info, S_HASHTABLE* table){
    S_SEARCH_WORKER_DATA* pWorkerData = malloc(sizeof(S_SEARCH_WORKER_DATA));
    pWorkerData->pos = malloc(sizeof(S_BOARD));
    memcpy(pWorkerData->pos, pos, sizeof(S_BOARD));
    pWorkerData->info = info;
    pWorkerData->ttable = table;
    pWorkerData->threadNumber = threadNum;
    thrd_create(workerTh, &StartWorkerThread, (void*)pWorkerData);
}

void CreateSearchWorkers(S_BOARD* pos, S_SEARCHINFO* info, S_HASHTABLE* table){
   // printf("CreateSearchWorkers: %d\n", info->threadNum);
    for(int i = 0; i<info->threadNum; i++){
        SetupWorker(i, &workerThreads[i], pos, info, table);
    }
}

void SearchPosition(S_BOARD* pos, S_SEARCHINFO* info , S_HASHTABLE* table){
    //iterative deepening
    //for depth = 1 to maxDepth
        //search AlphaBeta

    int bestMove = NOMOVE;
    // int bestScore = -AB_BOUND;
    // int currentDepth = 0;
    // int pvMoves = 0;
    // int pvNum = 0;
     //printf("hi");
    ClearForSearch(pos, info, table);
    if(EngineOptions->UseBook == TRUE){
        bestMove = GetBookMove(pos);
    }
    if(bestMove == NOMOVE){
        CreateSearchWorkers(pos, info, table);
    }else{
        printf("bestmove %s\n", PrMove(bestMove));
    }
    for(int i = 0; i<info->threadNum; i++){
        thrd_join(workerThreads[i], NULL);
    }
}