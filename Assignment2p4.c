#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//global variables of sizes
#define maxgames 18226
#define titlesz 100
#define platformsz 100
#define top10 10

//structure where we will store information from each game
typedef struct {
    char title[titlesz];
    char platform[platformsz];
    int rating;
    int releasey;
} game;

//funciton to convert the ratings from strings to integers
int atoi_safe(const char *str){
    return atoi(str);
}

//function used to compare the rating opf the two games being compared and see which one is higher 
//used in qsort
int compare(const void *A, const void *B){
    //pointer to each game 
    game *gameA = (game *)A;
    game * gameB = (game *)B;

    return gameB -> rating - gameA -> rating;
}

//function to check if that game is already in the top 10
int titleUsed(char printedTitles[top10][titlesz], int count, const char *title){
    for (int i = 0; i < count; i++){
        //when comparing if returns 0, it means that the title is already in the top 10 so return 1
        if (strcmp(printedTitles[i], title) == 0){
            return 1;
        }
    }
    
    //if title not used then return 0
    return 0;
}

int main(){
    game games[maxgames];
    int amount = 0;
    FILE *file = fopen("t4_ign.csv", "r");

    //just in case our file doesn't work
    if (!file){
        printf("File opening error\n");
        return 1;
    }

    char line[256];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && amount < maxgames){
        //create a key that will act as a token to store our information
        char *key = strtok(line, ",");

        //store the value of the title in our key
        strncpy(games[amount].title, key, titlesz);

        //store the value of the platform in our key
        key = strtok(NULL, ",");
        strncpy(games[amount].platform, key, platformsz);

        //store the value of the rating in our key
        key = strtok(NULL, ",");
        games[amount].rating = atoi_safe(key);

        //store the value of the release year in our key
        key = strtok(NULL, ",");
        games[amount].releasey = atoi_safe(key);

        amount++;
    }

    fclose(file);

    //function to sort our games by score
    qsort(games, amount, sizeof(game), compare);

    //array to store the titles that are in the top 10 already
    char printedTitles[top10][titlesz] = {0};
    int count = 0;


    //printing top 10 games in the last 20 years
    printf("Top 10 Highest rated games:\n");
    for (int i = 0; i < amount && count < top10; i++){
        //only print if the function returns 0
        if (!titleUsed(printedTitles, count, games[i].title) && games[i].releasey >= 2004){
            printf("%d. %s (%s) - Score: %d, Year: %d\n", count + 1, games[i].title, games[i].platform, games[i].rating, games[i].releasey);

            strncpy(printedTitles[count], games[i].title, titlesz);
            count++;
        }
    }

    return 0; 

}