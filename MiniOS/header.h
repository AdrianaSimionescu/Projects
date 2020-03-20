/* SIMIONESCU MARIA-ADRIANA - 312 CB */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#ifndef _TEMA2_h_
#define _TEMA2_h_

#define mem_3MiB 3145728

typedef struct celulag
{
    void *info;
    struct celulag *urm;

}TCelulaG, *TLG, **ALG;

typedef struct
{
    size_t dime;
    TLG vf;

}TStiva;

typedef struct{

    int start_memory;
    int total_memory;
    int end_memory;
    int PID;

}TMemory;

typedef struct{

    TStiva memory;
    int sum_bytes_stiva;
    int PID;
    int total_memory;
    int priority;
    int time_of_execution;
    int executed_time;

}TProcess_info;

typedef struct
{
    size_t dime;
    TLG ic, sc;
}TCoada;

void *InitS( size_t d );
void Push( void *s,void *ae );
void Pop( void *s, void *ae );
void DistrS( void *s );

void *InitQ( size_t d );
void ExtrgQ( void *c, void *ae );
void IntrQ( void *c, void *ae, size_t d );
void DistrQ( void *c );

TLG AlocCelulaG( void *ae, size_t d );
TProcess_info Init_Process( int x, int y, int t, int mem );
TMemory Init_Memory( int memo, int pid );
void DistrugeLG( ALG aL );
int InsSfG(ALG aL, void *ae, size_t d);
int Search_Best_PID( int *v );
void swap( TLG a, TLG b );
void Sortare( TLG L );
void print_stack( void *s, FILE *f, int pid );
void Memory_update( ALG aL, int pid );
int InsG( ALG aL, void *ae, size_t d, int *start);
void Defragmentare( ALG aL );
void Elib(TLG L);
void memory_allocation_buffer(int width, char **buffer);
void memory_allocation_array(int width, int **array);

int SearchRunning( TLG L, int pid, int long long *time );
void Run_and_Push( TLG L, int pid, int bytes, FILE *f );
void Run_and_Pop( TLG L, int pid, FILE *f );
void Run_Time( TLG L, int time, void *c, int *ok );
void Run_Time_2( ALG aL,int time, int TIME, void *c_f, void *c_w, int *v,
                            TLG L,int *rest, int *exec_time, int *process );
void Run_Stack( TLG L, int pid, FILE *F );
int Sum_Running( TLG L );
void Run_and_Finish( TLG L , void *c_f );

void Print_Waiting( void *c, FILE *f );
int SearchWaiting( void *c, int pid, int long long *time );
void Wait_and_Pop( void *c, int pid, FILE *f );
void Wait_Stack( void *c, int pid, FILE *f );
void Wait_and_Push( void *c, int pid, int bytes, FILE *f );
int Sum_Waiting( void *c );
void SortareWaiting( void *c,void *ae );
void Wait_and_finish( void *c ,void *c_f );

void Print_Finished( void *c, FILE *f );
int SearchFinished( void *c, int pid, FILE *f );

#endif
