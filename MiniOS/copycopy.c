#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define mem_3MiB 3145728
#define Idle 0

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
    int execution;
}TMemory;

typedef struct{
    TStiva memory;
    int PID;
    int priority;
    int time_of_execution;
    int executed_time;
}TProcess_info;

typedef struct
{
    size_t dime;
    TLG ic, sc;
}TCoada;

TLG AlocCelulaG(void *ae, size_t d)
{
    TLG aux;

    aux = (TLG) malloc(sizeof(TCelulaG));
    if(!aux) return NULL;

    aux->info = malloc(d);

    if(!aux->info){
        free(aux);
        return NULL;
    }
    memcpy(aux->info,ae,d);
    aux->urm = NULL;

    return aux;
}

void *InitS(size_t d)
{
    TStiva *s;

    s = (TStiva*)malloc(sizeof(TStiva));
    if(!s) return NULL;

    s->dime = d;
    s->vf = NULL;

    return (void*)s;
}

void *InitQ(size_t d)
{
    TCoada *c;

    c = (TCoada*)malloc(sizeof(TCoada));
    if(!c) return NULL;

    c->dime = d;
    c->ic = c->sc = NULL;

    return (void*)c;
}


void IntrQ(void *c, void *ae, size_t d)   
{ 
    TLG aux;

    aux = AlocCelulaG(ae,d);
    if(!aux) return;
    
    if(((TCoada*)c)->sc != NULL){
        ((TCoada*)c)->sc->urm = aux; 
        ((TCoada*)c)->sc = aux;
    }else{
        ((TCoada*)c)->sc = ((TCoada*)c)->ic = aux;
    }

    (((TCoada*)c)->dime)++;
}

/*initilize the characteristic features of the process*/
TProcess_info Init_Process(int x, int y, int t)
{
    TProcess_info *process;
    void *s;

    process = (TProcess_info*)malloc(sizeof(TProcess_info));

    s = InitS(0);
    process->executed_time = 0;
    process->memory = *((TStiva*)s);
    process->PID = x;
    process->priority = y;
    process->time_of_execution = t;

    return *process;
}

TMemory Init_Memory( int memo, int pid )
{
    TMemory *memory;

    memory = (TMemory*)malloc(sizeof(TMemory));

    memory->start_memory = 0;
    memory->total_memory = memo;
    memory->PID = pid;
    memory->execution = 1;
    memory->end_memory = 0;

    return *memory;
}

void ExtrgQ(void *c, void *ae)
{
    TLG aux;

    aux = ((TCoada*)c)->ic;

    if(aux != NULL){

    if(((TCoada*)c)->ic != ((TCoada*)c)->sc)  ((TCoada*)c)->ic = aux->urm;
    else ((TCoada*)c)->ic = ((TCoada*)c)->sc = NULL;

    memcpy(ae,(TProcess_info*)aux->info,sizeof(TProcess_info));
    (((TCoada*)c)->dime)--;
    free(aux->info);
    free(aux);
    }
}

void Pop(void *s, void *ae)
{
    TLG aux;
    
    memcpy(ae,((TStiva*)s)->vf->info,sizeof(int));

    aux = ((TStiva*)s)->vf;

    if( aux != NULL ){

    ((TStiva*)s)->vf = aux->urm;

    (((TStiva*)s)->dime)--;

    free(aux->info);
    free(aux);
    }
}

void DistrS(void *s)
{
    int x;

    while( ((TStiva*)s)->vf != NULL ){
		Pop(s,&x);
    }
}

void DistrQ(void *c)
{
    TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        DistrS(&(inf.memory));
	}
}

void SortareWaiting(void *c,void *ae)
{
    void *c1 = InitQ(0);
    void *c2 = InitQ(0);
	TProcess_info inf;
    int ok = 0;
    TProcess_info p2 = *((TProcess_info*)ae);

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        int done1 = 0, done = 0;

        if( inf.priority < p2.priority && ok == 0 ){
            IntrQ(c2,&p2,sizeof(TProcess_info));
            ok = 1;
            done = 1;
            IntrQ(c2,&inf,sizeof(TProcess_info));
        }

        if( inf.priority < p2.priority && ok == 1 && done == 0 ) done1 = 1;
        
        if( inf.priority == p2.priority && ok == 0 ){
            if( inf.time_of_execution > p2.time_of_execution ){
                IntrQ(c2,&p2,sizeof(TProcess_info));
                ok = 1;
                done = 1;
                IntrQ(c2,&inf,sizeof(TProcess_info));
            }
        }

        if( inf.priority == p2.priority && ok == 1 && done == 0 ){
            if( inf.time_of_execution > p2.time_of_execution ){
                done1 = 1;
            }
        }

        if( inf.priority == p2.priority && 
            inf.time_of_execution == p2.time_of_execution && ok == 0){
                if( inf.PID > p2.PID ){
                IntrQ(c2,&p2,sizeof(TProcess_info));
                ok = 1;
                done = 1;
                IntrQ(c2,&inf,sizeof(TProcess_info));
            }
        }

        if( inf.priority == p2.priority && 
            inf.time_of_execution == p2.time_of_execution && ok == 1 && done == 0){
            if( inf.PID > p2.PID ){
                done1 = 1;
            }
        }
        
        if( done1 == 1 ) IntrQ(c2,&inf,sizeof(TProcess_info));
        if( ok == 0 ) IntrQ(c1,&inf,sizeof(TProcess_info));
	}

    if(ok == 0) IntrQ(c1,&p2,sizeof(TProcess_info));

	while( ((TCoada*)c1)->sc != NULL ){
		ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);

    while( ((TCoada*)c2)->sc != NULL ){

		ExtrgQ(c2,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

    DistrQ(c2);
}

int Search_Best_PID(int *v)
{
    int i;

    for(i = 1; i < 100; i ++){
        if( v[i] <= 0 ) return i;
    }
    return -1;
}

void Search_PID(int *v)
{
    int i;

    for(i = 1; i < 20; i ++){
        printf("%d\n",v[i]);
    }
    printf("\n");
}

/* function to swap data of two nodes a and b*/
void swap( TLG a, TLG b) 
{ 
    TMemory temp;
    memcpy(&temp,(TMemory*)a->info,sizeof(TMemory)); 
    memcpy((TMemory*)a->info,(TMemory*)b->info,sizeof(TMemory)); 
    memcpy((TMemory*)b->info,&temp,sizeof(TMemory));
} 

void Sortare(TLG L) 
{ 
    TLG start = L; 
    TLG L1; 
    TLG L2; 
  
    /* Checking for empty list */
    if (L == NULL) 
        return;

    while( start->urm != NULL ){
        L2 = start;
        L1 = start->urm;

        while(L1 != NULL){

            if (((TMemory*)L2->info)->PID > ((TMemory*)L1->info)->PID){
                L2 = L1;
            } 

            L1 = L1->urm;
        }

        swap(start,L2);
        start = start->urm;
    }

}

int SearchRunning( TLG L, int pid, int long long *time)
{
    for(; L != NULL; L = L->urm ){
        if( ((TProcess_info*)L->info)->PID == pid ) {
            *time = ((TProcess_info*)L->info)->time_of_execution;
            return 1;
        } 
    }

    return 0;
}

void Print_Wainting(void *c, FILE *f)
{
    void *c1 = InitQ(0);
	TProcess_info inf; 

    size_t dime =  ((TCoada*)c)->dime, count = 0;

    fprintf(f,"[");
	while( ((TCoada*)c)->sc != NULL ){
        count++;
		ExtrgQ(c,&inf);

        if( dime - count != 0 ){
            fprintf(f,"(%d: priority = %d, remaining_time = %d),\n",
            inf.PID,
            inf.priority,
            inf.time_of_execution);
        }
        else{
            fprintf(f,"(%d: priority = %d, remaining_time = %d)]",
            inf.PID,
            inf.priority,
            inf.time_of_execution);
        }

		IntrQ(c1,&inf,sizeof(TProcess_info));
	}
    if( count == 0 ) fprintf(f,"]");

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);
}

void Print_Finished(void *c, FILE *f)
{
    void *c1 = InitQ(0);
	TProcess_info inf; 

    size_t dime =  ((TCoada*)c)->dime, count = 0;

    fprintf(f,"[");
	while( ((TCoada*)c)->sc != NULL ){
        count++;
		ExtrgQ(c,&inf);

        if( dime - count != 0 ){
            fprintf(f,"(%d: priority = %d, executed_time = %d),\n",
            inf.PID,
            inf.priority,
            inf.executed_time);
        }
        else{
            fprintf(f,"(%d: priority = %d, executed_time = %d)]",
            inf.PID,
            inf.priority,
            inf.executed_time);
        }

		IntrQ(c1,&inf,sizeof(TProcess_info));
	}
    if( count == 0 ) fprintf(f,"]");

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);
}

void Print_Wainting2(void *c)
{
    void *c1 = InitQ(0);
	TProcess_info inf; 

    size_t dime =  ((TCoada*)c)->dime, count = 0;

    printf("[");
	while( ((TCoada*)c)->sc != NULL ){
        count++;
		ExtrgQ(c,&inf);

        if( dime - count != 0 ){
            printf("(%d: priority = %d, remaining_time = %d),\n",
            inf.PID,
            inf.priority,
            inf.time_of_execution);
        }
        else{
            printf("(%d: priority = %d, remaining_time = %d)]",
            inf.PID,
            inf.priority,
            inf.time_of_execution);
        }

		IntrQ(c1,&inf,sizeof(TProcess_info));
	}
    if( count == 0 ) printf("]\n");

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);
}


void Push(void *s,void *ae,int *ok)
{
    TLG aux;

    *ok = 1;

    aux = AlocCelulaG(ae,sizeof(int));
    if(!aux) *ok = 0;

    if(((TStiva*)s)->vf != NULL){
        aux->urm = ((TStiva*)s)->vf;
        ((TStiva*)s)->vf = aux; 
    }else{
        ((TStiva*)s)->vf = aux;
    }

    (((TStiva*)s)->dime)++;
}


void print_stack(void *s, FILE *f, int pid)
{
    void *s1 = InitS(0);
	int inf;
    int ok = 0;

   if( ((TStiva*)s)->vf == NULL ) fprintf(f,"Empty stack PID %d.\n",pid);
    else{

        fprintf(f,"Stack of PID %d:",pid); 
	    while( ((TStiva*)s)->vf != NULL ){
		    Pop(s,&inf);
		    Push(s1,&inf,&ok);
	    }

	    while( ((TStiva*)s1)->vf != NULL){
            Pop(s1,&inf);
            fprintf(f," %d",inf);
		    Push(s,&inf,&ok);
	    }
        fprintf(f,".\n");
        
	    DistrS(s1);
    }
}

int SearchWaiting(void *c, int pid, int long long *time)
{
    void *c1 = InitQ(0);
    int found = 0;
	TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        if( inf.PID == pid ){
            found = 1;
            *time = inf.time_of_execution;
        }
		IntrQ(c1,&inf,sizeof(TProcess_info));
	}

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);

    if( found == 1 ) return 1;
    else return 0;

}

int SearchFinished(void *c, int pid)
{
    void *c1 = InitQ(0);
    int found = 0;
	TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        if( inf.PID == pid ) found = 1;
		IntrQ(c1,&inf,sizeof(TProcess_info));
	}

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);

    if( found == 1 ) return 1;
    else return 0;

}

void Run_and_Push(TLG L, int pid, int bytes, FILE *f)
{
    int ok = 0;
    for(; L != NULL; L = L->urm ){
        if( ((TProcess_info*)L->info)->PID == pid ) {
            Push(&((TProcess_info*)L->info)->memory,&bytes,&ok);
            if( ok == 0 ){
                fprintf(f,"Stack overflow PID %d.\n",pid);
                break;
            }
            break;
        } 
    }
}

void Run_and_Pop(TLG L, int pid, FILE *f)
{
    int bytes = 0;
    for(; L != NULL; L = L->urm ){
        if( ((TProcess_info*)L->info)->PID == pid ) {
            TProcess_info inf = *((TProcess_info*)L->info);
            if( inf.memory.vf == NULL ){
                fprintf(f,"Empty stack PID %d.\n",pid);
            }
            else { Pop(&((TProcess_info*)L->info)->memory,&bytes);}
            break;
        } 
    }
}

void Run_Time(TLG L, int pid, int time, void *c)
{

    for(; L != NULL; L = L->urm ){

        if( ((TProcess_info*)L->info)->PID == pid ) {

            if( time < ((TProcess_info*)L->info)->time_of_execution ){
                ((TProcess_info*)L->info)->time_of_execution -= time;
                ((TProcess_info*)L->info)->executed_time += time;
            }else{
                ((TProcess_info*)L->info)->executed_time += time;
                IntrQ(c,((TProcess_info*)L->info),sizeof(TProcess_info));
            }
            break;
        } 
    }
}

void Print_Runnig2(TLG L)
{

    for(; L != NULL; L = L->urm ){
        printf("\n%d %d \n",((TProcess_info*)L->info)->PID,((TProcess_info*)L->info)->time_of_execution);
    }
}

void Memory_update(TLG L, int pid)
{
    for( ; L != NULL; L = L->urm ){
        if( ((TMemory*)L->info)->PID == pid ){
            ((TMemory*)L->info)->execution = 0;
            break;
        }
    }
}

void Run_Time_2(ALG aL,int time, int TIME, void *c_f, void *c_w, int *v,TLG L,int *rest, int *exec_time)
{
    TProcess_info info, info1;
    TLG p = *aL;
    int ok = 0;

    if( *rest != 0 ){ 
        TIME = *rest;
        ok = 1; 
        *exec_time = *rest;
    }

    if( time < TIME ){
        if( time  < ((TProcess_info*)p->info)->time_of_execution ){
            ((TProcess_info*)p->info)->time_of_execution -= time;
            ((TProcess_info*)p->info)->executed_time += time;

            *rest = TIME - time;
            ok = 1;
            *exec_time = time;

        }else{
            ((TProcess_info*)p->info)->executed_time += ((TProcess_info*)p->info)->time_of_execution;
            ((TProcess_info*)p->info)->time_of_execution = 0;

            memset(&v[((TProcess_info*)p->info)->PID ],0,sizeof(int));
            Memory_update(L,((TProcess_info*)p->info)->PID);

            IntrQ(c_f,((TProcess_info*)p->info),sizeof(TProcess_info));
            ExtrgQ(c_w,&info);
            memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
            *rest  = 0;
        }
    }else{
        
        if( TIME  < ((TProcess_info*)p->info)->time_of_execution ){
            
            ((TProcess_info*)p->info)->time_of_execution -= TIME;
            ((TProcess_info*)p->info)->executed_time += TIME;

            ExtrgQ(c_w,&info);
            memcpy(&info1,((TProcess_info*)p->info),sizeof(TProcess_info));
            memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
            SortareWaiting(c_w,&info1);
            *rest  = 0;
        }else{
            ((TProcess_info*)p->info)->executed_time += ((TProcess_info*)p->info)->time_of_execution;
            ok = 1;
            *exec_time  = ((TProcess_info*)p->info)->time_of_execution;
            ((TProcess_info*)p->info)->time_of_execution = 0;
            
            memset(&v[((TProcess_info*)p->info)->PID ],0,sizeof(int));
            Memory_update(L,((TProcess_info*)p->info)->PID);

            IntrQ(c_f,((TProcess_info*)p->info),sizeof(TProcess_info));
            ExtrgQ(c_w,&info);
            memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
            *rest = 0;
        }
    }

    if( ok == 0 ) *exec_time = TIME;
 
}

void Wait_Finish_and_Pop(void *c, int pid, FILE *f)
{
    void *c1 = InitQ(0);
	TProcess_info inf;
    int information = 0;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);

        if( inf.PID == pid ) {
            if( inf.memory.vf  == NULL ) fprintf(f,"Empty stack PID %d.\n",pid);
            else {
                Pop(&(inf.memory),&information);
            }
        }
		IntrQ(c1,&inf,sizeof(TProcess_info));
	}

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);

}


void Run_Stack(TLG L, int pid, FILE *F)
{
    for(; L != NULL; L = L->urm ){

        if( ((TProcess_info*)L->info)->PID == pid ) {
            print_stack(&(((TProcess_info*)L->info)->memory),F,pid);
            break;
        } 
    }
}

void Wait_Finish_Stack(void *c, int pid, FILE *f)
{
    void *c1 = InitQ(0);
	TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        if( inf.PID == pid ) {
            print_stack(&inf.memory,f,pid);
        }
		IntrQ(c1,&inf,sizeof(TProcess_info));
	}

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);

}

void Wait_and_Push(void *c, int pid, int bytes, FILE *f)
{
    void *c1 = InitQ(0);
	TProcess_info inf;
    int ok = 0;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        if( inf.PID == pid ) {
            Push(&inf.memory,&bytes,&ok);
            if( ok == 0 ){
                fprintf(f,"Stack overflow PID %d.\n",pid);
            }
        }
		IntrQ(c1,&inf,sizeof(TProcess_info));
	}

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);

}

void Finish_and_Push(void *c, int pid, int bytes, FILE *f)
{
    void *c1 = InitQ(0);
	TProcess_info inf;
    int ok = 0;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        if( inf.PID == pid ) {
            Push(&inf.memory,&bytes,&ok);
            if( ok == 0 ){
                fprintf(f,"Stack overflow PID %d.\n",pid);
            }
        }
		IntrQ(c1,&inf,sizeof(TProcess_info));
	}

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);

}

int Sum_Running( TLG L )
{
    int sum = 0;

    for(; L != NULL; L = L->urm ){
       sum += ((TProcess_info*)L->info)->time_of_execution;
    }

    return sum;
}

int Sum_Waiting( void *c )
{
    void *c1 = InitQ(0);
    int sum = 0;
	TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        sum += inf.time_of_execution;
		IntrQ(c1,&inf,sizeof(TProcess_info));
	}

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);
    return sum;
}


int InsSfG(ALG aL, void *ae, size_t d)
{
    TLG aux, u = NULL;

    if(*aL != NULL){
        u = *aL;
        while(u->urm != NULL) u = u->urm;
    }

    aux = AlocCelulaG(ae,d);
    if(!aux) return 0;

    if( u == NULL ) *aL = aux;
    else u->urm = aux;

    return 1;
}

int InsG( ALG aL, void *ae, size_t d, int *start)
{ 
    TLG p;
    int long long x = 3145728;

    if( *aL == NULL ){
        if( ((TMemory*)ae)->total_memory < mem_3MiB ){

            ((TMemory*)ae)->end_memory = ((TMemory*)ae)->total_memory;
            *start = ((TMemory*)ae)->start_memory;
            
            InsSfG(aL,ae,d);

            return 1;

        }else return 0;

    }else{
        for( p = *aL; p != NULL; p = p->urm ){
                if( p->urm == NULL ){

                    if( ((TMemory*)p->info)->end_memory + ((TMemory*)ae)->total_memory < x ){

                    if( ((TMemory*)p->info)->execution == 0 && ((TMemory*)p->info)->end_memory - 
                        ((TMemory*)p->info)->start_memory >= ((TMemory*)ae)->total_memory){

                        ((TMemory*)ae)->execution = 1;
                        ((TMemory*)ae)->start_memory = ((TMemory*)p->info)->start_memory;
                        ((TMemory*)ae)->end_memory = ((TMemory*)p->info)->start_memory + ((TMemory*)ae)->total_memory;
                        *start = ((TMemory*)ae)->start_memory;
                         
                        memcpy((TMemory*)p->info,ae,sizeof(TMemory));
                        return 1;
                    }
                    ((TMemory*)ae)->end_memory = ((TMemory*)p->info)->end_memory + ((TMemory*)ae)->total_memory;
                    ((TMemory*)ae)->start_memory = ((TMemory*)p->info)->end_memory;
                    *start = ((TMemory*)ae)->start_memory;
                   
                    InsSfG(aL,ae,d);
                    return 1;
                    }else return 0;

                }else{
                  
                    if( ((TMemory*)p->info)->execution == 0 && 
                        ((TMemory*)p->info)->end_memory - 
                        ((TMemory*)p->info)->start_memory >= ((TMemory*)ae)->total_memory ){
                        ((TMemory*)ae)->execution = 1;
                        ((TMemory*)ae)->start_memory = ((TMemory*)p->info)->start_memory;
                        ((TMemory*)ae)->end_memory = ((TMemory*)p->info)->end_memory;
                        *start = ((TMemory*)ae)->start_memory;
                        memcpy((TMemory*)p->info,ae,sizeof(TMemory));
                        return 1;
                    }
                }
        }

    }

    return 0;
}

void Defragmentare( ALG aL )
{
    TLG L = *aL, ant;

    for( ant = NULL; L != NULL; ant = L, L = L ->urm ){
        if( L == *aL ){
            ((TMemory*)L->info)->start_memory = 0;
            ((TMemory*)L->info)->end_memory = ((TMemory*)L->info)->start_memory + ((TMemory*)L->info)->total_memory;
        }else{
            ((TMemory*)L->info)->start_memory =  ((TMemory*)ant->info)->end_memory;
            ((TMemory*)L->info)->end_memory = ((TMemory*)L->info)->start_memory + ((TMemory*)L->info)->total_memory;
        }
    }
}

int find_last_mem( TLG L )
{   
    int poz = 1, keep_poz = 0, num = 0;

    for( ; L != NULL; L = L ->urm ){
        if( ((TMemory*)L->info)->execution != 0 ){
            keep_poz = poz;
        }else num++;
        poz++;
    }

    if( num == 0 ) return 0;
    else return keep_poz;
}

void free_mem( ALG aL, int find_poz )
{
    TLG L = *aL, aux, ant;
    int poz = 1;

    for( ; L != NULL; L = L ->urm ){
        if( poz == find_poz ) break;
        poz++;
    }

    if( L == NULL ) return;
    L->urm = NULL;
    
}

void print_ceva( TLG L )
{
    printf("\n\n");
    for( ; L != NULL; L = L ->urm ){
        printf("%d %d %d %d %d\n",((TMemory*)L->info)->PID,((TMemory*)L->info)->execution,
        ((TMemory*)L->info)->total_memory,((TMemory*)L->info)->start_memory,((TMemory*)L->info)->end_memory);
    }
}

void Elib(TLG L)
{
    for( ; L != NULL; L = L->urm ){
        DistrS(&((TProcess_info*)L->info)->memory);
    }
}

void DistrugeLG(ALG aL)
{
    TLG aux;

    while(*aL){
        aux = *aL;
        *aL = aux->urm;
        free(aux->info);
        free(aux);
    }
}
/*
void print_process( ALG aL, int pid, FILE *f )
{
    TLG L = *aL;

    for( ; L != NULL; L = L->urm ){
        
        if( ((TMemory*)L->info)->PID == pid ){
           fprintf(f,
            "Process created successfully: PID: %d, Memory starts at 0x%x.\n",
            pid,((TMemory*)L->info)->start_memory);
        }
    }
}*/

//function made to allocate memory for my buffer
void memory_allocation_buffer(int width, char **buffer)
{
    //allocate memory 
    *buffer = (char*)malloc(width*sizeof(char));
}

void memory_allocation_array(int width, int **array)
{
    //allocate memory 
    *array = (int*)malloc(width*sizeof(int));
}

int main(int argc, char *argv[])
{
    char *buf, *queue;               // buffer used for reading from the file
    int *PID_apar;
    int TIME, memory_size, time_of_execution, priority, PID, time, bytes;
    int rest = 0; 

    TProcess_info inf;
    int process_count = 0;

    void *c_waiting, *c_finished;

    if (argc != 3) {  // invalid number of arguments
    fprintf(stderr,
            "Usage: %s operations_string input_file output_file\n",
            argv[0]);
    return -1;
    }

    // Open input file for reading
    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "aw");

    if (input_file == NULL) {
        fprintf(stderr, "File \"%s\" not found\n", argv[1]);
        return -2;
    }

    fscanf(input_file,"%d\n",&TIME);

    memory_allocation_buffer(100,&buf);
    memory_allocation_buffer(10,&queue);
    memory_allocation_array(100,&PID_apar);
    /* array for PIDs */
    memset(PID_apar,0,100);
    c_waiting = InitQ(0);
    c_finished = InitQ(0);

    TLG Lrunning = NULL;
    TLG Lmemory = NULL;

    // Read data in file line by line
    while(fscanf(input_file,"%s",buf) > 0){
        if( strcmp(buf,"add") == 0 ){

            fscanf(input_file,"%d %d %d",&memory_size,&time_of_execution,&priority);

            int pid = 0,start = 0;
            pid = Search_Best_PID(PID_apar);

         //   if( total_mem + memory_size < mem_3MiB ){
         //   total_mem += memory_size;
            PID_apar[pid] = 1; /* TAKEN PID*/

            TMemory memory;
            memory = Init_Memory(memory_size,pid);

            int poz_mem_free = find_last_mem(Lmemory);

            print_ceva(Lmemory);
            printf("\n");
            if( poz_mem_free != 0 ){
                printf("%d\n",poz_mem_free);
                free_mem(&Lmemory,poz_mem_free);
            }
            printf("\n");
            print_ceva(Lmemory);
            int memo = InsG(&Lmemory,&memory,sizeof(TMemory),&start);

            if( memo == 1 ){

                fprintf(output_file,
                "Process created successfully: PID: %d, Memory starts at 0x%x.\n",
                pid,start);
                
                process_count++;

                inf = Init_Process(pid,priority,time_of_execution);

                if( process_count == 1 ) { InsSfG(&Lrunning,&inf,sizeof(TProcess_info)); }
                else SortareWaiting(c_waiting,&inf);

                }else{
                    Sortare(Lmemory);
                    Defragmentare(&Lmemory);

                    memo = InsG(&Lmemory,&memory,sizeof(TMemory),&start);

                    if( memo == 1 ){

                        fprintf(output_file,
                        "Process created successfully: PID: %d, Memory starts at 0x%x.\n",
                        pid,start);
                
                        process_count++;

                        inf = Init_Process(pid,priority,time_of_execution);

                        if( process_count == 1 ) { InsSfG(&Lrunning,&inf,sizeof(TProcess_info)); }
                        else SortareWaiting(c_waiting,&inf);
                    }else{
                        PID_apar[pid] = 0;
                        fprintf(output_file,"Cannot reserve memory for PID %d.\n",pid);
                    }
                }

                print_ceva(Lmemory);
        }
        

        if( strcmp(buf,"get") == 0 ){

            fscanf(input_file,"%d",&PID);

            int long long remaining_time;
            int run = SearchRunning(Lrunning,PID,&remaining_time);
            int wait = SearchWaiting(c_waiting,PID,&remaining_time);
            int finish = SearchFinished(c_finished,PID);

            if( run == 1 ) fprintf(output_file,"Process %d is running (remaining_time: %lld).\n",PID,remaining_time);
            if( wait == 1 ) fprintf(output_file,"Process %d is waiting (remaining_time: %lld).\n",PID,remaining_time);
            if( finish == 1 ) fprintf(output_file,"Process %d is finished.\n",PID);
            if( run == 0 && wait == 0 && finish == 0 ) fprintf(output_file,"Process %d not found.\n",PID);
        }

        if( strcmp(buf,"print") == 0 ){
            fscanf(input_file,"%s",queue);

            if( strcmp(queue,"waiting") == 0){

            fprintf(output_file,"Waiting queue:\n");

            Print_Wainting(c_waiting,output_file);
            fprintf(output_file,"\n");
            }

            if( strcmp(queue,"finished") == 0){

            fprintf(output_file,"Finished queue:\n");

            Print_Finished(c_finished,output_file);
            fprintf(output_file,"\n");
            }

            if( strcmp(queue,"stack") == 0 ){
                fscanf(input_file,"%d",&PID);

                int long long remaining_time;
                int run = SearchRunning(Lrunning,PID,&remaining_time);
                int wait = SearchWaiting(c_waiting,PID,&remaining_time);
  
                if( run == 1 ) Run_Stack(Lrunning,PID,output_file);
                if( wait == 1 ) Wait_Finish_Stack(c_waiting,PID,output_file);
                if( run == 0 && wait == 0 ) fprintf(output_file,"PID %d not found.\n",PID);
            }
        }

        if( strcmp(buf,"run") == 0 ){
            fscanf(input_file,"%d",&time);
        
            if( process_count == 1 ){
                Run_Time(Lrunning,PID,time,c_finished);
            }else{
                while(time){
                        int exec_time = 0;
                        Run_Time_2(&Lrunning,time,TIME,c_finished,c_waiting,PID_apar,Lmemory,&rest,&exec_time);
                        time = time - exec_time;
                }
            }
        }


        if( strcmp(buf,"push") == 0 ){

            fscanf(input_file,"%d %d",&PID, &bytes);

            int long long remaining_time;
            int run = SearchRunning(Lrunning,PID,&remaining_time);
            int wait = SearchWaiting(c_waiting,PID,&remaining_time);
          
            if( run == 1 ) Run_and_Push(Lrunning,PID,bytes, output_file);
            if( wait == 1 ) Wait_and_Push(c_waiting,PID,bytes, output_file);

            if( run == 0 && wait == 0 ) fprintf(output_file,"PID %d not found.\n",PID);

        }

        if( strcmp(buf,"pop") == 0 ){
            fscanf(input_file,"%d",&PID);

            int long long remaining_time;
            int run = SearchRunning(Lrunning,PID,&remaining_time);
            int wait = SearchWaiting(c_waiting,PID,&remaining_time);

            if( run == 1 ) Run_and_Pop(Lrunning,PID,output_file);
            if( wait == 1 ) Wait_Finish_and_Pop(c_waiting,PID,output_file);

            if( run == 0 && wait == 0 ) fprintf(output_file,"PID %d not found.\n",PID);

        }

        if( strcmp(buf,"finish") == 0 ){

            int sum = 0;
            int sum_run = Sum_Running(Lrunning);
            int sum_wait = Sum_Waiting(c_waiting);
            sum = sum_run + sum_wait;

            fprintf(output_file,"Total time: %d\n",sum);

        }
    }

    fclose(output_file);
    fclose(input_file);
    Elib(Lrunning);
    DistrugeLG(&Lrunning);
    DistrugeLG(&Lmemory);
    free(PID_apar);
    free(buf);
    free(queue);
    DistrQ(c_finished);
    DistrQ(c_waiting);
}

