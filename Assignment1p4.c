#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_STRING_SIZE 110
#define ARRAY_SIZE 1000000
#define MAX_TRIES ARRAY_SIZE

//this variable will keep track of our collisions when using our insert name function
int collisions = 0;
int numberofterms = 0;



int hash3(char* s){
    int hash = 0;
    //this loop keeps going until it reaches a null terminator 
    while(*s){
        //turns the value *s to a number between 0-25 and adds it to the hash
        hash = (hash + tolower(*s) - 'a') % (ARRAY_SIZE - 1);
        s++;
    }

    //this line is just to check that the hash function never returns 0, so if the value is 0 it returns 1
    hash = (hash > 0) ? hash : 1; 
    //printf("Hash done succesfully: %d\n", hash);
    return hash;
}


typedef struct person{
    //creating our struct with our three variables for the hash table 
    char firstn[MAX_STRING_SIZE];
    char surn[MAX_STRING_SIZE];
    //this is a pointer used to point to the next person on the linked list
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

void removeq(char* name){
    //this pointer points to where we want to write the next character of the string (starts at the beginning)
    char *wrptr = name;
    //this one point to the current character we are reading
    char * readptr = name;

    //this will run as long as the pointer points to a non null character
    while (*readptr){
        //this is checking if its pointing to a ""
        if (*readptr != '"'){
            //this copies the character to the writing pointer meaning it is not a ""
            *wrptr = *readptr;
            wrptr++;
        }
        //if it's a "" it simply moves and reads the next character
        readptr++;
    }
    *wrptr = '\0';
}

void insertp(char* firstn, char* surn){
    //trim name before inserting it
    gridwhitespace(firstn);
    gridwhitespace(surn); 
    toLower(firstn);
    toLower(surn);
    int hash_index = hash3(surn);
    //this is allocating memmoty for a new person structure
    person* newp = (person*) malloc(sizeof(person));

    //in case of memory allocation failure it exits
    if (newp == NULL){
        exit(EXIT_FAILURE);
    }
    
    //Here we will set the first name and the surname 
    //this will copy the firstn data into the newp->firstn data
    strncpy(newp->firstn, firstn, MAX_STRING_SIZE - 1);
    newp->firstn[MAX_STRING_SIZE - 1] = '\0';
    //same here but with the surname
    strncpy(newp->surn, surn, MAX_STRING_SIZE - 1);
    newp->surn[MAX_STRING_SIZE - 1] = '\0';
    //printf("Inserting person: %s %s at index %d\n", firstn, surn, hash_index);

    if (hash_table[hash_index] != NULL){
        collisions++;
    }

    //this part is to handle collisions by using chaining
    //we will point next to newp to the start of the list at our hash table
    //if someone is stored there, they become the next
    newp->next = hash_table[hash_index];
    //this updates so the head points to the new person
    hash_table[hash_index] = newp;

    numberofterms++;


    //printf("inserting name: %s at index %d\n", buffer, hash_index);

    

    //printf("Stored name: %s at index %d\n", buffer, hash_index);

}



void searchsurn(char* surn){
    //trim whitespace before searching
    gridwhitespace(surn);
    toLower(surn);
    int hashi = hash3(surn);
    //this will set the current vairable to point to the head of the linked list at that hash
    person* current = hash_table[hashi];

    //this variable is to check if any matches are founf
    bool found = false;
    
    
    //printf("%s\n", hash_table[hashi].name);
    //printf("%s\n", name);
    //printf("%d\n", strcmp(hash_table[hashi].name, name));

    //printf("Searching for surname: %s at index %d\n", surn, hashi);

    //this while loop will go through all the people stored on that hash index until it becomes null
    while (current != NULL){
        //printf("%s\n", "Entra a hacer linear probing");
        //strcmp will compare the name stored in the current person to the searched surname
        if (strcmp(current->surn, surn) == 0){
            printf("%s %s\n", current->firstn, current->surn);
            found = true;
            
        }
        //this will go to the next person if there is 
        current = current->next;
    }

    if (found == false){
        printf("No person found with that surname.\n");
    }    
}

int next_field (FILE *csv, char *firstn, char *surn, int max_len){
    //this is the buffer declared to store the information
    char buffer[2 * MAX_STRING_SIZE];
    if(fgets(buffer, sizeof(buffer), csv) == NULL){
        return -1;
    }

    //this is just for debugging to see what it's reading
    //printf("Raw CSV: %s\n", buffer);

    //this function is used to separate the buffer with the commas 
    char *token = strtok(buffer, ",");
    //this is to know which field we are tracking
    int fieldc = 0;
    //temp variables to hold the extracted names and surnames
    char *tempfirstn = NULL, *tempsurn = NULL;

    //here we process the different tokens while it is not = NULL
    while(token != NULL){
        //if the field count is = 2 we know it is the surname so our temp variable 
        //will be equal to the surname
        if (fieldc == 2){
            tempsurn = token;
        }
        //same here but with the first name 
        else if (fieldc == 3){
            tempfirstn = token;
        }

        //here we are getting thr next token
        token = strtok(NULL, ",");
        fieldc++;
    }

    //this means these fields weren't found
    if (tempsurn == NULL || tempfirstn == NULL){
        return -1;
    }

    //removing quotes from names
    removeq(tempfirstn);
    removeq(tempsurn);

    //copying the names and surnames stored in the temp variables to the actual variables
    strncpy(surn, tempsurn, MAX_STRING_SIZE - 1);
    strncpy(firstn, tempfirstn, MAX_STRING_SIZE - 1);

    //this is just getting rid of whitespaces and making it easier for searching
    gridwhitespace(surn);
    gridwhitespace(firstn);
    toLower(surn);
    toLower(firstn);

    //printf("Parsed name: %s, surname: %s\n", firstn, surn);
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
    //this is to check and skip the header
    if (fgets(firstn, sizeof(firstn), fi) == NULL){
        perror("Cannot read header");
        return -1;
    }
    printf("Header skipped.\n");

    //inserting names and surnames 
    while (next_field(fi, firstn, surn, MAX_STRING_SIZE) != -1){
        insertp(firstn, surn);
    }


    fclose(fi); //making sure our file is closed

    printf("Number of collisions: %d\n", collisions);
    printf("Number of terms: %d\n", numberofterms);

    char sname[MAX_STRING_SIZE];
    printf("%s\n", "Enter a name to search or type quit: ");
    while (fgets(sname, MAX_STRING_SIZE, stdin)){
        //this will get rid of any new line characters
        sname[strcspn(sname, "\n")] = '\0';
        //comparing it to quit to stop the while loop
        if (strcmp(sname, "quit") == 0){
            break;
        }
        else {
            //searching for the names
            printf("Here is a list of names with that surname: \n");
            searchsurn(sname);
        }
        printf("%s\n", "Enter a name to search or type quit: ");
    }

    return 0;
}