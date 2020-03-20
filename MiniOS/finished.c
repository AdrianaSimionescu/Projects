/* SIMIONESCU MARIA-ADRIANA - 312 CB */

#include "header.h"

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
