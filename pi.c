#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    char name[20];
    char password[20];
    char line[40];
    FILE *data_base;

    // Identificacion del usuario 
    printf("Ingrese su usuario \n");
    scanf("%s", name);
    printf("Ingrese la contrasena \n");
    scanf("%s", password);

    // Consulta si el archivo se abrio correctamente
    if((data_base = fopen("data_base.txt", "r")) == NULL){
        printf("ERROR: File couldnt be open");
        exit(1);
    }
   
    // Mientras que hayan datos en el archivo
    while(!feof(data_base)){
        fgets(line, 40, data_base);

        // Si el usuario y la contrasena son iguales 
        if(strstr(line, name) != NULL && strstr(line, password) != NULL){
                printf("Autentificacion correcta, puede acceder al sistema \n");
                break;
        }
    }
    
    if(strstr(line, name) == NULL || strstr(line, password) == NULL){
        printf("Autentificacion incorrecta, no puede acceder al sistema \n");
    }
    fclose(data_base);
    return 0;
}