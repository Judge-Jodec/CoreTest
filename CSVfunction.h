// CSVfunction.h
#include "DynamicArr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef CSVFUNCTION_H
#define CSVFUNCTION_H

#define SIZE_STR                1024U
#define SIZEOF_CHAR             256U
#define SIZE_CAPASITY_ADDRESS   32U
#define SIZE_CAPASITY_TABLE     32U
#define SIZE_CAPASITY_ROW       32U
#define SIZE_CAPASITY_COL       32U
#define SIZE_CAPASITY_FORMULS   32U
#define TYPE_VALUE_TABLE        int

// Структура для хранения формата CSV
struct CSVFormat{
    char *formula;  
    TYPE_VALUE_TABLE value;
    short int was_detect;
};
// Структура для хранения адреса и индекса CSV
struct CSVAddressId{
    char *address;  
    size_t id;
};
// Структура корня бинарного дерева 
//!только! для структуры CSVAddressId
// левый меньше, правый больше    
struct BinTreeNode{
    struct CSVAddressId *data;
    struct BinTreeNode *left;
    struct BinTreeNode *right;  
};
// Инициализация структуры CSVAddressId
struct CSVAddressId* initCSVAddressId(const char* address, const size_t id);
// Инициализация корня бинарного дерева
struct BinTreeNode *initBinTree(const char* address, const size_t id);
// Деление адреса типа А1 на две строки типа А и 1
void splitString(const char *input, char **letters, char **digits);
// Сравнение двух CSVAddressId
int comparisonCSVAddressId(const void* a, const void* b);
// Добавление в бинарное дерево
struct BinTreeNode* addToBinTree(struct BinTreeNode* root, struct BinTreeNode* newNode);
// Метод возврата сортированного массив по бинарному дереву (отсортированный массив)
struct DynamicArray *returnDynamicArrayFromBinTree(struct BinTreeNode* root, struct DynamicArray * arr);
// Инициализация CSVFormat
struct CSVFormat * initCSVFormat(const char *str);
// Чтение файла CSV и формирование таблиц CSV 
//(таблица, названия столбцов, названия рядов, все возможные адреса ячеек таблицы и индексы всех ячеек с формулами)
void readFileFormCSF(const char* pathCSV, struct DynamicArray *arrCSVTable, struct DynamicArray *arrColumnsName,struct DynamicArray *arrRowsName, struct DynamicArray *arrAddressesId, struct DynamicArray *arrIdFormuls);
// Резделение формулы "=ARG1OPARG2" на три подстроки "ARG1" "OP" "ARG2"
void splitFormula(const char *formula, char **operand1, char **operator, char **operand2);
// Проверки корректности адреса
int isValidAddress(const char *address);
// Поиск по формуле
long long resultValueFromCSVFormula(struct DynamicArray *arrCSVTable, size_t *id, struct DynamicArray *arrAddressesId);
// Пробег по всем ячейкам с формулами
void calculatorCSV(struct DynamicArray *arrCSVTable, struct DynamicArray *arrAddressesId, struct DynamicArray *arrIdFormuls);
// Вывод таблицы
void printCSVTable(const struct DynamicArray *arrCSVTable, const struct DynamicArray *arrColumnsName, const struct DynamicArray *arrRowsName);
// Основной цикл работы с таблицей CSV
void processCSV(const char* pathCSV);

#endif