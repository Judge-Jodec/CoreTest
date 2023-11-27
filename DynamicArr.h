// DynamicArr.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DYNAMICARR_H
#define DYNAMICARR_H

#include <stddef.h>

struct DynamicArray {
    void *array;        // Указатель на начало массива
    size_t capacity;    
    size_t size;        
    size_t elementSize; // Размер одного элемента массива (в байтах)
};
// Инициализация динамического массива
void initDynamicArray(struct DynamicArray *dynArray,const size_t capacity,const size_t elementSize);
// Освобождение памяти
void freeDynamicArray(struct DynamicArray *dynArray);
// Добавление элемента в динамический массив
void addToDynamicArray(struct DynamicArray *dynArray, const void *element);

#endif