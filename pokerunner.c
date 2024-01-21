#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILEPATH "C:/Users/sarco/OneDrive - DEPED REGION 7-2/Desktop/pokepractice/init_poke.csv"
#define MAXSIZE 5

typedef struct {
    char name[100];
    int type1;
    int type2;
    int hp;
    int atk;
    int def;
} pokemon;

// global variables
pokemon list[MAXSIZE];
int pokeSize = 0;

void pokeViewer();
void statViewer(int index);

char* typeConverter(int num){

    char* types[19] = {
        "N/A",
        "Normal",
        "Fighting",
        "Flying",
        "Poison",
        "Ground",
        "Rock",
        "Bug",
        "Ghost",
        "Steel",
        "Fire",
        "Water",
        "Grass",
        "Electric",
        "Psychic",
        "Ice",
        "Dragon",
        "Dark",
        "Fairy"
    };

    if (num >= 0 && num <= 18)
        return types[num];
    else
        return "ERROR";

}

int setType(int value, int pos, int strict){

    //printf("%d\n", value);

    if (pos == 1 && value == 0){
        printf("The first type cannot be N/A\n");
        strict == 1 ? exit(EXIT_FAILURE) : 0;
    }

    if (value >= 0 && value <= 18){
        return value;
    } else {
        printf("Type entered should be between 0 to 18\n");
        strict == 1 ? exit(EXIT_FAILURE) : 0;
    }
}

int set100(int value, char* pos, int strict){

    //printf("%d\n", value);

    if (value >= 1 && value <= 100){
        return value;
    } else {
        printf("%s entered should be between 1 to 100", pos);
        strict == 1 ? exit(EXIT_FAILURE) : 0;
    }
}

char menuInputCheck(char* text, char* valid, int hideList){

    char input = 0;
    char* ibuffer = calloc(2, sizeof(char));

    while (true) {

        if (!hideList){
            system("cls");
            pokeViewer();
        } else
            hideList--;

        if (strpbrk(ibuffer, valid) && input)
            return input;
        else if (input)
            printf("%c\nInvalid input!\n\n", input);

        printf("%s", text);

        do {
                input = tolower(getche());
        } while (isspace(input) != 0); //do-while loop allows for white-space input not to count

        memset(ibuffer, input, 1);

    }

}

int numInputCheck(char* text, int max, int hideList){

    int input = -1;
    char* cintput = calloc(5, sizeof(char));

    while (true) {

        if (!hideList){
            system("cls");
            pokeViewer();
        } else
            hideList--;

        if (input == -1){}
        else if (strcmp(cintput, "x") == 0 || strcmp(cintput, "X") == 0){
            return -1;
        } else if (input == 0){
            printf("Enter a number!\n\n");
        } else if (input > max || input < 1){
            printf("Enter a valid number!\n\n");
        } else{
            free(cintput);
            return input;
        }

        printf("%s", text);

        scanf("%s", cintput);
        input = atoi(cintput);

    }
}

char* bars(int prop){

    char* output = calloc(10, sizeof(char));
    memset(output, '_', 10);
    memset(output, 'o', prop < 10 ? 1 : prop/10);

    return output;
}

void main(){
    // loading the file
    // https://en.cppreference.com/w/c/program/EXIT_status
    printf("A file named 'init_poke.csv' must exist in the folder where the executable is run for pre-polulated data\n");
    printf("Press any key to continue\n");
    getche();

    FILE *fp;
    char row[1000];
    fp = fopen(FILEPATH, "r");

    if (fp == NULL){
       fprintf(stderr, "\n#%d - No such file exists - %s\n", __LINE__, __FILE__);
       exit(EXIT_FAILURE);
    }

    // grabs values and some error checking
    // https://stdin.top/posts/csv-in-c/
    // https://en.cppreference.com/w/c/io/fgets

    int countRow = 0;

    while (fgets(row, 1000, fp) != NULL)
    {
        // checks if header is correct
        if (countRow == 0 && strcmp(row, ",name,type1,type2,hp,atk,def\n") != 0){
            fprintf(stderr, "\n#%d - Incorrect file format\n", __LINE__);
            exit(EXIT_FAILURE);
        } else if (countRow == 0){
            countRow++;
            continue;
        }

        // grabs individual values from rows
        strtok(row, ","); // skipping index; index is for csv
        strcpy(list[pokeSize].name, strtok(NULL, ","));
        list[pokeSize].type1 = setType(atoi(strtok(NULL, ",")), 1, 1);
        list[pokeSize].type2 = setType(atoi(strtok(NULL, ",")), 2, 1);
        list[pokeSize].hp = set100(atoi(strtok(NULL, ",")), "HP", 1);
        list[pokeSize].atk = set100(atoi(strtok(NULL, ",")), "Attack", 1);
        list[pokeSize].def = set100(atoi(strtok(NULL, ",")), "Defense", 1);

        pokeSize++;
        countRow++;

        if (pokeSize > MAXSIZE || countRow > MAXSIZE + 1){
            fprintf(stderr, "\n#%d - Maximum size reached\n", __LINE__);
            exit(EXIT_FAILURE);
        }
    }

    system("cls");

    // main menu

    char userInput;
    int pokeIndex;

    do { // while (userInput != 'q')

        char* menuText = calloc(110, sizeof(char));
        char* selText = calloc(10, sizeof(char));
        memset(menuText, '\0', 110);
        memset(selText, '\0', 10);
        strcat(menuText, "Select one of the following:\n");
        strcat(selText, "q");
        
        if (pokeSize > 0){
            strcat(menuText, "[V]iew individual stats\n[R]emove pokemon\n");
            strcat(selText, "vr");
        }

        if (pokeSize < 50){
            strcat(menuText, "[A]dd new pokemon\n");
            strcat(selText, "a");
        }

        strcat(menuText, "[Q]uit and save\n\n");
        strcat(selText, "q");


        userInput = menuInputCheck(menuText, selText, 0);

        switch (userInput) {
            case 'v':

                pokeIndex = numInputCheck("Select pokedex entry to view\n[X] to return to menu\n\n", pokeSize, 0);

                while(pokeIndex != -1) {                   
        
                    system("cls");
                    pokeViewer();
                    statViewer(--pokeIndex);
                    
                    pokeIndex = numInputCheck("Select another pokedex entry to view\n[X] to return to menu\n\n", pokeSize, 1);

                }
 
                break;

            case 'r':

                pokeIndex = numInputCheck("Select pokedex entry to remove\n[X] to return to menu\n\n", pokeSize, 0);

                printf("To be removed:\n");
                statViewer(--pokeIndex);

                userInput = menuInputCheck("Are you sure?\nThis action is irreversable\n[Y]es\n[N]o\n", "ynxq", 1);

                if (userInput != 'y')
                    break;

                for(int i = pokeIndex; i < MAXSIZE; i++){
                    
                    strcpy(list[i].name, list[i + 1].name);
                    list[i].type1 = list[i + 1].type1;
                    list[i].type2 = list[i + 1].type2;
                    list[i].hp = list[i + 1].hp;
                    list[i].atk = list[i + 1].atk;
                    list[i].def = list[i + 1].def;

                }

                pokeSize--;

                system("clr");
                pokeViewer();
                printf("Successfully removed!\nPress any key to continue\n");
                getche();

            default:
                break;
        }

    } while (userInput != 'q');
    

}



void pokeViewer(){

    system("cls");

    printf("No. |    Name    | Type 1 | Type 2 | HP | ATK | DEF |\n");

    for (int i = 0; i < pokeSize; i++){
        printf("%3.2d %13.13s %8s %8s %4d %5d %5d\n", i+1, list[i].name, typeConverter(list[i].type1), typeConverter(list[i].type2), list[i].hp, list[i].atk, list[i].def);
    }

    printf("\n");

}

void statViewer(int index){

    printf("Name: %s\n", list[index].name);
    printf("Type: %s %s\n", typeConverter(list[index].type1), list[index].type2 ? typeConverter(list[index].type2) : "");
    printf("HP  : [%s]\n", bars(list[index].hp));
    printf("ATK : [%s]\n", bars(list[index].atk));
    printf("DEF : [%s]\n\n", bars(list[index].def));

}