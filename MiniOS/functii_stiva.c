/* SIMIONESCU MARIA-ADRIANA - 312 CB */

#include "header.h"

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