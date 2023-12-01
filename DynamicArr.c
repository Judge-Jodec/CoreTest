#include "DynamicArr.h"

// Инициализация динамического массива
void initDynamicArray(struct DynamicArray *dynArray,const size_t capacity,const size_t elementSize){
    if(dynArray == NULL){
        dynArray = malloc(sizeof(struct DynamicArray));
        if(dynArray == NULL){
            printf("Error, initDynamicArray(), malloc == NULL");
            exit(EXIT_FAILURE);
        } 
    }
    dynArray->array = malloc(capacity * elementSize);
    if(dynArray->array == NULL){
        printf("Error, initDynamicArray(), malloc == NULL");
        exit(EXIT_FAILURE);
    } 
    dynArray->capacity = capacity;
    dynArray->size = 0;
    dynArray->elementSize = elementSize;
}
// Освобождение памяти
void freeDynamicArray(struct DynamicArray *dynArray){
    free(dynArray->array);
    dynArray->array = NULL;
    dynArray->capacity = 0;
    dynArray->size = 0;
}
// Добавление элемента в динамический массив
void addToDynamicArray(struct DynamicArray *dynArray, const void *element){
    if (dynArray->size >= dynArray->capacity){
        size_t newCapacity = dynArray->capacity * 2;
        dynArray->array = realloc(dynArray->array, newCapacity * dynArray->elementSize);
        if(dynArray->array == NULL){
            printf("Error, addToDynamicArray(), realloc == NULL");
            exit(EXIT_FAILURE);
        } 
        dynArray->capacity = newCapacity;
    }
    char *destination = (char*)dynArray->array + dynArray->size * dynArray->elementSize;
    memcpy(destination, element, dynArray->elementSize);
    if(destination == NULL){
        printf("Error, addToDynamicArray(), memcpy == NULL");
        exit(EXIT_FAILURE);
    } 
    dynArray->size++;
}