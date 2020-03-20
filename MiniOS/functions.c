/* SIMIONESCU MARIA-ADRIANA - 312 CB */

#include "header.h"

/* alocare celula pentru lista generica*/
TLG AlocCelulaG( void *ae, size_t d )
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

/* initializeaza lista proceselor */
TProcess_info Init_Process( int x, int y, int t, int mem )
{
    TProcess_info *process;
    void *s;

    process = (TProcess_info*)malloc(sizeof(TProcess_info));

    s = InitS(0);                       
    process->executed_time = 0;         /* timpul executat de proces */
    process->total_memory = mem;        /*memoria totala a unui proces*/
    process->memory = *((TStiva*)s);    /* stiva specifica fiecarui element */
    process->PID = x;                   /* pidul procesului */
    process->priority = y;              
    process->time_of_execution = t;     /*timpul initial de executie */
    process->sum_bytes_stiva = 0;       /* octetii ocupati de stiva */

    return *process;
}

/* initializeaza lista memoriei */
TMemory Init_Memory( int memo, int pid )
{
    TMemory *memory;

    memory = (TMemory*)malloc(sizeof(TMemory));

    memory->start_memory = 0;   /* inceputul zonei de memoriei */
    memory->total_memory = memo;    /*memoria totala */
    memory->PID = pid;             
    memory->end_memory = 0;     /* sfarsitul zonei ocupate de memorie*/

    return *memory;
}

/* distrugere lista generica */
void DistrugeLG( ALG aL )
{
    TLG aux;

    while(*aL){
        aux = *aL;
        *aL = aux->urm;
        free(aux->info);
        free(aux);
    }

}

/* functie care insereaza la sfarsitul unei liste generice */
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

/* cauta primul PID neutilizat */
int Search_Best_PID( int *v )
{
    int i;

    for(i = 1; i < 100; i ++){
        if( v[i] == 0 ) return i;
    }

    return -1;
}

/* functie care interschimba informatiile dintre doua celule*/
void swap( TLG a, TLG b ) 
{ 
    TMemory temp;
    memcpy(&temp,(TMemory*)a->info,sizeof(TMemory)); 
    memcpy((TMemory*)a->info,(TMemory*)b->info,sizeof(TMemory)); 
    memcpy((TMemory*)b->info,&temp,sizeof(TMemory));

} 

/* functie care sorteaza lista specifica memoriei, in functie
        de PID-uri, folosind 2 liste auxiliare cu care iteram 
            prin lista */
void Sortare( TLG L ) 
{ 
    TLG start = L; 
    TLG L1; 
    TLG L2; 

    if( L == NULL ) return;

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



/*  afiseaza continutul stivei unui element */
void print_stack( void *s, FILE *f, int pid )
{
    void *s1 = InitS(0);
	int inf;

   if( ((TStiva*)s)->vf == NULL ) fprintf(f,"Empty stack PID %d.\n",pid);
    else{

        fprintf(f,"Stack of PID %d:",pid); 

	    while( ((TStiva*)s)->vf != NULL ){
		    Pop(s,&inf);
		    Push(s1,&inf);
	    }

	    while( ((TStiva*)s1)->vf != NULL){
            Pop(s1,&inf);
            fprintf(f," %d",inf);
		    Push(s,&inf);
	    } 

        fprintf(f,".\n");
        
	    DistrS(s1);
    }
}

/* se actualizeaza lista de memorie a proceselor in urma 
    terminarii executiei; se sterg celule ce contin 
    inforamtiile corespunzatoare memoriei unui proces */
void Memory_update( ALG aL, int pid )
{
    TLG L = *aL, ant;

    for( ant = NULL; L != NULL; ant = L, L = L->urm ){
        if( ((TMemory*)L->info)->PID == pid ) break;
    }

    if( L == NULL ) return;

    if( ant == NULL ){
        ((TMemory*)L->info)->end_memory = 0;
    }
    else ant->urm = L->urm;

}


/* functie care creeaza lista memoriei proceselor;
    returneaza 1 in cazul care memoria are loc si 0
    in caz contrar */
int InsG( ALG aL, void *ae, size_t d, int *start)
{ 
    TLG p, ant, aux;

    if( *aL == NULL ){
        /* se verifica daca este posibila introducerea procesului */
        if( ((TMemory*)ae)->total_memory < mem_3MiB ){
            /* primul proces incepe de la adresa 0  */
            ((TMemory*)ae)->end_memory = ((TMemory*)ae)->total_memory;
            *start = ((TMemory*)ae)->start_memory;
            InsSfG(aL,ae,d);

            return 1;

        }else return 0;

    }else{
            for(p = *aL, ant = NULL; p != NULL; ant = p, p = p->urm ){

                /* daca ne aflam in interiorul listei */
                if( ant != NULL ){ 
                    /* se cauta primul loc cu memorie libera unde poate fi
                    introdusa memoria procesului */ 
                    if( ((TMemory*)p->info)->start_memory - 
                            ((TMemory*)ant->info)->end_memory >= 
                                        ((TMemory*)ae)->total_memory ){

                        /* inceputul va fi sfarsitul memoriei 
                                            procesului antecedent */ 
                        ((TMemory*)ae)->start_memory =  
                                            ((TMemory*)ant->info)->end_memory;

                        *start = ((TMemory*)ae)->start_memory;

                        ((TMemory*)ae)->end_memory = ((TMemory*)ae)->start_memory + 
                                                        ((TMemory*)ae)->total_memory;
                        /*se adauga memoria in lista */
                        aux = AlocCelulaG(ae,d);
                        ant->urm = aux;
                        aux->urm = p;

                        return 1;
                    }
                }

                if( p->urm != NULL ){
                    /* daca nu ne aflam pe ultima pozitie din lista, 
                        memoria a fost eliberata, iar memoria curenta are loc*/
                    if( ((TMemory*)p->info)->end_memory == 0 && 
                            ((TMemory*)p->urm->info)->start_memory >= 
                                    ((TMemory*)ae)->total_memory ){

                        ((TMemory*)ae)->end_memory = 
                                        ((TMemory*)ae)->start_memory 
                                            + ((TMemory*)ae)->total_memory;

                        *start = ((TMemory*)ae)->start_memory;
                        /* vom copia peste noua informatie */
                        memcpy((TMemory*)p->info,ae,sizeof(TMemory));

                        return 1;
                    }

                }else{
                    /* daca ne aflam pe ultima pozitie verificam daca 
                    memoria are loc si o adaugam in lista */
                    if( ((TMemory*)p->info)->end_memory + 
                                ((TMemory*)ae)->total_memory < mem_3MiB ){

                        ((TMemory*)ae)->start_memory =  
                                        ((TMemory*)p->info)->end_memory;

                        ((TMemory*)ae)->end_memory = 
                                        ((TMemory*)ae)->start_memory + 
                                            ((TMemory*)ae)->total_memory;

                        InsSfG(aL,ae,d);

                        *start = ((TMemory*)ae)->start_memory;

                        return 1;

                    }else return 0;
                }
            }
        }

    return 0;
}

/* lista de memorie este sortata crescator in functie de PID-uri;
    functia muta memoria la adresa 0+dimensiunea memoriei 
    procesului cu PID-ul cel mai mic */
void Defragmentare( ALG aL )
{
    TLG L = *aL, ant;

    for( ant = NULL; L != NULL; ant = L, L = L ->urm ){
        if( L == *aL ){
            ((TMemory*)L->info)->start_memory = 0;
            ((TMemory*)L->info)->end_memory = 
                        ((TMemory*)L->info)->start_memory + 
                            ((TMemory*)L->info)->total_memory;
        }else{
            ((TMemory*)L->info)->start_memory =  
                            ((TMemory*)ant->info)->end_memory;
            ((TMemory*)L->info)->end_memory = 
                            ((TMemory*)L->info)->start_memory + 
                                ((TMemory*)L->info)->total_memory;
        }
    }
}

/* stergerea stivelor proceselor */
void Elib(TLG L)
{
    for( ; L != NULL; L = L->urm ){
        DistrS(&((TProcess_info*)L->info)->memory);
    }
}

/* functie care aloca memorie pentru buffer */
void memory_allocation_buffer(int width, char **buffer)
{
    *buffer = (char*)malloc(width*sizeof(char));
}

/* functie care aloca memorie pentru un vector */
void memory_allocation_array(int width, int **array)
{
    *array = (int*)malloc(width*sizeof(int));
}
