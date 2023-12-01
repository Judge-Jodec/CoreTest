#include "CSVfunction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
    char *pathCSV = argv[1];
    if (argc != 2){
       printf("Error, the argument must be one:\".\\CSV_Reader.exe <file_name>\"\n");
       return EXIT_FAILURE;
    }
    processCSV(pathCSV);
    return EXIT_SUCCESS;
}
