/*SIMIONESCU MARIA-ADRIANA - 312CB*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "atm.h"

#define MAX 100

/* verify if the card already exists, return 1 if it exists and 0 if not */
int verify_card_existence(TLC L, char *card_number, int max_poz)
{
    TLSC L1;
    int i;
    /*search for the card*/
    for(i = 0; i <= max_poz; i++){
        L1 = L->current_cards;
        for(; L1 != NULL; L1 = L1->next_card){
                if( strcmp(L1->card.card_number,card_number) == 0 ){
                    return 1;
                }
        }
        L = L->next_lsc;
    }
    return 0;
}

void print_card(TLC L, int max_poz, char *card_number, FILE *output_file)
{
    TLSC L1;
    int i, ok = 0;
    TLista L2 = NULL;

    /*Write result in output file*/
    /*go through each list and print the corresponding values*/
    for(i = 0; i <= max_poz; i++){

        L1 = L->current_cards;

        for(; L1 != NULL; L1 = L1->next_card){

                if( strcmp(L1->card.card_number,card_number) == 0 ){
                    fprintf(output_file,"(");
                    fprintf(output_file,"card number: %s, PIN: %s, ",
                        L1->card.card_number,L1->card.pin);
                    fprintf(output_file,"expiry date: %s, CVV: %s, ",
                        L1->card.expiry_date,L1->card.cvv);
                    fprintf(output_file,"balance: %d, status: %s, ",
                        L1->card.balance,L1->card.status);

                    fprintf(output_file,"history: [");
                    L2 = L1->card.HISTORY;

                    for(; L2 != NULL; L2 = L2->next_history){
                        if(L2->next_history != NULL){
                            fprintf(output_file,"(%s), ",L2->history);
                        }
                        else fprintf(output_file,"(%s)",L2->history);
                    }
                    fprintf(output_file,"]");
                    fprintf(output_file,")\n");
                    ok = 1;
                    break;
                }
        }
        if(ok == 1) break;
        L = L->next_lsc;
    }
}

void print_all(TLC L, int max_poz,FILE *output_file)
{
    int i, poz = 0;

    TLSC LSC = NULL;
    TLista L2 = NULL;

    /*Write result in output file*/
    /*go through each list and print the corresponding values*/
    for(i = 0; i <= max_poz; i++){

        LSC = L->current_cards;

        fprintf(output_file,"pos%d: [",poz);

        if(LSC != NULL) fprintf(output_file,"\n");

        for(; LSC != NULL; LSC = LSC->next_card){
            fprintf(output_file,"(");

            fprintf(output_file,"card number: %s, PIN: %s, ",
                        LSC->card.card_number,LSC->card.pin);
            fprintf(output_file,"expiry date: %s, CVV: %s, ",
                        LSC->card.expiry_date,LSC->card.cvv);
            fprintf(output_file,"balance: %d, status: %s, ",
                        LSC->card.balance,LSC->card.status);

            fprintf(output_file,"history: [");
            L2 = LSC->card.HISTORY;

            for(; L2 != NULL; L2 = L2->next_history){
                if(L2->next_history != NULL){
                    fprintf(output_file,"(%s), ",L2->history);
                }
                else fprintf(output_file,"(%s)",L2->history);
            }
            fprintf(output_file,"]");
            fprintf(output_file,")\n");
        }   
        fprintf(output_file,"]\n");
        L = L->next_lsc;
        poz++;
    }
}

/*returns the card position by getting the sum digits from the card number*/
int get_card_poz(char text[50], int start, int stop, int nr_max_cards)
{
    int i,sum = 0;
    for(i = start+1; i < stop; i++){
        sum += text[i] - '0';
    }
    return sum % nr_max_cards;
}

TLista AllocCell(char *e)          
{ 
    TLista aux = (TLista)malloc(sizeof(TCell));  
    if(aux)                          
    {
        memcpy(aux->history,e,sizeof(aux->history));             
        aux->next_history = NULL;
    }
    return aux;             
}

TLSC AllocCell1(TCard *e)   
{ 
    TLSC aux = (TLSC)malloc(sizeof(TCell1));
    if(aux){ 
        memcpy(&aux->card, e, sizeof(TCard));
        aux->next_card = NULL;
    }
    return aux;
}

TLC AllocCell2(TLSC *e, int poz)   
{ 
    TLC aux = (TLC)malloc(sizeof(TCell2));
    if(aux){
        aux->poz = poz;
        memcpy(&aux->current_cards, e, sizeof(TLSC));
        aux->next_lsc = NULL;
    }
    return aux;
}

/*insert before into a LSC list*/ 
void InsBefore(TLSC *LSC, TCard card)
{
    TLSC aux;
    aux = AllocCell1(&card);
    if(!aux) return;
    aux->next_card = *LSC;
    *LSC = aux;
}

/*returns a list of type LSC*/
TLSC GenerateList(TCard card)
{
    TLSC LSC = NULL, u = NULL, aux;
    aux = AllocCell1(&card);
    if(!aux) return LSC;
    if(LSC == NULL) LSC = aux;
    else u->next_card = aux;
    u = aux; 
    return LSC;
}

/*destroy a history list*/
void DestroyL(ALista aL)
{
    TLista aux;
    while(*aL != NULL)
    { 
        aux = *aL;  
        *aL = aux->next_history;   
        free(aux);  
    }
}

/*destroy a LSC list*/
void DestroyLSC(ALSC aL)
{
    TLSC aux;
    while(*aL != NULL)
    {   
        aux = *aL; 
        *aL = aux->next_card;   
        free(aux);  
    }
}

/*destroys lists of type LSC*/
void Destroy_Lists(ALC aLC, int max_poz1,int max_poz)
{
    int i;

    /*locate the LSC list*/
    while(*aLC != NULL)
    {
        if((*aLC)->poz == max_poz1 + 1) break;
         aLC = &(*aLC)->next_lsc;
    }

    for(i = max_poz1 + 1; i <= max_poz; i++){
        TLSC LSC = (*aLC)->current_cards;
        /*destroy every history list contained in the card cell*/ 
        for(; LSC != NULL; LSC = LSC->next_card){
            TLista L = LSC->card.HISTORY;
            DestroyL(&L);
        }
        /*destroy a LSC list coresponding to a 
        certain position in the LC list*/
        DestroyLSC(&(*aLC)->current_cards);
        aLC = &(*aLC)->next_lsc;
    }
}

/*destroys cells of type LC starting from a certain position*/
void Destroy_Extra(ALC aLC, int max_poz1)
{
    TLC aux;
     while(*aLC != NULL)
    {
        if((*aLC)->poz == max_poz1 + 1) break;
        aLC = &(*aLC)->next_lsc;
    }

    while(*aLC != NULL)
    {
        aux = *aLC;  
        *aLC = aux->next_lsc;   
        free(aux);  
    }
}

/*delete a cell of type LSC used to delete a card*/
void DestroyLSC1(ALSC aL)
{
    TLSC aux;  
    aux = *aL; 
    *aL = aux->next_card;   
    free(aux);  
}

/*delete a card*/
void Delete_card(TLC LC, char *card_number, int card_poz)
{
    /*locate the position in LC*/
    for(; LC != NULL; LC = LC->next_lsc){
        if(LC->poz == card_poz) break;
    }

    /*get the adress of the list where the card is located*/
    ALSC L1 = &LC->current_cards;
    TLSC ant = NULL, p = NULL;

    /*locate the card*/
    for(p = *L1, ant = NULL; p != NULL; ant = p, p = p->next_card){
        if(strcmp(p->card.card_number,card_number) == 0) break;
    }

    /*the links are restored*/
    if(p == NULL) return;
    if(ant == NULL) *L1 = p->next_card;
    else ant->next_card = p->next_card;

    /*the history list of the card is destroyed*/
    DestroyL(&p->card.HISTORY);
    /*then the cell of the current card is destroyed*/
    DestroyLSC1(&p);
}

/*returns 0 if the pin should be changed, -1 if the pin is invalid, 
-3 if the card is already blocked, -2 if the pin is wrong and the card
is getting blocked, and 1 id it is correct*/
int Verify_pin(TLC LC,int card_poz,char *card_number, char *pin)
{
    /*locate the card*/
    while(LC){
        if(LC->poz == card_poz) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number) == 0) break;
        L2 = L2->next_card;
    }

    if(strcmp(L2->card.status,"LOCKED") == 0) return -3;

    if(strcmp(L2->card.pin,pin) != 0){
        /*keep the evidence of the number of wrong pins*/
        L2->card.invalid_pin++;
        /*if the number of wrong pins is 3 the card will be blocked*/
        if(L2->card.invalid_pin == 3) {
            strcpy(L2->card.status,"LOCKED");
            return -2;
        }
        return -1;
    }
    if(strcmp(L2->card.original_pin,pin) == 0 && strcmp(L2->card.status,"ACTIVE") != 0) return 0;
    return 1;
}

/*function that generates a history list*/
TLista GenerateHistory(char *history)
{
    TLista L = NULL, u = NULL, aux;
    aux = AllocCell(history);
    if(!aux) return L;
    if(L == NULL) L = aux;
    else u->next_history = aux;
    u = aux; 
    return L;
}

/*function that inserts before into a history list*/
void InsBeforeHistory(TLista *L, char *history)
{
    TLista aux;
    aux = AllocCell(history);
    if(!aux) return;
    aux->next_history = *L;
    *L = aux;
}

/*creates the history list for a card*/
void History(TLC LC, int card_poz, char *char_aux, char *history)
{
    /*locate the card*/
    while(LC){
        if(LC->poz == card_poz) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,char_aux) == 0) break;
        L2 = L2->next_card;
    }

    TLista H1 = NULL;

    /*if the list of history is empty a new list will be generated*/
    if(L2->card.HISTORY == NULL){
        H1 = GenerateHistory(history);
    }
    else{
        /*if the list is not empty, the current history list will be 
        copied into a new one*/
        memcpy(&H1,&L2->card.HISTORY,sizeof(TLista));
        /*insert before into the new list the new information*/
        InsBeforeHistory(&H1,history);
    }

    /*the generated list will be copied into the history cell
    corresponding to the card*/
    memcpy(&L2->card.HISTORY,&H1,sizeof(TLista));
}

/*resets the number of invalid pins tried to 0*/
void Cancel(TLC LC,int card_poz, char *card_number)
{
    /*locate the card*/
    while(LC){
        if(LC->poz == card_poz) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number) == 0) break;
        L2 = L2->next_card;
    }

    /*reset the number of invalid pins tried to 0*/
    L2->card.invalid_pin = 0;
}

/*resets the number of invalid pins tried and update the status of the card*/
void Unblock_card(TLC LC,int card_poz, char *card_number)
{
    /*locate the card*/
     while(LC){
        if(LC->poz == card_poz) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number) == 0) break;
        L2 = L2->next_card;
    }
    
    /*reset the number of invalid pins tried to 0*/
    L2->card.invalid_pin = 0;
    /*update the status of the card*/
    strcpy(L2->card.status,"ACTIVE");
}

/*changes the pin and update the status of the card*/
void Change_pin(TLC LC, int card_poz, char *card_number, char *new_pin)
{
    /*locate the card*/
    while(LC){
        if(LC->poz == card_poz) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number) == 0) break;
        L2 = L2->next_card;
    }

    /*update the pin with the new one*/
    strcpy(L2->card.pin,new_pin);
    /*update the status*/
    strcpy(L2->card.status,"ACTIVE");
}

void Balance_inquiry(TLC LC, int card_poz, 
                    char *card_number,FILE *output_file)
{
    /*locate the card*/
    while(LC){
        if(LC->poz == card_poz) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number) == 0) break;
        L2 = L2->next_card;
    }

    /*print the balance of the card*/
    fprintf(output_file,"%d\n",L2->card.balance);
}

/*returns 0 if there are other characters besides numbers
and 1 if the pin is correct*/
int Verif_Pin(char *pin)
{
    int i, num = 0, x;
    x = strlen(pin);
    for(i = 0; i < x; i++){
        num = pin[i] - '0';
        if( num < 1 || num > 9) return 0;     
    }
    return 1;
}

/*returns 0 if the sum corresponding with the char given is not a multiple 
of 10 and the sum in case the requirement is respeted*/
int Verif_sum(char *sum)
{
    int i, s = 0, ten = 1, num  = 0;
    for(i = strlen(sum) - 1; i >= 0; i--){
        num = sum[i] - '0';
        s = s + num*ten;
        ten *= 10;
    }
    if(s % 10 != 0) return 0;
    return s; 
}

/*returns the sum corresponding to the char given*/
int Sum(char *sum)
{
    int i, s = 0, ten = 1, num  = 0;
    for(i = strlen(sum) - 1; i >= 0; i--){
        num = sum[i] - '0';
        s = s + num*ten;
        ten *= 10;
    }
    return s;
}

/*update the balance according to the command*/
void Recharge(TLC LC, int card_poz, char *card_number,
                            int sum, FILE *output_file)
{
    /*locate the card*/
    while(LC){
        if(LC->poz == card_poz) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number) == 0) break;
        L2 = L2->next_card;
    }
    
    /*update the balance*/
    L2->card.balance += sum;
    /*print the new balance*/
    fprintf(output_file,"%d\n",L2->card.balance);
}

/*returns 1 if the balance allows the withdrawal and o if not*/
int Cash_withdrawal(TLC LC, int card_poz, char *card_number,
                                    int sum, FILE *output_file)
{
    /*locate the card*/
    while(LC){
        if(LC->poz == card_poz) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number) == 0) break;
        L2 = L2->next_card;
    }

    if(L2->card.balance < sum) return 0;
    else{
        /*set the new balance*/
        L2->card.balance -= sum;
        /*print the new balance*/
        fprintf(output_file,"%d\n",L2->card.balance);
    }
    return 1;
}

/*returns 1 if the transfer of funds can be made and 0 if not*/
int Transfer_funds(TLC LC, int card_poz1, int card_poz2,        
        char *card_number1, char *card_number2, int sum, FILE *output_file)
{
    TLC LC_copy = LC;

    /*locate the card source*/
    while(LC){
        if(LC->poz == card_poz1) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number1) == 0) break;
        L2 = L2->next_card;
    }
          
    if(L2->card.balance < sum) return 0;
    else{
        /*if there are enough funds,the money will be transferred 
        from the source card to the destination card*/

        L2->card.balance -= sum;

        /*print the balance of the source card*/
        fprintf(output_file,"%d\n",L2->card.balance);

        /*locate the card destination*/
        while(LC_copy){
            if(LC_copy->poz == card_poz2) break;
            LC_copy = LC_copy->next_lsc;
        }

        TLSC L2_copy = LC_copy->current_cards;

        while(L2_copy){
            if(strcmp(L2_copy->card.card_number,card_number2) == 0) break;
            L2_copy = L2_copy->next_card;
        }

        L2_copy->card.balance += sum;
    }
    return 1;
}

/*returns 1 if the reverse transaction can be made and 0 if not*/
int Reverse_transaction(TLC LC, int card_poz1, int card_poz2, 
             char *card_number1, char *card_number2,char *char_3, int sum)
{
    TLC LC_copy = LC;

    /*locate the card source*/
    while(LC){
        if(LC->poz == card_poz1) break;
        LC = LC->next_lsc;
    }

    TLSC L2 = LC->current_cards;

    while(L2){
        if(strcmp(L2->card.card_number,card_number1) == 0) break;
        L2 = L2->next_card;
    }

    /*locate the card destination*/
    while(LC_copy){
        if(LC_copy->poz == card_poz2) break;
        LC_copy = LC_copy->next_lsc;
    }

    TLSC L2_copy = LC_copy->current_cards;

    while(L2_copy){
        if(strcmp(L2_copy->card.card_number,card_number2) == 0) break;
        L2_copy = L2_copy->next_card;
    }
    /*check if the destination card has a balance that 
    allows the transaction*/
    if(L2_copy->card.balance < sum) return 0;
    else{

        /*if it is possible, the money from the destination card will be
        transferred to the card source*/
        L2_copy->card.balance -= sum;
        L2->card.balance += sum;

        /*delete the cell from the history of the destination card 
        that contains the transaction*/ 
        ALista L = &L2_copy->card.HISTORY;
        TLista ant = NULL, p = NULL;
        for(p = *L, ant = NULL; p != NULL; ant = p, p = p->next_history){
            if(strcmp(p->history,char_3) == 0) break;
        }
        if(p == NULL) return -1;
        if(ant == NULL) *L = p->next_history;
        else ant->next_history = p->next_history;
        free(p);

    }
    return 1;
}
