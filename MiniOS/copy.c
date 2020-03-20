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

/* creeaza stiva vida cu elemente de dimensiune d */
void *InitS( size_t d )
{
    TStiva *s;

    s = (TStiva*)malloc(sizeof(TStiva));
    if(!s) return NULL;

    s->dime = d;
    s->vf = NULL;

    return (void*)s;
}

/* creeaza coada vida cu elemente de dimensiune d */
void *InitQ( size_t d )
{
    TCoada *c;

    c = (TCoada*)malloc(sizeof(TCoada));
    if(!c) return NULL;

    c->dime = d;
    c->ic = c->sc = NULL;

    return (void*)c;
}

/* adauga element la sfarsitul cozii */
void IntrQ( void *c, void *ae, size_t d )   
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

/* extrage primul element din coada la adresa ae */
void ExtrgQ( void *c, void *ae )
{
    TLG aux;

    aux = ((TCoada*)c)->ic;

    if(aux != NULL){

    if(((TCoada*)c)->ic != ((TCoada*)c)->sc)  ((TCoada*)c)->ic = aux->urm;
    else ((TCoada*)c)->ic = ((TCoada*)c)->sc = NULL;

    /* copiaza elementul de la inceputul cozii */
    memcpy(ae,(TProcess_info*)aux->info,sizeof(TProcess_info));  

    (((TCoada*)c)->dime)--; /*scade numarul de elemente din coada */

    free(aux->info);
    free(aux);

    }
}

/* extrage elementul din varful stivei la adresa ae */
void Pop( void *s, void *ae )
{
    TLG aux;

    /* copiaza elementul scos din stiva */
    memcpy(ae,((TStiva*)s)->vf->info,sizeof(int));  

    aux = ((TStiva*)s)->vf;

    if( aux != NULL ){

    ((TStiva*)s)->vf = aux->urm;    /* deplaseaza varful stivei */

    (((TStiva*)s)->dime)--;     /* scade numarul de elemente din stiva*/

    free(aux->info);
    free(aux);

    }
}

/* distruge stiva */
void DistrS( void *s )
{
    int x;

    while( ((TStiva*)s)->vf != NULL ){
		Pop(s,&x);
    }

}

/* distruge coada */
void DistrQ( void *c )
{
    TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        DistrS(&(inf.memory));/* distruge stiva corespunzatoare unui proces */
	}
}

/* sorteaza coada de asteptare folosind 2 cozi auxiliare;
    pentru fiecare element introdus in coada se verifica prioritatea,
    timpul de executie si PID-ul */
void SortareWaiting( void *c,void *ae )
{
    void *c1 = InitQ(0);
    void *c2 = InitQ(0);

    int ok = 0;
    TProcess_info inf;
    TProcess_info p2 = *((TProcess_info*)ae);

	while( ((TCoada*)c)->sc != NULL ){

		ExtrgQ(c,&inf);
        int done1 = 0, done = 0;

        /* se pun in coada c2 elementele cu prioritate mai mica decat cea
        a elementului ce trebuie introdus, inclusiv acesta */
        if( inf.priority < p2.priority && ok == 0 ){ 
            IntrQ(c2,&p2,sizeof(TProcess_info));
            ok = 1;
            done = 1;
            IntrQ(c2,&inf,sizeof(TProcess_info));
        }

        if( inf.priority < p2.priority && ok == 1 && done == 0 ) done1 = 1;
        

        /* se pun in coada c2 elementele cu timp de executie mai mare decat cel
        a elementului ce trebuie introdus, inclusiv acesta, in cazul 
        prioritatilor egale */
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

         /* se pun in coada c2 elementele cu pid-ul mai mare decat cel
        al elementului ce trebuie introdus, inclusiv acesta, in cazul 
        prioritatilor egale si timpilor de executie egali */
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
            inf.time_of_execution == p2.time_of_execution 
                && ok == 1 && done == 0){

            if( inf.PID > p2.PID ){
                done1 = 1;
            }

        }
        
        /*elementele care respecta conditiile dar care 
            nu au fost adaugate in coada*/
        if( done1 == 1 ) IntrQ(c2,&inf,sizeof(TProcess_info));

        /* se pun in prima coada elemetele care nu verifica conditiile anterioare;
        respectiv restul elementelor */
        if( ok == 0 ) IntrQ(c1,&inf,sizeof(TProcess_info));

	}

    /*daca nu a fost gasit niciun element care sa incalce 
    regulile elemetul ce trebuie adaugat este pus in prima coada*/
    if(ok == 0) IntrQ(c1,&p2,sizeof(TProcess_info));

    /* se construieste coada sortata punand la inceput elemetele
    din prima coada auxiliara si la sfarsit elementele din 
    a doua coada auxiliara */

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

/* functie care cauta in lista running un proces in functie de
    PID-ul sau si intoarce 1-exista, 0-nu exista  si prin efect
    lateral si timpul de executie */
int SearchRunning( TLG L, int pid, int long long *time )
{
    for(; L != NULL; L = L->urm ){ 
        if( ((TProcess_info*)L->info)->PID == pid ){

            *time = ((TProcess_info*)L->info)->time_of_execution;
            return 1;

        }
    }

    return 0;
}

/* functie care afiseaza coada de asteptare */ 
void Print_Wainting( void *c, FILE *f )
{
    void *c1 = InitQ(0);
	TProcess_info inf; 

    /* numarul de elemente din coada de asteptare */
    size_t dime = ((TCoada*)c)->dime;
    int count = 0;

    fprintf(f,"[");
	while( ((TCoada*)c)->sc != NULL ){
        count++;
		ExtrgQ(c,&inf);

        /* verificam daca este ultimul element afisat */ 
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

/* functie care afiseaza coada finished */
void Print_Finished( void *c, FILE *f )
{
    void *c1 = InitQ(0);
	TProcess_info inf; 

    /*elementele din coada finished */
    size_t dime =  ((TCoada*)c)->dime; 
    int count = 0;

    fprintf(f,"[");
	while( ((TCoada*)c)->sc != NULL ){
        count++;
		ExtrgQ(c,&inf);

        /* se verifica daca este ultimul element afisat */
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

/* pune element in varful stivei */
void Push( void *s,void *ae )
{
    TLG aux;

    aux = AlocCelulaG(ae,sizeof(int));
    if(!aux) return;

    if(((TStiva*)s)->vf != NULL){
        aux->urm = ((TStiva*)s)->vf;
        ((TStiva*)s)->vf = aux; 
    }else{
        ((TStiva*)s)->vf = aux;
    }

    /* creste numarul de elemente din stiva */
    (((TStiva*)s)->dime)++;
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

/* cauta un proces in functie de pid si intoarce 1-exista, 0-nu exista 
    si prin efect lateral, timpul de executie*/
int SearchWaiting( void *c, int pid, int long long *time )
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

/* cauta un proces in coada finished in functie de pid si 
    intoarce 1-exista, 0-nu exista si prin efect lateral, 
    timpul de executie*/
int SearchFinished( void *c, int pid, FILE *f )
{
    void *c1 = InitQ(0);

	TProcess_info inf;
    int found = 0;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf); 

        if( inf.PID == pid ){
            fprintf(f,"Process %d is finished.\n",pid);
            found = 1;
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

/* se adauga pe stiva procesului 4 octeti */
void Run_and_Push( TLG L, int pid, int bytes, FILE *f )
{
    for(; L != NULL; L = L->urm ){
        if( ((TProcess_info*)L->info)->PID == pid ) {

            /* verificam daca mai incap octeti pe stiva procesului */
            if( 4 + ((TProcess_info*)L->info)->sum_bytes_stiva <= 
                            ((TProcess_info*)L->info)->total_memory ){
                /* daca da, se tine cont de suma acestora */
                ((TProcess_info*)L->info)->sum_bytes_stiva += 4;
                Push(&((TProcess_info*)L->info)->memory,&bytes);

            }else{
                fprintf(f,"Stack overflow PID %d.\n",pid);
                break;
            }
            break;
        } 
    }
}

/* se scot din stiva procesului 4 octeti */
void Run_and_Pop( TLG L, int pid, FILE *f )
{
    int bytes = 0;

    for(; L != NULL; L = L->urm ){
        if( ((TProcess_info*)L->info)->PID == pid ) {

            TProcess_info inf = *((TProcess_info*)L->info);

            /* se verifica daca stiva e goala */
            if( inf.memory.vf == NULL ){
                fprintf(f,"Empty stack PID %d.\n",pid);
            }
            else {
                /* sunt eliminati cei 4 octeti din stiva si 
                se tine cont de acetia in suma pastrata */
                ((TProcess_info*)L->info)->sum_bytes_stiva -= 4;
                Pop(&((TProcess_info*)L->info)->memory,&bytes);
            }
            break;
        } 
    }
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

/* functie care actualizeaza timpul de executie si timpul executat
    pentru procesele care ruleaza singure, prin variabila ok se 
    verifica daca procesul si-a terminat executia */
void Run_Time( TLG L, int time, void *c, int *ok )
{
    *ok = 0;

    for( ; L != NULL; L = L->urm ){

        if( time < ((TProcess_info*)L->info)->time_of_execution ){
            ((TProcess_info*)L->info)->time_of_execution -= time;
            ((TProcess_info*)L->info)->executed_time += time;
        }else{
            /* daca timpul de rulare este mai mare decat cel posibil de 
            executie procesul se termina si se adauga in coada de astepatare */
            ((TProcess_info*)L->info)->executed_time += 
                        ((TProcess_info*)L->info)->time_of_execution;
            ((TProcess_info*)L->info)->time_of_execution = 0;

            IntrQ(c,((TProcess_info*)L->info),sizeof(TProcess_info));
            *ok = 1;
        }

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

/* functia actualizeaza timpul de executie si timpul executat 
    de un proces in functie de cuanta de timp si de timpul in care
    procesele trebuie sa ruleze */
void Run_Time_2( ALG aL,int time, int TIME, void *c_f, void *c_w, int *v,
                            TLG L,int *rest, int *exec_time, int *process )
{
    TProcess_info info, info1;
    TLG p = *aL;
    int ok = 0;

    if( p != NULL ){

         /* daca un proces a rulat un timp mai mic decat cuanta inseamna ca 
         acesta trebuie sa isi continue rularea si mai are de executat 
         restul de timp dintre cuanta de timp si timpul pe care l-a executat */

        if( *rest != 0 ){ 
            TIME = *rest;
            ok = 1; 
            *exec_time = *rest;
        }

        if( time < TIME ){

            /*daca timpul de rulare ramas este mai mic decat cel de executie 
                al procesului din starea running, acesta va rula pana
                cand timpul de rulare se termina si se va pastra restul de 
                timp pe care procesul il mai poate efectua */
            if( time  < ((TProcess_info*)p->info)->time_of_execution ){

                ((TProcess_info*)p->info)->time_of_execution -= time;
                ((TProcess_info*)p->info)->executed_time += time;
 
                *rest = TIME - time;
                ok = 1;
                *exec_time = time;

            }else{
                
                /* daca timpul de rulare este mai mare decat cel de executie
                 al procesului si acesta este de asemenea mai mic decat decat
                  cuanta de timp, procesul isi va incheia executia */
                ((TProcess_info*)p->info)->executed_time += 
                                ((TProcess_info*)p->info)->time_of_execution;
                ((TProcess_info*)p->info)->time_of_execution = 0;
                /* PID-ul este eliberat */
                memset(&v[((TProcess_info*)p->info)->PID ],0,sizeof(int));
                /* memoria este eliberata */
                Memory_update(&L,((TProcess_info*)p->info)->PID);
                /* numarul de procese scade */
                (*process)--;
                /* procesul este pus in coada finished */
                IntrQ(c_f,((TProcess_info*)p->info),sizeof(TProcess_info));
                /* este extras din coada de asteptare primul proces */
                ExtrgQ(c_w,&info);
                /* si este plasat in lista running */
                memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
                *rest  = 0;
            }

        }else{
        
            if( TIME  < ((TProcess_info*)p->info)->time_of_execution ){
                
                /* procesul va rula la capacitate maxima */
                ((TProcess_info*)p->info)->time_of_execution -= TIME;
                ((TProcess_info*)p->info)->executed_time += TIME;
                /* este extras din coada de asteptare primul proces */
                ExtrgQ(c_w,&info);
                /* este copiata informatia elementului din starea running */
                memcpy(&info1,((TProcess_info*)p->info),sizeof(TProcess_info));
                /* procesul extras este plasat in lista running */
                memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
                /* este adaugat in coada de asteptare elementul 
                                                        din starea running */
                SortareWaiting(c_w,&info1);
                *rest  = 0;

            }else{

                ((TProcess_info*)p->info)->executed_time += 
                            ((TProcess_info*)p->info)->time_of_execution;
                ok = 1;
                /* timpul de rulare va fi timpul maxim de 
                                                executie al procesului */
                *exec_time  = ((TProcess_info*)p->info)->time_of_execution;

                ((TProcess_info*)p->info)->time_of_execution = 0;
                /* PID-ul este eliberat */
                memset(&v[((TProcess_info*)p->info)->PID ],0,sizeof(int));
                /* memoria este eliberata */
                Memory_update(&L,((TProcess_info*)p->info)->PID);
                /* numarul de procese scade */
                (*process)--;
                /* procesul este pus in coada finished */
                IntrQ(c_f,((TProcess_info*)p->info),sizeof(TProcess_info));
                /* este extras din coada de asteptare primul proces */
                ExtrgQ(c_w,&info);
                /* si este plasat in lista running */
                memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
                *rest = 0;

            }
        }

        /* timpul de executie va fi chiar cuanta in cazurile in 
                                                            care este posinil*/
        if( ok == 0 ) *exec_time = TIME;
    }
 
}

/* elimina 4 octeti din varful stivei procesului din coada de astepare */
void Wait_and_Pop( void *c, int pid, FILE *f )
{
    void *c1 = InitQ(0);

	TProcess_info inf;
    int information = 0;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);

        if( inf.PID == pid ) {
            if( inf.memory.vf  == NULL ) fprintf(f,"Empty stack PID %d.\n",pid);
            else {
                inf.sum_bytes_stiva -= 4;
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

/* cauta in lista running procesul pentru care se va face afisarea stivei */
void Run_Stack( TLG L, int pid, FILE *F )
{
    for(; L != NULL; L = L->urm ){

        if( ((TProcess_info*)L->info)->PID == pid ) {
            print_stack(&(((TProcess_info*)L->info)->memory),F,pid);
            break;
        } 
    }
}

/* cauta in coada de asteptare procesul pentru care se va face afisarea stivei */
void Wait_Stack( void *c, int pid, FILE *f )
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

/* cauta in coada de asteptare elementul caruia ii vor fi adugati in stiva
    4 octeti */
void Wait_and_Push( void *c, int pid, int bytes, FILE *f )
{
    void *c1 = InitQ(0);
	TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);

        if( inf.PID == pid ) {
            /* se tine cont de numarul de octeti adaugati in stiva procesului */
            if( 4 + inf.sum_bytes_stiva <= inf.total_memory ){
                inf.sum_bytes_stiva += 4;
                Push(&inf.memory,&bytes);
            }else{
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

/* functia returneaza suma timpului de executie al proceselor din 
        starea running */ 
int Sum_Running( TLG L )
{
    int sum = 0;

    for(; L != NULL; L = L->urm ){
       sum += ((TProcess_info*)L->info)->time_of_execution;
    }

    return sum;
}

/* functia returneaza suma timpului de executie al proceselor din 
        coada de asteptare */
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

int main(int argc, char *argv[])
{
    char *buf, *queue;          
    int *PID_apar;
    int TIME, memory_size, time_of_execution, priority, PID, time, bytes;
    int rest = 0; 

    TProcess_info inf;
    int process_count = 0;

    void *c_waiting, *c_finished;

    if (argc != 3) {  /* numar invalid de argumente */
        fprintf(stderr,
            "Usage: %s operations_string input_file output_file\n",
            argv[0]);
        return -1;
    }

    /* se deschide fisierul de input pentru citire */ 
    FILE *input_file = fopen(argv[1], "r");
    /* se deschide fisierul de output pentru scriere */ 
    FILE *output_file = fopen(argv[2], "aw");

    if( input_file == NULL ){
        fprintf(stderr, "File \"%s\" not found\n", argv[1]);
        return -2;
    }

    /* se citeste cuanta de timp */
    fscanf(input_file,"%d\n",&TIME);

    memory_allocation_buffer(200,&buf);
    memory_allocation_buffer(20,&queue);
    memory_allocation_array(200,&PID_apar);

    /* vector de aparitii pentru PID-uri */
    memset(PID_apar,0,100);

    /* initializarea cozilor */
    c_waiting = InitQ(0);
    c_finished = InitQ(0);

    TLG Lrunning = NULL;
    TLG Lmemory = NULL;

    /* se citesc datele din fisierul de iesire */
    while(fscanf(input_file,"%s",buf) > 0){

        if( strcmp(buf,"add") == 0 ){

            fscanf(input_file,"%d %d %d",&memory_size,
                                            &time_of_execution,&priority);

            int pid = 0,start = 0;
            pid = Search_Best_PID(PID_apar);

            PID_apar[pid] = 1; /* pid ocupat */

            TMemory memory;
            memory = Init_Memory(memory_size,pid);

            /* se verifica alocarea memoriei unui proces */
            int memo = InsG(&Lmemory,&memory,sizeof(TMemory),&start);

            if( memo == 1 ){
            
                fprintf(output_file,
                "Process created successfully: PID: %d,",pid); 
                fprintf(output_file," Memory starts at 0x%x.\n",start);
                
                process_count++;
                /* in caz afirmativ se adauga procesul in starea de running 
                daca este primul proces din sistem sau in coada de asteptare */

                inf = Init_Process(pid,priority,time_of_execution,memory_size);

                if( process_count == 1 ){ 
                    InsSfG(&Lrunning,&inf,sizeof(TProcess_info)); 
                }
                else SortareWaiting(c_waiting,&inf);

                }else{
                    /* in caz negativ, se realizeaza defragmentarea si se
                    reia procesul */

                    Sortare(Lmemory);
                    Defragmentare(&Lmemory);

                    memo = InsG(&Lmemory,&memory,sizeof(TMemory),&start);

                    if( memo == 1 ){

                        fprintf(output_file,
                        "Process created successfully: PID: %d,",pid); 
                        fprintf(output_file," Memory starts at 0x%x.\n",start);
                
                        process_count++;

                        inf = Init_Process(pid,priority,
                                            time_of_execution,memory_size);

                        if( process_count == 1 ){ 
                            InsSfG(&Lrunning,&inf,sizeof(TProcess_info));
                        }
                        else SortareWaiting(c_waiting,&inf);

                    }else{
                        /* in cazul in care procesul nu are loc 
                                            este eliberat PID-ul*/
                        PID_apar[pid] = 0;
                        fprintf(output_file,
                        "Cannot reserve memory for PID %d.\n",pid);

                    }
                }
        }

        if( strcmp(buf,"get") == 0 ){

            fscanf(input_file,"%d",&PID);

            int long long remaining_time;
            int run = SearchRunning(Lrunning,PID,&remaining_time);
            int wait = SearchWaiting(c_waiting,PID,&remaining_time);

            if( run == 1 ) fprintf(output_file,
                        "Process %d is running (remaining_time: %lld).\n"
                                                    ,PID,remaining_time);
            if( wait == 1 ) fprintf(output_file,
                        "Process %d is waiting (remaining_time: %lld).\n"
                                                    ,PID,remaining_time);
            
            int finish = SearchFinished(c_finished,PID,output_file);

            if( run == 0 && wait == 0 && finish == 0 ){
                fprintf(output_file,"Process %d not found.\n",PID);
            }
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
                if( wait == 1 ) Wait_Stack(c_waiting,PID,output_file);

                if( run == 0 && wait == 0 ){
                    fprintf(output_file,"PID %d not found.\n",PID);
                }
            }
        }

        if( strcmp(buf,"run") == 0 ){

            fscanf(input_file,"%d",&time);

            int ok = 0;
            int p = ((TProcess_info*)Lrunning->info)->PID;

            if( process_count == 1 ){
        
                Run_Time(Lrunning,time,c_finished,&ok);
                /* daca procesul este singurul din sistem si acesta si-a 
                terminat executia */    
                if( ok == 1 ){
                    /* memoria va fi eliberata */
                    Memory_update(&Lmemory,p);
                    /* PID-ul va fi eliberat */
                    PID_apar[p] = 0; 
                    /* numarul de procese scade */
                    process_count--;
                    /* listele de memorie si running devin nule */
                    Lrunning = NULL;
                    Lmemory = NULL;
                }
    
            }else{

                while(time){
                    int exec_time = 0;
                    
                    Run_Time_2(&Lrunning,time,TIME,c_finished,c_waiting,PID_apar,
                                        Lmemory,&rest,&exec_time,&process_count);
                    time = time - exec_time;

                    if( process_count == 1 ){
                        /* daca procesul este singurul din sistem si acesta si-a 
                        terminat executia */ 
                        Run_Time(Lrunning,time,c_finished,&ok);

                        if( ok == 1 ){
                            Memory_update(&Lmemory,p);
                            PID_apar[p] = 0; 
                            process_count--;
                            Lrunning = NULL;
                            Lmemory = NULL;
                        }
                        break;
                    }
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

            if( run == 0 && wait == 0 ){ 
                fprintf(output_file,"PID %d not found.\n",PID);
            }

        }

        if( strcmp(buf,"pop") == 0 ){
            fscanf(input_file,"%d",&PID);

            int long long remaining_time;
            int run = SearchRunning(Lrunning,PID,&remaining_time);
            int wait = SearchWaiting(c_waiting,PID,&remaining_time);

            if( run == 1 ) Run_and_Pop(Lrunning,PID,output_file);
            if( wait == 1 ) Wait_and_Pop(c_waiting,PID,output_file);

            if( run == 0 && wait == 0 ){ 
                fprintf(output_file,"PID %d not found.\n",PID);
            }

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

/* creeaza stiva vida cu elemente de dimensiune d */
void *InitS( size_t d )
{
    TStiva *s;

    s = (TStiva*)malloc(sizeof(TStiva));
    if(!s) return NULL;

    s->dime = d;
    s->vf = NULL;

    return (void*)s;
}

/* creeaza coada vida cu elemente de dimensiune d */
void *InitQ( size_t d )
{
    TCoada *c;

    c = (TCoada*)malloc(sizeof(TCoada));
    if(!c) return NULL;

    c->dime = d;
    c->ic = c->sc = NULL;

    return (void*)c;
}

/* adauga element la sfarsitul cozii */
void IntrQ( void *c, void *ae, size_t d )   
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

/* extrage primul element din coada la adresa ae */
void ExtrgQ( void *c, void *ae )
{
    TLG aux;

    aux = ((TCoada*)c)->ic;

    if(aux != NULL){

    if(((TCoada*)c)->ic != ((TCoada*)c)->sc)  ((TCoada*)c)->ic = aux->urm;
    else ((TCoada*)c)->ic = ((TCoada*)c)->sc = NULL;

    /* copiaza elementul de la inceputul cozii */
    memcpy(ae,(TProcess_info*)aux->info,sizeof(TProcess_info));  

    (((TCoada*)c)->dime)--; /*scade numarul de elemente din coada */

    free(aux->info);
    free(aux);

    }
}

/* extrage elementul din varful stivei la adresa ae */
void Pop( void *s, void *ae )
{
    TLG aux;

    /* copiaza elementul scos din stiva */
    memcpy(ae,((TStiva*)s)->vf->info,sizeof(int));  

    aux = ((TStiva*)s)->vf;

    if( aux != NULL ){

    ((TStiva*)s)->vf = aux->urm;    /* deplaseaza varful stivei */

    (((TStiva*)s)->dime)--;     /* scade numarul de elemente din stiva*/

    free(aux->info);
    free(aux);

    }
}

/* distruge stiva */
void DistrS( void *s )
{
    int x;

    while( ((TStiva*)s)->vf != NULL ){
		Pop(s,&x);
    }

}

/* distruge coada */
void DistrQ( void *c )
{
    TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        DistrS(&(inf.memory));/* distruge stiva corespunzatoare unui proces */
	}
}

/* sorteaza coada de asteptare folosind 2 cozi auxiliare;
    pentru fiecare element introdus in coada se verifica prioritatea,
    timpul de executie si PID-ul */
void SortareWaiting( void *c,void *ae )
{
    void *c1 = InitQ(0);
    void *c2 = InitQ(0);

    int ok = 0;
    TProcess_info inf;
    TProcess_info p2 = *((TProcess_info*)ae);

	while( ((TCoada*)c)->sc != NULL ){

		ExtrgQ(c,&inf);
        int done1 = 0, done = 0;

        /* se pun in coada c2 elementele cu prioritate mai mica decat cea
        a elementului ce trebuie introdus, inclusiv acesta */
        if( inf.priority < p2.priority && ok == 0 ){ 
            IntrQ(c2,&p2,sizeof(TProcess_info));
            ok = 1;
            done = 1;
            IntrQ(c2,&inf,sizeof(TProcess_info));
        }

        if( inf.priority < p2.priority && ok == 1 && done == 0 ) done1 = 1;
        

        /* se pun in coada c2 elementele cu timp de executie mai mare decat cel
        a elementului ce trebuie introdus, inclusiv acesta, in cazul 
        prioritatilor egale */
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

         /* se pun in coada c2 elementele cu pid-ul mai mare decat cel
        al elementului ce trebuie introdus, inclusiv acesta, in cazul 
        prioritatilor egale si timpilor de executie egali */
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
            inf.time_of_execution == p2.time_of_execution 
                && ok == 1 && done == 0){

            if( inf.PID > p2.PID ){
                done1 = 1;
            }

        }
        
        /*elementele care respecta conditiile dar care 
            nu au fost adaugate in coada*/
        if( done1 == 1 ) IntrQ(c2,&inf,sizeof(TProcess_info));

        /* se pun in prima coada elemetele care nu verifica conditiile anterioare;
        respectiv restul elementelor */
        if( ok == 0 ) IntrQ(c1,&inf,sizeof(TProcess_info));

	}

    /*daca nu a fost gasit niciun element care sa incalce 
    regulile elemetul ce trebuie adaugat este pus in prima coada*/
    if(ok == 0) IntrQ(c1,&p2,sizeof(TProcess_info));

    /* se construieste coada sortata punand la inceput elemetele
    din prima coada auxiliara si la sfarsit elementele din 
    a doua coada auxiliara */

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

/* functie care cauta in lista running un proces in functie de
    PID-ul sau si intoarce 1-exista, 0-nu exista  si prin efect
    lateral si timpul de executie */
int SearchRunning( TLG L, int pid, int long long *time )
{
    for(; L != NULL; L = L->urm ){ 
        if( ((TProcess_info*)L->info)->PID == pid ){

            *time = ((TProcess_info*)L->info)->time_of_execution;
            return 1;

        }
    }

    return 0;
}

/* functie care afiseaza coada de asteptare */ 
void Print_Wainting( void *c, FILE *f )
{
    void *c1 = InitQ(0);
	TProcess_info inf; 

    /* numarul de elemente din coada de asteptare */
    size_t dime = ((TCoada*)c)->dime;
    int count = 0;

    fprintf(f,"[");
	while( ((TCoada*)c)->sc != NULL ){
        count++;
		ExtrgQ(c,&inf);

        /* verificam daca este ultimul element afisat */ 
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

/* functie care afiseaza coada finished */
void Print_Finished( void *c, FILE *f )
{
    void *c1 = InitQ(0);
	TProcess_info inf; 

    /*elementele din coada finished */
    size_t dime =  ((TCoada*)c)->dime; 
    int count = 0;

    fprintf(f,"[");
	while( ((TCoada*)c)->sc != NULL ){
        count++;
		ExtrgQ(c,&inf);

        /* se verifica daca este ultimul element afisat */
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

/* pune element in varful stivei */
void Push( void *s,void *ae )
{
    TLG aux;

    aux = AlocCelulaG(ae,sizeof(int));
    if(!aux) return;

    if(((TStiva*)s)->vf != NULL){
        aux->urm = ((TStiva*)s)->vf;
        ((TStiva*)s)->vf = aux; 
    }else{
        ((TStiva*)s)->vf = aux;
    }

    /* creste numarul de elemente din stiva */
    (((TStiva*)s)->dime)++;
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

/* cauta un proces in functie de pid si intoarce 1-exista, 0-nu exista 
    si prin efect lateral, timpul de executie*/
int SearchWaiting( void *c, int pid, int long long *time )
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

/* cauta un proces in coada finished in functie de pid si 
    intoarce 1-exista, 0-nu exista si prin efect lateral, 
    timpul de executie*/
int SearchFinished( void *c, int pid, FILE *f )
{
    void *c1 = InitQ(0);

	TProcess_info inf;
    int found = 0;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf); 

        if( inf.PID == pid ){
            fprintf(f,"Process %d is finished.\n",pid);
            found = 1;
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

/* se adauga pe stiva procesului 4 octeti */
void Run_and_Push( TLG L, int pid, int bytes, FILE *f )
{
    for(; L != NULL; L = L->urm ){
        if( ((TProcess_info*)L->info)->PID == pid ) {

            /* verificam daca mai incap octeti pe stiva procesului */
            if( 4 + ((TProcess_info*)L->info)->sum_bytes_stiva <= 
                            ((TProcess_info*)L->info)->total_memory ){
                /* daca da, se tine cont de suma acestora */
                ((TProcess_info*)L->info)->sum_bytes_stiva += 4;
                Push(&((TProcess_info*)L->info)->memory,&bytes);

            }else{
                fprintf(f,"Stack overflow PID %d.\n",pid);
                break;
            }
            break;
        } 
    }
}

/* se scot din stiva procesului 4 octeti */
void Run_and_Pop( TLG L, int pid, FILE *f )
{
    int bytes = 0;

    for(; L != NULL; L = L->urm ){
        if( ((TProcess_info*)L->info)->PID == pid ) {

            TProcess_info inf = *((TProcess_info*)L->info);

            /* se verifica daca stiva e goala */
            if( inf.memory.vf == NULL ){
                fprintf(f,"Empty stack PID %d.\n",pid);
            }
            else {
                /* sunt eliminati cei 4 octeti din stiva si 
                se tine cont de acetia in suma pastrata */
                ((TProcess_info*)L->info)->sum_bytes_stiva -= 4;
                Pop(&((TProcess_info*)L->info)->memory,&bytes);
            }
            break;
        } 
    }
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

/* functie care actualizeaza timpul de executie si timpul executat
    pentru procesele care ruleaza singure, prin variabila ok se 
    verifica daca procesul si-a terminat executia */
void Run_Time( TLG L, int time, void *c, int *ok )
{
    *ok = 0;

    for( ; L != NULL; L = L->urm ){

        if( time < ((TProcess_info*)L->info)->time_of_execution ){
            ((TProcess_info*)L->info)->time_of_execution -= time;
            ((TProcess_info*)L->info)->executed_time += time;
        }else{
            /* daca timpul de rulare este mai mare decat cel posibil de 
            executie procesul se termina si se adauga in coada de astepatare */
            ((TProcess_info*)L->info)->executed_time += 
                        ((TProcess_info*)L->info)->time_of_execution;
            ((TProcess_info*)L->info)->time_of_execution = 0;

            IntrQ(c,((TProcess_info*)L->info),sizeof(TProcess_info));
            *ok = 1;
        }

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

/* functia actualizeaza timpul de executie si timpul executat 
    de un proces in functie de cuanta de timp si de timpul in care
    procesele trebuie sa ruleze */
void Run_Time_2( ALG aL,int time, int TIME, void *c_f, void *c_w, int *v,
                            TLG L,int *rest, int *exec_time, int *process )
{
    TProcess_info info, info1;
    TLG p = *aL;
    int ok = 0;

    if( p != NULL ){

         /* daca un proces a rulat un timp mai mic decat cuanta inseamna ca 
         acesta trebuie sa isi continue rularea si mai are de executat 
         restul de timp dintre cuanta de timp si timpul pe care l-a executat */

        if( *rest != 0 ){ 
            TIME = *rest;
            ok = 1; 
            *exec_time = *rest;
        }

        if( time < TIME ){

            /*daca timpul de rulare ramas este mai mic decat cel de executie 
                al procesului din starea running, acesta va rula pana
                cand timpul de rulare se termina si se va pastra restul de 
                timp pe care procesul il mai poate efectua */
            if( time  < ((TProcess_info*)p->info)->time_of_execution ){

                ((TProcess_info*)p->info)->time_of_execution -= time;
                ((TProcess_info*)p->info)->executed_time += time;
 
                *rest = TIME - time;
                ok = 1;
                *exec_time = time;

            }else{
                
                /* daca timpul de rulare este mai mare decat cel de executie
                 al procesului si acesta este de asemenea mai mic decat decat
                  cuanta de timp, procesul isi va incheia executia */
                ((TProcess_info*)p->info)->executed_time += 
                                ((TProcess_info*)p->info)->time_of_execution;
                ((TProcess_info*)p->info)->time_of_execution = 0;
                /* PID-ul este eliberat */
                memset(&v[((TProcess_info*)p->info)->PID ],0,sizeof(int));
                /* memoria este eliberata */
                Memory_update(&L,((TProcess_info*)p->info)->PID);
                /* numarul de procese scade */
                (*process)--;
                /* procesul este pus in coada finished */
                IntrQ(c_f,((TProcess_info*)p->info),sizeof(TProcess_info));
                /* este extras din coada de asteptare primul proces */
                ExtrgQ(c_w,&info);
                /* si este plasat in lista running */
                memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
                *rest  = 0;
            }

        }else{
        
            if( TIME  < ((TProcess_info*)p->info)->time_of_execution ){
                
                /* procesul va rula la capacitate maxima */
                ((TProcess_info*)p->info)->time_of_execution -= TIME;
                ((TProcess_info*)p->info)->executed_time += TIME;
                /* este extras din coada de asteptare primul proces */
                ExtrgQ(c_w,&info);
                /* este copiata informatia elementului din starea running */
                memcpy(&info1,((TProcess_info*)p->info),sizeof(TProcess_info));
                /* procesul extras este plasat in lista running */
                memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
                /* este adaugat in coada de asteptare elementul 
                                                        din starea running */
                SortareWaiting(c_w,&info1);
                *rest  = 0;

            }else{

                ((TProcess_info*)p->info)->executed_time += 
                            ((TProcess_info*)p->info)->time_of_execution;
                ok = 1;
                /* timpul de rulare va fi timpul maxim de 
                                                executie al procesului */
                *exec_time  = ((TProcess_info*)p->info)->time_of_execution;

                ((TProcess_info*)p->info)->time_of_execution = 0;
                /* PID-ul este eliberat */
                memset(&v[((TProcess_info*)p->info)->PID ],0,sizeof(int));
                /* memoria este eliberata */
                Memory_update(&L,((TProcess_info*)p->info)->PID);
                /* numarul de procese scade */
                (*process)--;
                /* procesul este pus in coada finished */
                IntrQ(c_f,((TProcess_info*)p->info),sizeof(TProcess_info));
                /* este extras din coada de asteptare primul proces */
                ExtrgQ(c_w,&info);
                /* si este plasat in lista running */
                memcpy(((TProcess_info*)p->info),&info,sizeof(TProcess_info));
                *rest = 0;

            }
        }

        /* timpul de executie va fi chiar cuanta in cazurile in 
                                                            care este posinil*/
        if( ok == 0 ) *exec_time = TIME;
    }
 
}

/* elimina 4 octeti din varful stivei procesului din coada de astepare */
void Wait_and_Pop( void *c, int pid, FILE *f )
{
    void *c1 = InitQ(0);

	TProcess_info inf;
    int information = 0;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);

        if( inf.PID == pid ) {
            if( inf.memory.vf  == NULL ) fprintf(f,"Empty stack PID %d.\n",pid);
            else {
                inf.sum_bytes_stiva -= 4;
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

/* cauta in lista running procesul pentru care se va face afisarea stivei */
void Run_Stack( TLG L, int pid, FILE *F )
{
    for(; L != NULL; L = L->urm ){

        if( ((TProcess_info*)L->info)->PID == pid ) {
            print_stack(&(((TProcess_info*)L->info)->memory),F,pid);
            break;
        } 
    }
}

/* cauta in coada de asteptare procesul pentru care se va face afisarea stivei */
void Wait_Stack( void *c, int pid, FILE *f )
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

/* cauta in coada de asteptare elementul caruia ii vor fi adugati in stiva
    4 octeti */
void Wait_and_Push( void *c, int pid, int bytes, FILE *f )
{
    void *c1 = InitQ(0);
	TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);

        if( inf.PID == pid ) {
            /* se tine cont de numarul de octeti adaugati in stiva procesului */
            if( 4 + inf.sum_bytes_stiva <= inf.total_memory ){
                inf.sum_bytes_stiva += 4;
                Push(&inf.memory,&bytes);
            }else{
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

/* functia returneaza suma timpului de executie al proceselor din 
        starea running */ 
int Sum_Running( TLG L )
{
    int sum = 0;

    for(; L != NULL; L = L->urm ){
       sum += ((TProcess_info*)L->info)->time_of_execution;
    }

    return sum;
}

/* functia returneaza suma timpului de executie al proceselor din 
        coada de asteptare */
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

int main(int argc, char *argv[])
{
    char *buf, *queue;          
    int *PID_apar;
    int TIME, memory_size, time_of_execution, priority, PID, time, bytes;
    int rest = 0; 

    TProcess_info inf;
    int process_count = 0;

    void *c_waiting, *c_finished;

    if (argc != 3) {  /* numar invalid de argumente */
        fprintf(stderr,
            "Usage: %s operations_string input_file output_file\n",
            argv[0]);
        return -1;
    }

    /* se deschide fisierul de input pentru citire */ 
    FILE *input_file = fopen(argv[1], "r");
    /* se deschide fisierul de output pentru scriere */ 
    FILE *output_file = fopen(argv[2], "aw");

    if( input_file == NULL ){
        fprintf(stderr, "File \"%s\" not found\n", argv[1]);
        return -2;
    }

    /* se citeste cuanta de timp */
    fscanf(input_file,"%d\n",&TIME);

    memory_allocation_buffer(200,&buf);
    memory_allocation_buffer(20,&queue);
    memory_allocation_array(200,&PID_apar);

    /* vector de aparitii pentru PID-uri */
    memset(PID_apar,0,100);

    /* initializarea cozilor */
    c_waiting = InitQ(0);
    c_finished = InitQ(0);

    TLG Lrunning = NULL;
    TLG Lmemory = NULL;

    /* se citesc datele din fisierul de iesire */
    while(fscanf(input_file,"%s",buf) > 0){

        if( strcmp(buf,"add") == 0 ){

            fscanf(input_file,"%d %d %d",&memory_size,
                                            &time_of_execution,&priority);

            int pid = 0,start = 0;
            pid = Search_Best_PID(PID_apar);

            PID_apar[pid] = 1; /* pid ocupat */

            TMemory memory;
            memory = Init_Memory(memory_size,pid);

            /* se verifica alocarea memoriei unui proces */
            int memo = InsG(&Lmemory,&memory,sizeof(TMemory),&start);

            if( memo == 1 ){
            
                fprintf(output_file,
                "Process created successfully: PID: %d,",pid); 
                fprintf(output_file," Memory starts at 0x%x.\n",start);
                
                process_count++;
                /* in caz afirmativ se adauga procesul in starea de running 
                daca este primul proces din sistem sau in coada de asteptare */

                inf = Init_Process(pid,priority,time_of_execution,memory_size);

                if( process_count == 1 ){ 
                    InsSfG(&Lrunning,&inf,sizeof(TProcess_info)); 
                }
                else SortareWaiting(c_waiting,&inf);

                }else{
                    /* in caz negativ, se realizeaza defragmentarea si se
                    reia procesul */

                    Sortare(Lmemory);
                    Defragmentare(&Lmemory);

                    memo = InsG(&Lmemory,&memory,sizeof(TMemory),&start);

                    if( memo == 1 ){

                        fprintf(output_file,
                        "Process created successfully: PID: %d,",pid); 
                        fprintf(output_file," Memory starts at 0x%x.\n",start);
                
                        process_count++;

                        inf = Init_Process(pid,priority,
                                            time_of_execution,memory_size);

                        if( process_count == 1 ){ 
                            InsSfG(&Lrunning,&inf,sizeof(TProcess_info));
                        }
                        else SortareWaiting(c_waiting,&inf);

                    }else{
                        /* in cazul in care procesul nu are loc 
                                            este eliberat PID-ul*/
                        PID_apar[pid] = 0;
                        fprintf(output_file,
                        "Cannot reserve memory for PID %d.\n",pid);

                    }
                }
        }

        if( strcmp(buf,"get") == 0 ){

            fscanf(input_file,"%d",&PID);

            int long long remaining_time;
            int run = SearchRunning(Lrunning,PID,&remaining_time);
            int wait = SearchWaiting(c_waiting,PID,&remaining_time);

            if( run == 1 ) fprintf(output_file,
                        "Process %d is running (remaining_time: %lld).\n"
                                                    ,PID,remaining_time);
            if( wait == 1 ) fprintf(output_file,
                        "Process %d is waiting (remaining_time: %lld).\n"
                                                    ,PID,remaining_time);
            
            int finish = SearchFinished(c_finished,PID,output_file);

            if( run == 0 && wait == 0 && finish == 0 ){
                fprintf(output_file,"Process %d not found.\n",PID);
            }
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
                if( wait == 1 ) Wait_Stack(c_waiting,PID,output_file);

                if( run == 0 && wait == 0 ){
                    fprintf(output_file,"PID %d not found.\n",PID);
                }
            }
        }

        if( strcmp(buf,"run") == 0 ){

            fscanf(input_file,"%d",&time);

            int ok = 0;
            int p = ((TProcess_info*)Lrunning->info)->PID;

            if( process_count == 1 ){
        
                Run_Time(Lrunning,time,c_finished,&ok);
                /* daca procesul este singurul din sistem si acesta si-a 
                terminat executia */    
                if( ok == 1 ){
                    /* memoria va fi eliberata */
                    Memory_update(&Lmemory,p);
                    /* PID-ul va fi eliberat */
                    PID_apar[p] = 0; 
                    /* numarul de procese scade */
                    process_count--;
                    /* listele de memorie si running devin nule */
                    Lrunning = NULL;
                    Lmemory = NULL;
                }
    
            }else{

                while(time){
                    int exec_time = 0;
                    
                    Run_Time_2(&Lrunning,time,TIME,c_finished,c_waiting,PID_apar,
                                        Lmemory,&rest,&exec_time,&process_count);
                    time = time - exec_time;

                    if( process_count == 1 ){
                        /* daca procesul este singurul din sistem si acesta si-a 
                        terminat executia */ 
                        Run_Time(Lrunning,time,c_finished,&ok);

                        if( ok == 1 ){
                            Memory_update(&Lmemory,p);
                            PID_apar[p] = 0; 
                            process_count--;
                            Lrunning = NULL;
                            Lmemory = NULL;
                        }
                        break;
                    }
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

            if( run == 0 && wait == 0 ){ 
                fprintf(output_file,"PID %d not found.\n",PID);
            }

        }

        if( strcmp(buf,"pop") == 0 ){
            fscanf(input_file,"%d",&PID);

            int long long remaining_time;
            int run = SearchRunning(Lrunning,PID,&remaining_time);
            int wait = SearchWaiting(c_waiting,PID,&remaining_time);

            if( run == 1 ) Run_and_Pop(Lrunning,PID,output_file);
            if( wait == 1 ) Wait_and_Pop(c_waiting,PID,output_file);

            if( run == 0 && wait == 0 ){ 
                fprintf(output_file,"PID %d not found.\n",PID);
            }

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

