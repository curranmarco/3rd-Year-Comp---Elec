#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_STRING_SIZE 30
#define ARRAY_SIZE 1000000
#define MAX_TRIES ARRAY_SIZE

//this variable will keep track of our collisions when using our insert name function
int collisions = 0;
int numberofterms = 0;



int hash3(char* s){
    int hash = 0;
    while(*s){
        hash = (hash + tolower(*s) - 'a') % (ARRAY_SIZE - 1);
        s++;
    }
    hash = (hash > 0) ? hash : 1; 
    printf("Hash done succesfully: %d\n", hash);
    return hash;
}


typedef struct person{
    //creating our struct with our three variables for the hash table 
    char firstn[MAX_STRING_SIZE];
    char surn[MAX_STRING_SIZE];
    struct person* next;
}person; 

person* hash_table[ARRAY_SIZE];

//int linearprob(int hash_index){
    //linear probing method in case of collisions 
  //  return (hash_index + 1) % ARRAY_SIZE;
//}

//was having problems with the whitespaces so searching wouldn't work properly
void gridwhitespace(char* name){
    //start will point towards the first character in our name
    char *start = name;
    //isspace will check if the first character is a whitespace
    while (isspace((unsigned char)*start)){
        //keeps going until there is no whitespace characters
        start++;
    }

    //memmove is a function that will move the first character that isn't a whitespace to the beginning
    memmove(name, start, strlen(start) + 1);

    //end will point towards the last character of our name
    char *end = name + strlen(name) - 1;
    //this loop will go backwards from the last character of our string
    while (end > name && isspace((unsigned char)*end)){
        //this will replace any whitespace with a null terminator
        *end = '\0';
        end--;
    }
}

void toLower(char* name){
    for (int i = 0; name[i]; i++){
        name[i] = tolower(name[i]);
    }
}

void insertp(char* firstn, char* surn){
    //trim name before inserting it
    gridwhitespace(firstn);
    gridwhitespace(surn); 
    toLower(firstn);
    toLower(surn);
    int hash_index = hash3(surn);
    person* newp = (person*) malloc(sizeof(person));

    if (newp == NULL){
        exit(EXIT_FAILURE);
    }
    
    //Here we will set the first name and the surname 
    strncpy(newp->firstn, firstn, MAX_STRING_SIZE - 1);
    newp->firstn[MAX_STRING_SIZE - 1] = '\0';
    strncpy(newp->surn, surn, MAX_STRING_SIZE - 1);
    newp->surn[MAX_STRING_SIZE - 1] = '\0';
    printf("Inserting person: %s %s at index %d\n", firstn, surn, hash_index);


    newp->next = hash_table[hash_index];
    hash_table[hash_index] = newp;


    //printf("inserting name: %s at index %d\n", buffer, hash_index);

    

    //printf("Stored name: %s at index %d\n", buffer, hash_index);

}

/*void print_hash(){
    printf("\nHere's the Hash Table: \n");
    for (int i = 0; i < ARRAY_SIZE; i++){
        if (hash_table[i].available){ //check if there is an entry to that value in our hash table
            //this will print the name in our hash table
            printf("%s\n", hash_table[i].name);

            //and this will print the amount of times that name has appeared
            printf("%d\n", hash_table[i].frequency);
        }
    }
}*/

void searchsurn(char* surn){
    //trim whitespace before searching
    gridwhitespace(surn);
    toLower(surn);
    int hashi = hash3(surn);
    person* current = hash_table[hashi];
    //variable to make sure our while loop doesn't do more than the amount of names inside our hash function
    bool foundp = false;
    //printf("%s\n", hash_table[hashi].name);
    //printf("%s\n", name);
    //printf("%d\n", strcmp(hash_table[hashi].name, name));

    while (current != NULL){
        //printf("%s\n", "Entra a hacer linear probing");
        if (strcmp(current->surn, surn) == 0){
            foundp = true;
        }
        current = current->next;
    }
    printf("Searching for surname: %s at index %d\n", surn, hashi);


    if (foundp == false){
        printf("%s\n", "No person found with that surname");
    }
    //name hasn't been found
    //printf("\n No encuentra el nombre\n");
    
}

int next_field (FILE *csv, char *firstn, char *surn, int max_len){
    char buffer[2 * MAX_STRING_SIZE];
    if(fgets(buffer, sizeof(buffer), csv) == NULL){
        return -1;
    }

    int parsed = sscanf(buffer, "%*[^,],%*[^,],%[^,],%s", surn, firstn);
    if (parsed == 2){
        printf("Parsed surname: %s, firstname: %s\n", surn, firstn);
    }
    return 0;
}

int main(){
    FILE *fi = fopen("people.csv", "r");
    if (fi == NULL){
        perror("Cannot open file");
        return -1;
    }
    else printf("%s\n", "names.csv loaded succesfully!");

    

    //make sure the hash table works and it doesn't have any data we don't want
    for (int i = 0; i < ARRAY_SIZE; i++){
        hash_table[i] = NULL;
    }

    char firstn[MAX_STRING_SIZE];
    char surn[MAX_STRING_SIZE];
    if (fgets(firstn, sizeof(firstn), fi) == NULL){
        perror("Cannot read header");
        return -1;
    }

    while (next_field(fi, firstn, surn, MAX_STRING_SIZE) != -1){
        insertp(firstn, surn);
    }


    fclose(fi); //making sure our file is closed

    char sname[MAX_STRING_SIZE];
    printf("%s\n", "Enter a name to search or type quit: ");
    while (fgets(sname, MAX_STRING_SIZE, stdin)){
        sname[strcspn(sname, "\n")] = '\0';
        if (strcmp(sname, "quit") == 0){
            break;
        }
        else {
            searchsurn(sname);
        }
        printf("%s\n", "Enter a name to search or type quit: ");
    }

    return 0;
}