#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILEPATH "init_poke.csv"
#define MAXSIZE 5

typedef struct pokemon{
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


// function prototypes

void pokeViewer();
void statViewer(pokemon* list, int index);
void sheetDisplay(int pos);
char* bars(int prop);

char* typeConverter(int num);
int setType(int value, int pos, int strict);
int set100(int value, char* pos, int strict);
char menuInputCheck(char* text, char* valid, int anyKey, pokemon* list, int index);
int numInputCheck(char* text, int max, int sheet, pokemon* list, int index);
int infoAdder(char* text, int obj, char* pos, pokemon* list, int index);

void main(){
    // local variables
    FILE *fp;
    char row[1000];
    int countRow = 0;

    char userInput;
    int pokeIndex;
    pokemon toAdd[1];

    //
    // loading the file
    //

    // https://en.cppreference.com/w/c/program/EXIT_status
    menuInputCheck("A file named 'init_poke.csv' must exist in the folder where the executable is run for pre-polulated data\nPress any key to continue\n\n", "", 1, list, -1);
    fp = fopen(FILEPATH, "r");

    if (fp == NULL){
       fprintf(stderr, "\n#%d - No such file exists - %s\n", __LINE__, __FILE__);
       exit(EXIT_FAILURE);
    }

    // grabs values and some error checking
    // https://stdin.top/posts/csv-in-c/
    // https://en.cppreference.com/w/c/io/fgets
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

    //
    // main menu
    //

    do { // while (userInput != 'q')

        // adds menu elements
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

        if (pokeSize < MAXSIZE){
            strcat(menuText, "[A]dd new pokemon\n");
            strcat(selText, "a");
        }

        strcat(menuText, "[Q]uit and save\n\n");
        strcat(selText, "q");

        // menu prompt
        userInput = menuInputCheck(menuText, selText, 0, list, -1);

        switch (userInput) {
            case 'v': // [V]iew individual stats

                pokeIndex = numInputCheck("Select pokedex entry to view\n[X] to return to menu\n\n", pokeSize, 0, list, -1);
                while(pokeIndex != -1) {                   
                    pokeIndex = numInputCheck("Select another pokedex entry to view\n[X] to return to menu\n\n", pokeSize, 0, list, --pokeIndex);
                }
                break;

            case 'r': // [R]emove pokemon

                pokeIndex = numInputCheck("Select pokedex entry to remove\n[X] to return to menu\n\n", pokeSize, 0, list, -1);
                if (pokeIndex == -1) break; // if user selects [X]

                userInput = menuInputCheck("Remove the pokemon listed above?\nThis action is irreversable\n[Y]es\n[N]o\n\n", "ynx", 0, list, --pokeIndex);
                if (userInput != 'y') break; // if user selects [N]
                    
                // pokemon removal loop
                for(int i = pokeIndex; i < MAXSIZE; i++){
                    strcpy(list[i].name, list[i + 1].name);
                    list[i].type1 = list[i + 1].type1;
                    list[i].type2 = list[i + 1].type2;
                    list[i].hp = list[i + 1].hp;
                    list[i].atk = list[i + 1].atk;
                    list[i].def = list[i + 1].def;
                }

                pokeSize--;
                menuInputCheck("Successfully removed!\nPress any key to continue\n\n", "", 1, list, -1);
                break;

            case 'a': // [A]dd new pokemon
                
                // initializing values
                memset(toAdd[0].name, '\0', 100);
                toAdd[0].type1 = 0;
                toAdd[0].type2 = 0;
                toAdd[0].hp = 0;
                toAdd[0].atk = 0;
                toAdd[0].def = 0;

                do {
                    // prompts for name
                    system("cls");
                    pokeViewer();
                    printf("Name of the pokemon to be added\n");
                    memset(toAdd[0].name, '\0', 100);
                    rewind(stdin);
                    scanf("%[^\n,]s", &toAdd[0].name);

                    // confirmation prompt for name
                    userInput = menuInputCheck("Is the name correct?\n[Y]es\n[N]o\n\n", "ynx", 0, toAdd, 0);
                } while (userInput != 'y');

                // adds individual values
                toAdd[0].type1 = infoAdder("Enter first type from the list above\n\n", 1, "", toAdd, 0);
                toAdd[0].type2 = infoAdder("Enter second type from the list above\n\n", 2, "", toAdd, 0);
                toAdd[0].hp = infoAdder("Enter HP of pokemon [1 - 100]\n\n", 3, "HP", toAdd, 0);
                toAdd[0].atk = infoAdder("Enter Attack of pokemon [1 - 100]\n\n", 3, "ATK", toAdd, 0);
                toAdd[0].def = infoAdder("Enter Defense of pokemon [1 - 100]\n\n", 3, "DEF", toAdd, 0);

                // confirmation prompt for pokemon
                userInput = menuInputCheck("Add this pokemon to the pokedex?\n[Y]es\n[N]o\n\n", "ynx", 0, toAdd, 0); 
                if (userInput != 'y') break; // if user selects [N]

                // adding of pokemon to list
                strcpy(list[pokeSize].name, toAdd[0].name);
                list[pokeSize].type1 = toAdd[0].type1;
                list[pokeSize].type2 = toAdd[0].type2;
                list[pokeSize].hp = toAdd[0].hp;
                list[pokeSize].atk = toAdd[0].atk;
                list[pokeSize].def = toAdd[0].def;
                pokeSize++;
                menuInputCheck("Successfully added!\nPress any key to continue\n\n", "", 1, list, pokeSize - 1);
                break;
        }

    } while (userInput != 'q');

    //
    // saving to file
    //

    fp = fopen("output_poke.csv", "w");
    fprintf(fp, ",name,type1,type2,hp,atk,def\n");

    // main loop
    for (int i = 0; i < MAXSIZE - 1; i++){
        fprintf(fp, "%d,%s,%d,%d,%d,%d,%d\n", i, list[i].name, list[i].type1, list[i].type2, list[i].hp, list[i].atk, list[i].def);
    }

    // confirmation
    menuInputCheck("Successfully saved to 'output_poke.csv'\nPress any key to continue\n\n", "", 1, list, -1);

}





// displays table of all pokemon
void pokeViewer(){
    if (!pokeSize) return; // zero size should display nothing

    printf("No. |    Name    | Type 1 | Type 2 | HP | ATK | DEF |\n"); // header

    // main loop
    for (int i = 0; i < pokeSize; i++){
        printf("%3.2d %13.13s %8s %8s %4d %5d %5d\n", i+1, list[i].name, typeConverter(list[i].type1), typeConverter(list[i].type2), list[i].hp, list[i].atk, list[i].def);
    }
    printf("\n");

}

// displays detailed stats of a single pokemon
void statViewer(pokemon* list, int index){
    printf("Name: %s\n", list[index].name);
    printf("Type: %s %s\n", typeConverter(list[index].type1), list[index].type2 ? typeConverter(list[index].type2) : "");
    printf("HP  : [%s] %3d\n", bars(list[index].hp), list[index].hp);
    printf("ATK : [%s] %3d\n", bars(list[index].atk), list[index].atk);
    printf("DEF : [%s] %3d\n\n", bars(list[index].def), list[index].def);
}

// displays types sheet
void sheetDisplay(int pos){
    for (int i = 2 - pos; i <= 18; i++){
        printf("%02d: %s\n", i, types[i]);
    }
    printf("\n");
}

// displays filled bars proportional to 100
char* bars(int prop){
    char* output = calloc(10, sizeof(char));
    memset(output, '_', 10);
    memset(output, 'o', (prop < 10 && prop > 0) ? 1 : prop/10);
    return output;
}

// converts numeric types to text types
char* typeConverter(int num){

    if (num >= 0 && num <= 18)
        return types[num];
    else
        return "ERROR";
}

// error checking for type1 and type2 fields
//
// [value] from user
// [pos] 1 = type1, 2 = type 2
// [strict] 1 to exit immediately for errors, 0 otherwise
int setType(int value, int pos, int strict){

    if (pos == 1 && value == 0){
        printf("The first type cannot be N/A\n");
        strict == 1 ? exit(EXIT_FAILURE) : 0;
        return -2; 
    }

    if (value >= 0 && value <= 18){
        return value;
    } else {
        printf("Enter a valid number!\n");
        strict == 1 ? exit(EXIT_FAILURE) : 0;
        return -3;
    }
}

// error checking for HP, ATK, and DEF fields
//
// [value] from user
// [pos] HP, ATK, DEF
// [strict] 1 to exit immediately for errors, 0 otherwise
int set100(int value, char* pos, int strict){

    if (value >= 1 && value <= 100){
        return value;
    } else {
        printf("%s entered should be between 1 to 100", pos);
        strict == 1 ? exit(EXIT_FAILURE) : 0;
        return -1;
    }
}

// prompts menu from user and accepts character inputs. also does input validation
//
// [text] to display
// [valid] valid characters
// [anyKey] bypass for valid
// [list][index] for displaying pokemon statistics
char menuInputCheck(char* text, char* valid, int anyKey, pokemon* list, int index){

    char input = 0;
    char* ibuffer = calloc(2, sizeof(char));

    while (true) {

        system("cls");
        pokeViewer();
        (index != -1) ? (statViewer(list, index)) : 0;

        if (strpbrk(ibuffer, valid) && input)
            return input;
        else if (input)
            printf("%c\nInvalid input!\n\n", input);

        printf("%s", text);

        if (anyKey){
            getche();
            return 'x';
        }

        do {
                input = tolower(getche());
        } while (isspace(input) != 0); //do-while loop allows for white-space input not to count

        memset(ibuffer, input, 1);
    }
}

// prompts menu from user and accepts numeric inputs. also does input validation
//
// [text] to display
// [max] maximum value allowed
// [sheet] 1 to display types sheet, 0 otherwise
// [list][index] for displaying pokemon statistics
int numInputCheck(char* text, int max, int sheet, pokemon* list, int index){

    int input = -9949; // arbitrary number
    char* cintput = calloc(5, sizeof(char));

    while (true) {

        system("cls");
        pokeViewer();
        (index != -1) ? (statViewer(list, index)) : 0;
        sheet ? sheetDisplay(sheet) : 0;

        if (input == -9949){}
        else if (strcmp(cintput, "x") == 0 || strcmp(cintput, "X") == 0){
            return -1;
        } else if (input == 0 && !(sheet == 2)){
            printf("Enter a number!\n\n");
        } else if (input > max || input < 0){
            printf("Enter a valid number!\n\n");
            if (sheet) return -2;
        } else{
            free(cintput);
            return input;
        }

        printf("%s", text);

        scanf("%s", cintput);
        input = atoi(cintput);
    }
}


// main code for adding pokemon data
//
// [text] to display
// [int] 1 = type1, 2 = type2, 3 = 'others'
// [pos] HP, ATK, DEF
// [list][index] for displaying pokemon statistics
int infoAdder(char* text, int obj, char* pos, pokemon* list, int index){

    int value = 0;
    char* blockText = calloc(200, sizeof(char));
    
    if (obj != 3){ // for type1 and type2 fields

        do{

        strcat(blockText, text);
        value = setType(numInputCheck(blockText, 18, obj, list, index), obj, 0);
        memset(blockText, '\0', 200);

        value == -2 ? strcat(blockText, "The first type cannot be N/A\n\n") : 0;
        value == -3 ? strcat(blockText, "Enter a valid number!\n\n") : 0;

        } while (value < 0);

    } else { // for HP, ATK, and DEF fields

        do{
        strcat(blockText, text);
        value = set100(numInputCheck(blockText, 100, 0, list, index), pos, 0);
        memset(blockText, '\0', 200);

        value == -1 ? strcat(blockText, "Finish filling up the values first!\n\n") : 0;
        value == -2 ? strcat(blockText, pos) : 0;
        value == -2 ? strcat(blockText, " entered should be between 1 to 100\n\n") : 0;

        } while (value < 0);
    }

    return value;
}