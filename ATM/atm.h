/*SIMIONESCU MARIA-ADRIANA - 312CB*/

#ifndef _ATM_h_
#define _ATM_h_

#define MAX 100

typedef struct cell{
    char history[100];
    struct cell *next_history;
}TCell, *TLista, **ALista;

typedef struct{
    char card_number[18];
    char pin[6];
    char original_pin[6];
    char expiry_date[6];
    int invalid_pin;
    char cvv[4];
    int balance;
    char status[7];
    TLista HISTORY;
}TCard;

typedef struct lista_carduri1{
    TCard card;
    struct lista_carduri1 *next_card; 
}TCell1, *TLSC, **ALSC;

typedef struct lista_carduri{
    int poz;
    TLSC current_cards;
    struct lista_carduri *next_lsc;
}TCell2, *TLC, **ALC; 

int verify_card_existence(TLC L, char *card_number, int max_poz);
void print_card(TLC L, int max_poz, char *card_number, FILE *output_file);
void print_all(TLC L, int max_poz,FILE *output_file);
int get_card_poz(char text[50], int start, int stop, int nr_max_cards);
TLista AllocCell(char *e);
TLSC AllocCell1(TCard *e);
TLC AllocCell2(TLSC *e, int poz);
void InsBefore(TLSC *LSC, TCard card);
TLSC GenerateList(TCard card);
void DestroyL(ALista aL);
void DestroyLSC(ALSC aL);
void Destroy_Lists(ALC aLC, int max_poz1,int max_poz);
void Destroy_Extra(ALC aLC, int max_poz1);
void DestroyLSC1(ALSC aL);
void Delete_card(TLC LC, char *card_number, int card_poz);
int Verify_pin(TLC LC,int card_poz,char *card_number, char *pin);
TLista GenerateHistory(char *history);
void InsBeforeHistory(TLista *L, char *history);
void History(TLC LC, int card_poz, char *char_aux, char *history);
void Cancel(TLC LC,int card_poz, char *card_number);
void Unblock_card(TLC LC,int card_poz, char *card_number);
void Change_pin(TLC LC, int card_poz, char *card_number, char *new_pin);
void Balance_inquiry(TLC LC, int card_poz, 
                    char *card_number,FILE *output_file);
int Verif_Pin(char *pin);
int Verif_sum(char *sum);
int Sum(char *sum);
void Recharge(TLC LC, int card_poz, char *card_number,
                            int sum, FILE *output_file);
int Cash_withdrawal(TLC LC, int card_poz, char *card_number,
                                    int sum, FILE *output_file);
int Transfer_funds(TLC LC, int card_poz1, int card_poz2,        
        char *card_number1, char *card_number2, int sum, FILE *output_file);
int Reverse_transaction(TLC LC, int card_poz1, int card_poz2, 
             char *card_number1, char *card_number2,char *char_3, int sum);

#endif