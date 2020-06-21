#include <stdio.h>
#include <string.h>
#include "iniparser/iniparser.h"

int main(){
    dictionary *ini_file = NULL;
    char *file = "test.ini";
    FILE *tmp = NULL;

    ini_file = iniparser_load(file);
    if(ini_file != NULL){
        printf("Fichier ouvert\n");
                
        iniparser_set(ini_file, "sec1", NULL);
        iniparser_set(ini_file, "sec2", NULL);
        iniparser_set(ini_file, "sec2:bolossage", "True");
        iniparser_set(ini_file, "sec3", NULL);

        printf("iniparser_set marche !\n");
        tmp = fopen(file, "r+");
        if(tmp == NULL){
            printf("Erreur fopen\n");
            return -1;
        }
        //fprintf(tmp, "%s", "[sec1]\n[sec2]\n[sec3]\n");
        //fseek(tmp, 0, SEEK_SET);
        iniparser_dump_ini(ini_file, tmp);
        fclose(tmp);
    }
    else{
        printf("Erreur file parser\n\n");
        return -1;
    }

    iniparser_freedict(ini_file);

    return 0;
}