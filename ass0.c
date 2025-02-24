#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXL 1024

int next_field (FILE *csv, char *buffer, int max_len){
    //integer to know which value in the buffer array it's stored
    int i = 0;

    //char to store in the buffer
    char p;

    //boolean to check if it's commas so it doesnt go to a next line
    bool commas = false;

    while ((p = fgetc(csv)) != EOF){

        //this will be used to not go to a new line when we encounter a comma inside commas 
        if (p == '"'){
            commas = true;
        }

        //\0 to not store the value in our array
        if (p == ',' && commas == false){
            buffer [i] = '\0';
            return 0;
        }

        //returning 1 when weve reached the end of the line also \0 to not store that value in the array
        if (p == '\n'){
            buffer[i] = '\0';
            return 1;
        }

        //this will store the character in the next value of our buffer array
        if (i < max_len - 1){
            buffer[i++] = p;
        }
    }

    //From here it's to handle the EOF
    if (i > 0){
        buffer[i] = '\0';
        return 1;
    }

    //this will mean we have reached the EOF so it will return -1 and break our while loop in our main function
    return -1;
}

struct POKEMON{
    int pokemon_n;
    char name[20];
    char type1[15];
    char type2[15];
    int tHP;
    int attack;
    int defense;
    int attspeed;
    int defspeed;
    int speed;
    int generation;
    bool legendary;
    char pokedexe[200];
};  

int main(){
    FILE *fi = fopen("pokemon.csv", "r");
    if (fi == NULL){
        return -1;
    }

    char buffer[MAXL]; //creating buffer variable where we store the fields
    int status; //to check if the return value is -1, 0 or 1 
    struct POKEMON pokemons[700]; //we are creating our array of pokemons using the struct
    int v = 0; //value for knowing what to store in the struct 
    int i = 0; //to know which pokemon number it is 


    while ((status = next_field(fi,buffer, sizeof(buffer))) != -1){
        //this will print out our buffer fields 
        printf("%s\n", buffer);
        
        //if we receive 1 from our function (which means theres a new line), it will do a double new line
        if (status == 1){
            printf("\n");
        }

        //we are using atoi to convert strings into integers 
        //also using sizeof() will ensure that when we are storing a string in our array it doesnt overflow 

        if (v == 0){
            pokemons[i].pokemon_n = atoi(buffer);
        }
        else if (v == 1)
        {
            strncpy(pokemons[i].name, buffer, sizeof(pokemons[i].name));
        }
        else if (v == 2)
        {
            strncpy(pokemons[i].type1, buffer, sizeof(pokemons[i].type1));
        }
        else if (v == 3)
        {
            strncpy(pokemons[i].type2, buffer, sizeof(pokemons[i].type2));
        }
        else if (v == 4)
        {
            pokemons[i].tHP = atoi(buffer);
        }
        else if (v == 5)
        {
            pokemons[i].attack = atoi(buffer);
        }
        else if (v == 6)
        {
            pokemons[i].defense = atoi(buffer);
        }
        else if (v == 7)
        {
            pokemons[i].attspeed = atoi(buffer);
        }
        else if (v == 8)
        {
            pokemons[i].defspeed = atoi(buffer);
        }
        else if (v == 9)
        {
            pokemons[i].speed = atoi(buffer);
        }
        else if (v == 10)
        {
            pokemons[i].generation = atoi(buffer);
        }
        else if (v == 11){
            pokemons[i].legendary = (strcmp(buffer, "True") == 0);
        }
        else {
            strncpy(pokemons[i].pokedexe, buffer, sizeof(pokemons[i].pokedexe));
            v = -1; //reset the v values
            i++; //next part of our array struct
        }
        v++;
        
    }

    //close file when finished
    fclose(fi);

    //just a check that the struct is working properly
    printf("%d\n", pokemons[2].speed);
    

    return 0;

}