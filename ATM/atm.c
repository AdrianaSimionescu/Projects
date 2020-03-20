/*SIMIONESCU MARIA-ADRIANA - 312CB*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "atm.h"

#define MAX 100

int main()
{
    char buf[MAX],              //buffer used for reading from the file
        text[MAX][MAX];         //input, line by line
    int line_count = 0, i, nr_max_cards = 0, j,sec_sp1 = 0,sec_sp2 = 0,
        sec_sp3 = 0,sec_sp4 = 0, max_poz = 0;
    char command[50], char_aux[20], char_aux2[20], char_aux3[30];
    int card_poz, k, card_existence, add_card = 0, card_poz1 = 0;

    //Open input file for reading 
    FILE *input_file = fopen("input.in","r");

    if(input_file == NULL){
        fprintf(stderr, "File \"%s\" not found\n", "file");
        return -1;
    }

    //Read data in file line by line
    while(fgets(buf, MAX, input_file)){
        strcpy(text[line_count], buf);
        line_count++;
    }

    //close the input file
    fclose(input_file);

    // Open output file for writing

    FILE *output_file = fopen("output.out", "w");

    if(output_file == NULL) {
    fprintf(stderr, "Could not open or create file \"%s\"\n","output.out");
    return -1;
    }

    /*get the first character of the file that
     represents the maximum number of cards*/
    nr_max_cards = *text[0] - '0';

    TLSC LSC2 = NULL;
    TCard card_details;
    TLC aux, u, LC = NULL;

    /*alloc cells for the LC list*/
    for(i = 0; i <= nr_max_cards; i++){
        aux = AllocCell2(&LSC2,i);
        if(!aux) return -1;    
        if(LC == NULL) LC = aux;
        else u->next_lsc = aux;
        u = aux;
    }

    memset(char_aux,0,sizeof char_aux);
    memset(command,0,sizeof command);

    for(i = 1; i < line_count; i++){
        j = 0; 

        while(text[i][j] != ' ' && text[i][j] != '\n'){
            j++;  //find the first space in the line
        }

        strncpy(command,text[i],j); //get the command

        memset(&card_details, 0, sizeof(card_details));
        memset(char_aux,0,sizeof char_aux);
        memset(char_aux2,0,sizeof char_aux2);
        memset(char_aux3,0,sizeof char_aux3);

         /*keep the lenght of the current line*/
        int lenght = strlen(text[i])-1;

        if(strcmp(command,"add_card") == 0){
            add_card = 1;
            sec_sp1 = sec_sp2 = sec_sp3 = sec_sp4 = 0;

            sec_sp1 = j + 1;
            /* get the card number */
            while(text[i][sec_sp1] != ' '){
                sec_sp1++;
            }
            /*get the position where the card should be added in the LC list*/
            card_poz = get_card_poz(text[i],j,sec_sp1,nr_max_cards);
            /*keep the last position where we insert a card 
            deleting the extra cells allocated at the end*/
            if(card_poz > max_poz) max_poz = card_poz;

            strncpy(card_details.card_number,text[i]+j+1,sec_sp1-j-1);
            /*verify if the card exists*/
            if( i > 1 ) card_existence = verify_card_existence(LC,card_details.card_number,max_poz);
            
            if(card_existence == 1) fprintf(output_file,"The card already exists\n");
            else{
                /* get the pin of the card */
                sec_sp2 = sec_sp1 + 1;
                while(text[i][sec_sp2] != ' '){
                    sec_sp2++;
                }
                strncpy(card_details.pin,text[i]+sec_sp1+1,sec_sp2-sec_sp1-1);
                /*keep the original pin so that we can 
                see if it needs to be changed*/
                strncpy(card_details.original_pin,text[i]+sec_sp1+1,sec_sp2-sec_sp1-1);
                /* get the expiry date */
                sec_sp3 = sec_sp2 + 1;
                while(text[i][sec_sp3] != ' '){
                    sec_sp3++;
                }
                strncpy(card_details.expiry_date,text[i]+sec_sp2+1,sec_sp3-sec_sp2-1);

                /* get the CVV */
                sec_sp4 = sec_sp3 + 1;
                while(text[i][sec_sp4] != '\n'){
                    sec_sp4++;
                }
                strncpy(card_details.cvv,text[i]+sec_sp3+1,sec_sp4-sec_sp3-1);

                card_details.balance = 0;
                /*in this I keep the number of invalid pins added*/
                card_details.invalid_pin = 0;
                /*at the beginning the status is "NEW"*/
                strcpy(card_details.status,"NEW");
                /*go through the LC list*/
                for(k = 0; k <= card_poz; k++){
                    //search for the position where the card should be added
                    if(k == card_poz){ 
                        TLSC LSC = NULL;
                        TLista L2 = NULL,aux;
                        ALC L1 = &LC;

                        memset(char_aux2,0,sizeof(char_aux2));
                        /*allocate memory for the first cell of history 
                        included in the CARD cell*/
                        aux = AllocCell(char_aux2);
                        if(L2 == NULL) L2 = aux;
                        /*get the adress of the LSC list
                         corresponding to the position in LC*/ 
                        while(*L1){
                            if((*L1)->poz == card_poz) break;
                            L1 = &(*L1)->next_lsc;
                        }
                        /*if the LSC list is empty I generate a new list
                        with the corresponding card details*/
                        if((*L1)->current_cards == NULL){
                        LSC = GenerateList(card_details);
                        }
                        else {
                            /*if the LSC list is not empty I copy the 
                            current list into a NULL list and insert
                            before another cell with the actual card details*/
                            memcpy(&LSC,&(*L1)->current_cards,sizeof(TLSC));
                            InsBefore(&LSC,card_details);
                        }
                        /*copy the information that includes the LSC list
                        generated into the correspondig LC cell*/
                        memcpy(&(*L1)->current_cards,&LSC,sizeof(TLSC));    
                    }
                }
            }
        }

        if(strcmp("show",command) == 0){
            /*check if there are more characters besides "show"
            in case there are more they represent the card number
            of the card that we have to show*/ 
            if(text[i][5] >= 48 && text[i][5] <= 57){
                strncpy(char_aux,text[i] + 5,16);
                print_card(LC,max_poz,char_aux,output_file);
            }
            else{
                /*check if there are cards to show*/
                if(add_card != 0){
                print_all(LC,max_poz,output_file);
                }
            }
        }

        if(strcmp("delete_card",command) == 0){
            strncpy(char_aux,text[i] + 12,16);
            /*get the card position*/
            card_poz = get_card_poz(text[i],11,28,nr_max_cards);
            Delete_card(LC,char_aux,card_poz);
        }
   
        if(strcmp("insert_card",command) == 0){
            /*get the card number*/
            strncpy(char_aux,text[i] + 12,16);
            /*get the card pin*/
            strncpy(char_aux2,text[i] + 29,lenght-29);
             /*get the card position*/
            card_poz = get_card_poz(text[i],11,28,nr_max_cards);

            int verif_pin = Verify_pin(LC,card_poz,char_aux,char_aux2);

            if(verif_pin == 0) {
                fprintf(output_file,"You must change your PIN.\n");
            }
            if(verif_pin == -1) { 
                fprintf(output_file,"Invalid PIN\n");
            }
            if(verif_pin == -2) {
                fprintf(output_file,"Invalid PIN\n");
                fprintf(output_file,
                "The card is blocked. Please contact the administrator.\n"); 
            }
            if(verif_pin == -3) fprintf(output_file,
            "The card is blocked. Please contact the administrator.\n");
            
            memset(char_aux2,0,sizeof char_aux2);
            /*create the char that needs to be placed as information
            into the history list of the card*/
            if(verif_pin == 0 || verif_pin == 1){
                strcpy(char_aux2,"SUCCESS, ");
                strcat(char_aux2,text[i]);
                char_aux2[9+lenght] = 0;
            }
             
            if(verif_pin == -1 || verif_pin == -2 || verif_pin == -3){
                strcpy(char_aux2,"FAIL, ");
                strcat(char_aux2,text[i]);
                char_aux2[lenght+6] = 0;
            }
            History(LC,card_poz,char_aux,char_aux2);
        }
    
        if(strcmp("cancel",command) == 0){
            /*create the char that needs to be placed as information
            into the history list of the card*/
            strncpy(char_aux,text[i] + 7,16);
            strcpy(char_aux2,"SUCCESS, ");
            strcat(char_aux2,text[i]);
            char_aux2[9+lenght] = 0;
            /*get the card position*/
            card_poz = get_card_poz(text[i], 6, 23, nr_max_cards);
            Cancel(LC,card_poz,char_aux);
            History(LC,card_poz,char_aux,char_aux2);
        }

        if(strcmp("unblock_card",command) == 0){
            strncpy(char_aux,text[i] + 13,16);
            /*get the card position*/
            card_poz = get_card_poz(text[i], 12, 29, nr_max_cards);
            Unblock_card(LC,card_poz,char_aux);
        }

        if(strcmp("pin_change",command) == 0){
            /*copy the characters after the card number
            to check if they are more than it should*/
            strncpy(char_aux2,text[i] + 28,lenght-28);
            /*keep the lenght of the characters after the card number*/
            int lenght1 = strlen(char_aux2);
            /*get the card position*/
            card_poz = get_card_poz(text[i], 10, 27, nr_max_cards);
            /*get the card number*/
            strncpy(char_aux,text[i] + 11,16);
            int verif_pin = Verif_Pin(char_aux2);
            /*if the pin has 4 numbers and it is formed correctly
            it will be changed*/
            if(lenght1 == 4 && verif_pin != 0){
                Change_pin(LC,card_poz,char_aux,char_aux2);
                /*create the char that needs to be placed as information
                into the history list of the card*/ 
                strcpy(char_aux2,"SUCCESS, ");
                strcat(char_aux2,text[i]);
                char_aux2[9+lenght] = 0;

                History(LC,card_poz,char_aux,char_aux2);
            }
            else{
                fprintf(output_file,"Invalid PIN\n");
                /*create the char that needs to be placed as information
                into the history list of the card*/ 
                strcpy(char_aux2,"FAIL, ");
                strcat(char_aux2,text[i]);
                char_aux2[6+lenght] = 0;

                History(LC,card_poz,char_aux,char_aux2);
            }
        }

        if(strcmp("balance_inquiry",command) == 0){
            /*get the card number*/
            strncpy(char_aux,text[i] + 16,16);
            /*get the card position*/
            card_poz = get_card_poz(text[i], 15, 32, nr_max_cards);
            Balance_inquiry(LC,card_poz,char_aux,output_file);
            /*create the char that needs to be placed as information
            into the history list of the card*/ 
            strcpy(char_aux2,"SUCCESS, ");
            strcat(char_aux2,text[i]);
            char_aux2[9+lenght] = 0;

            History(LC,card_poz,char_aux,char_aux2);
        }

        if(strcmp("recharge",command) == 0){
            /*get the card number*/
            strncpy(char_aux,text[i] + 9,16);
            /*get the card position*/
            card_poz = get_card_poz(text[i], 8, 25, nr_max_cards);
            /*get the sum requested*/
            strncpy(char_aux2,text[i] + 26,lenght-26);
            /*check if the sum is a multiple of 10*/
            int verif_sum = Verif_sum(char_aux2);
            if(verif_sum != 0){
                Recharge(LC,card_poz,char_aux,verif_sum,output_file);
                /*create the char that needs to be placed as information
                into the history list of the card*/ 
                strcpy(char_aux2,"SUCCESS, ");
                strcat(char_aux2,text[i]);
                char_aux2[9+lenght] = 0;

                History(LC,card_poz,char_aux,char_aux2);

            }
            else{
                fprintf(output_file,
                            "The added amount must be multiple of 10\n");
                /*create the char that needs to be placed as information
                into the history list of the card*/
                strcpy(char_aux2,"FAIL, ");
                strcat(char_aux2,text[i]);
                char_aux2[6+lenght] = 0;
                
                History(LC,card_poz,char_aux,char_aux2);
            }
        }

        if(strcmp("cash_withdrawal",command) == 0){
            /*get the card position*/
            strncpy(char_aux,text[i] + 16,16);
            /*get the card position*/
            card_poz = get_card_poz(text[i], 15, 32, nr_max_cards);
            /*get the sum requested*/
            strncpy(char_aux2,text[i] + 33,lenght-33);
            /*check if the sum is a multiple of 10*/
            int verif_sum = Verif_sum(char_aux2);
            if(verif_sum != 0){
                int withdrawal = Cash_withdrawal(LC,card_poz,char_aux,
                                                verif_sum,output_file);
                if(withdrawal == 0){
                    fprintf(output_file,"Insufficient funds\n");
                    /*create the char that needs to be placed as information
                    into the history list of the card*/ 
                    strcpy(char_aux2,"FAIL, ");
                    strcat(char_aux2,text[i]);
                    char_aux2[6+lenght] = 0;

                    History(LC,card_poz,char_aux,char_aux2);
                }
                else{
                    /*create the char that needs to be placed as information
                    into the history list of the card*/
                    strcpy(char_aux2,"SUCCESS, ");
                    strcat(char_aux2,text[i]);
                    char_aux2[9+lenght] = 0;

                    History(LC,card_poz,char_aux,char_aux2);
                }
            }
            else{
                fprintf(output_file,
                "The requested amount must be multiple of 10\n");
                /*create the char that needs to be placed as information
                into the history list of the card*/
                strcpy(char_aux2,"FAIL, ");
                strcat(char_aux2,text[i]);
                char_aux2[6+lenght] = 0;

                History(LC,card_poz,char_aux,char_aux2);
            }
        }

        if(strcmp("transfer_funds",command) == 0){
            /*get the card number of card source*/
            strncpy(char_aux,text[i] + 15,16); 
            /*get the card position of card source*/
            card_poz = get_card_poz(text[i], 14, 31, nr_max_cards);
            /*get the card number of card destination*/
            strncpy(char_aux2,text[i] + 32,16);
            /*get the card position of card destination*/
            card_poz1 = get_card_poz(text[i], 31, 48, nr_max_cards);
            strncpy(char_aux3,text[i] + 49,lenght-49); //sum

            int verif_sum = Verif_sum(char_aux3);

            if(verif_sum != 0){
                int verif_transfer = 
                Transfer_funds(LC,card_poz,card_poz1,
                                char_aux,char_aux2,verif_sum,output_file);
                if(verif_transfer != 0){
                    /*create the char that needs to be placed as information
                    into the history list of the card*/
                    strcpy(char_aux3,"SUCCESS, ");
                    strcat(char_aux3,text[i]);
                    char_aux3[9+lenght] = 0;
                    History(LC,card_poz,char_aux,char_aux3);
                    /*create the char that needs to be placed as information
                    into the history list of the card*/
                    strcpy(char_aux3,"SUCCESS, ");
                    strcat(char_aux3,text[i]);
                    char_aux3[9+lenght] = 0;
                    History(LC,card_poz1,char_aux2,char_aux3);
                }
                else{
                    fprintf(output_file,"Insufficient funds\n");
                     /*create the char that needs to be placed as information
                    into the history list of the card*/
                    strcpy(char_aux3,"FAIL, ");
                    strcat(char_aux3,text[i]);
                    char_aux3[6+lenght] = 0;

                    History(LC,card_poz,char_aux,char_aux3);
                }

            }
            else{
                fprintf(output_file,
                "The transferred amount must be multiple of 10\n");
                /*create the char that needs to be placed as information
                into the history list of the card*/
                strcpy(char_aux3,"FAIL, ");
                strcat(char_aux3,text[i]);
                char_aux3[6+lenght] = 0;
                History(LC,card_poz,char_aux,char_aux3);
            }
        }

        if(strcmp("reverse_transaction",command) == 0){
            /*get the card number of the card source*/
            strncpy(char_aux,text[i] + 20,16); 
            /*get the card position of card source*/
            card_poz = get_card_poz(text[i], 19, 36, nr_max_cards);
            /*get the card number of card destination*/
            strncpy(char_aux2,text[i] + 37,16);
            /*get the card position of card destination*/
            card_poz1 = get_card_poz(text[i], 36, 53, nr_max_cards);
            strncpy(char_aux3,text[i] + 54,lenght-54); /*get the sum*/
            /*transform the sum  from char to int*/
            int sum = Sum(char_aux3);
            /*create the char that we will search for 
            in the history of the destination card so that 
            we can delete the cell that contains this infomation*/
            strcpy(char_aux3,"SUCCESS, transfer_funds ");
            strcat(char_aux3,text[i] + 20);
            char_aux3[lenght+4] = 0;

            int check_transaction = 
            Reverse_transaction(LC,card_poz,card_poz1,
                                char_aux,char_aux2,char_aux3,sum);

            if(check_transaction != 0){
                 /*create the char that needs to be placed as information
                into the history list of the card*/
                strcpy(char_aux3,"SUCCESS, "); 
                strcat(char_aux3,text[i]);
                char_aux3[9+lenght] = 0;

                History(LC,card_poz,char_aux,char_aux3);
            }
            else{
                fprintf(output_file,"The transaction cannot be reversed\n");
            }
        }
        memset(command,0,sizeof command);
    }

    /*Destroy the LSC lists with the HISTORY lists 
    allocated for the extra LC cells allocated*/
    Destroy_Lists(&LC,max_poz,nr_max_cards);
    /*Destroy the extra cells allocated for the LC list*/
    Destroy_Extra(&LC,max_poz);

    /*close the file*/
    fclose(output_file);
    return 0;
}