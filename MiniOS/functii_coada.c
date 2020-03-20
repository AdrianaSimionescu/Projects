/* SIMIONESCU MARIA-ADRIANA - 312 CB */

#include "header.h"

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

/* distruge coada */
void DistrQ( void *c )
{
    TProcess_info inf;

	while( ((TCoada*)c)->sc != NULL ){
		ExtrgQ(c,&inf);
        DistrS(&(inf.memory));/* distruge stiva corespunzatoare unui proces */
	}
}