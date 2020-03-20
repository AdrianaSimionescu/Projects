/* SIMIONESCU MARIA-ADRIANA - 312 CB */

#include "header.h"

/* functie care afiseaza coada de asteptare */ 
void Print_Waiting( void *c, FILE *f )
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

/* pune in coada de asteptare toate elementele din coada de aspetare */
void Wait_and_finish( void *c ,void *c_f )
{
    void *c1 = InitQ(0);
	TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        IntrQ(c_f,&inf,sizeof(TProcess_info));
		IntrQ(c1,&inf,sizeof(TProcess_info));
	}

	while( ((TCoada*)c1)->sc != NULL){
        ExtrgQ(c1,&inf);
		IntrQ(c,&inf,sizeof(TProcess_info));
	}

	DistrQ(c1);

}