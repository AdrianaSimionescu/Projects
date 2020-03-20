/* SIMIONESCU MARIA-ADRIANA - 312 CB */

#include "header.h"

/* functie care cauta in lista running un proces in functie de
    PID-ul sau si intoarce 1-exista, 0-nu exista si prin efect
    lateral timpul de executie */
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
                                                            care este posibil*/
        if( ok == 0 ) *exec_time = TIME;
    }
 
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

/* functia plaseaza in coada de asteptare elementele din lista running */ 
void Run_and_Finish( TLG L , void *c_f )
{
    for(; L != NULL; L = L->urm ){
        IntrQ(c_f,((TProcess_info*)L->info),sizeof(TProcess_info));
    }
}