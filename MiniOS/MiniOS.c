/* SIMIONESCU MARIA-ADRIANA - 312 CB */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "header.h"

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

    memory_allocation_buffer(10000,&buf);
    memory_allocation_buffer(200,&queue);
    memory_allocation_array(1000,&PID_apar);

    /* vector de aparitii pentru PID-uri */
    memset(PID_apar,0,100);

    /* initializarea cozilor */
    c_waiting = InitQ(0);
    c_finished = InitQ(0);

    TLG Lrunning = NULL;
    TLG Lmemory = NULL;

    /* se citesc datele din fisierul de iesire */
    while( fscanf(input_file,"%s",buf) > 0 ){

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

            Print_Waiting(c_waiting,output_file);
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

            /* Run_and_Finish(Lrunning,c_finished);
               Wait_and_finish(c_waiting,c_finished); */

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

    return 0;
}

