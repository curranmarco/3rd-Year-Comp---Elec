#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



//function 
int next_field(FILE *csv, char *buffer, int max_len){
    //value for the buffer array 
    int i = 0;
    //variable to store each character previously to the buffer
    char p;

    bool commas = false; 

    while ((p = fgetc(csv)) != EOF ){

        if (p == '"'){
            commas = true;
        }

        //we use this so that the function stops reading when we encounter a comma because its a different field
        if (p == ',' && commas == false || p == '\n'){
            buffer[i] = '\0';
            
            if (p == '\n'){
                //returning 2 will mean that it is the end of the line so it does double space
                return 2;
                
                
            }
            //This means that we have reached the end of the field 
            else return 1;
            

        }
        else {
            while (commas == true){
                p = fgetc(csv);
                if (p == '"'){
                    commas = false;
                    break; 
                }
                buffer[i++] = p;
                
            }

           
            
        }
        if(i < max_len -1){
            //This will store the character in the next space in the buffer
            buffer[i++] = p;
        }
    }

    //This will terminate our last field
    buffer[i] = '\0'; 
   if (i > 0){
    return 1;
   }
    return -1; //This is when we have reached the end of the file so our main loop breaks
}



int main(){
    
    FILE *fi = fopen("pokemon.csv", "r");
    if (fi == NULL) {
        perror("Error opening file");
        return 1;
    }

    char buffer[1024];
    int status; //Can be 1, 0 -1

    while (status = next_field(fi, buffer, sizeof(buffer)) != -1){
        if(status == 2){
            printf("\n");
        }
        
        printf("%s\n", buffer); //This will print our buffer
        
        
    } 


    fclose(fi);
    return 0;
}