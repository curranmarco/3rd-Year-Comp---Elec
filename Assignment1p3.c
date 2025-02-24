#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_STRING_SIZE 20
#define ARRAY_SIZE 59
#define MAX_TRIES ARRAY_SIZE

//this variable will keep track of our collisions when using our insert name function
int collisions = 0;
int numberofterms = 0;



int hash3(char* s){
    int hash = 0;
    while(*s){
        hash = 1 + (hash + *s- 'A') % (ARRAY_SIZE - 1);
        s++;
    }
    return hash; 
}

int hash1(char* s){ 
    int hash = 0;
    while(*s){
    hash = (hash + (*s - 'A' + 52)) % ARRAY_SIZE;
    s++; 
    }
    return hash;
}


typedef struct{
    //creating our struct with our three variables for the hash table 
    char name[MAX_STRING_SIZE];
    int frequency;
    bool available;
} person; 

person hash_table[ARRAY_SIZE];

void initiate_hashtable(){
    for (int i = 0; i < ARRAY_SIZE; i++){
        hash_table[i].name[0] = '\0';
        hash_table[i].frequency = 0;
        hash_table[i].available = false;
    }
    //function to initiate the table and make sure it works 
}

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

void insert_name(char* buffer){
    //trim name before inserting it
    gridwhitespace(buffer); 
    int hash_index = hash1(buffer);
    int step = hash3(buffer);
    int howmany = 0;

    //printf("inserting name: %s at index %d\n", buffer, hash_index);

    while (hash_table[hash_index].available && strcmp(hash_table[hash_index].name, buffer)){
        //by doing this we will be using the double hashing method 
        hash_index = (hash_index + step) % ARRAY_SIZE;  
        howmany++;
        collisions++;

        if(howmany >= MAX_TRIES){
            //printf("Hash Table is full or linear probing isn't working properly");
            return;
        }
    }

    if (hash_table[hash_index].available == false){
        strncpy(hash_table[hash_index].name, buffer, MAX_STRING_SIZE - 1); //strncpy just ensures that the string being copied isn't over our size counting the final null value 
        hash_table[hash_index].name[MAX_STRING_SIZE - 1] = '\0'; // Ensure null termination of strncpy
        hash_table[hash_index].frequency = 1; //this will "initiate our frequency" 
        hash_table[hash_index].available = true; //this boolean value will make sure if the name appears again we don't add it to the hash table another time
        numberofterms++;
    }
    //this means that the name is already in our hash table so if will sum up 1 to the frequency
    else hash_table[hash_index].frequency++;

    //printf("Stored name: %s at index %d\n", buffer, hash_index);

}

void print_hash(){
    printf("\nHere's the Hash Table: \n");
    for (int i = 0; i < ARRAY_SIZE; i++){
        if (hash_table[i].available){ //check if there is an entry to that value in our hash table
            //this will print the name in our hash table
            printf("%s\n", hash_table[i].name);

            //and this will print the amount of times that name has appeared
            printf("%d\n", hash_table[i].frequency);
        }
    }
}

int searchname(char* name){
    //trim whitespace before searching
    gridwhitespace(name);
    int hashi = hash1(name);
    int step = hash3(name);
    //variable to make sure our while loop doesn't do more than the amount of names inside our hash function
    int repeats = 0;
    //printf("%s\n", hash_table[hashi].name);
    //printf("%s\n", name);
    //printf("%d\n", strcmp(hash_table[hashi].name, name));

    while (hash_table[hashi].available && strcmp(hash_table[hashi].name, name) != 0){
        //printf("%s\n", "Entra a hacer linear probing");
        hashi = (hashi + step) % ARRAY_SIZE;
        repeats++;

        if (repeats >= MAX_TRIES){
            //this means the name hasn't been found on the hash table so it returns -1
            printf("\nReached max tries\n");
            return -1;
        }
    }

    if (hash_table[hashi].available && strcmp(hash_table[hashi].name, name) == 0){
        //if the name has been found it will return the frequency 
        return hash_table[hashi].frequency;
    }
    //name hasn't been found
    //printf("\n No encuentra el nombre\n");
    return -1;
}

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

int main(){
    FILE *fi = fopen("names.csv", "r");
    if (fi == NULL){
        perror("Cannot open file");
        return -1;
    }
    else printf("%s\n", "names.csv loaded succesfully!");

    //user input name for our search function
    char userName[MAX_STRING_SIZE];
    //this is where we will store the frequency value of the name searched from the input of our user
    int inputFrequency = 0;
    //status is going to check if there is more data to be read or if its the end of the file 
    int status = 0; 
    //buffer to store each name 
    char buffer[20];

    //make sure the hash table works and it doesn't have any data we don't want
    initiate_hashtable();

    while ((status = next_field(fi,buffer, sizeof(buffer))) != -1){
        //will insert the name in our hash table 
        insert_name(buffer);
    }

    fclose(fi); //making sure our file is closed

    //print_hash();
    printf("Number of collisions: %d\n", collisions);
    printf("Number of terms: %d\n", numberofterms);
    

    printf("%s\n","Enter term to get frequency or type quit to escape: ");
    //this will get the input from the user
    fgets(userName, MAX_STRING_SIZE, stdin);

    //This will remove the "\n" character if added
    userName[strcspn(userName, "\n")] = '\0';

    while (strcmp(userName, "quit") != 0){
        inputFrequency = searchname(userName);
    
        if (inputFrequency != -1){
            printf("The name '%s' appears %d times(s).\n", userName, inputFrequency);
        }
        else {
            printf("The name '%s' was not found.\n", userName);
        }
        printf("\nEnter another name or type quit to exit: ");
        fgets(userName, MAX_STRING_SIZE, stdin);
        userName[strcspn(userName, "\n")] = '\0';
    }

    return 0;
}