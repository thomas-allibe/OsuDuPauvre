#include <stdio.h>
#include <string.h>

typedef struct{
    int entier;
    char caractere;
    char chaine[10];
    unsigned aha;
    float flotant;
}Data;

int main(){
    FILE *file = fopen("../settings.config", "r+");
    Data donnee, lecture;
    donnee.entier = -127;
    donnee.caractere = 'D';
    strcpy(donnee.chaine, "abcde");
    donnee.aha = 3940;
    donnee.flotant = 3.1416;

    lecture.entier = 0;
    lecture.caractere = 'O';
    strcpy(lecture.chaine, "vide");
    lecture.aha = 0;
    lecture.flotant = 0;

    fwrite(&donnee, sizeof(Data), 1, file);

    fseek(file, 0, SEEK_SET);

    fread(&lecture, sizeof(Data), 1, file);

    printf("int : %d\nchar : %c\nstring : %s\nunsigned : %u\nfloat %f\n",
            lecture.entier, lecture.caractere, lecture.chaine, lecture.aha, lecture.flotant);

    return 0;
}