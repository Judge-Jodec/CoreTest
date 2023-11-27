#include "CSVfunction.h"
// Инициализация структуры CSVAddressId
struct CSVAddressId* initCSVAddressId(const char* address, const size_t id){
    struct CSVAddressId *csvAddressId = malloc(SIZEOF_CSV_ADDRESS_ID);
    if (csvAddressId == NULL){
        printf("Error, initCSVAddressId(), malloc == NULL");
        exit(EXIT_FAILURE);
    } 
    csvAddressId->address = malloc((strlen(address)+1) * SIZEOF_CHAR);
    if (csvAddressId->address == NULL){
        printf("Error, initCSVAddressId(), malloc == NULL");
        exit(EXIT_FAILURE);
    } 
    strcpy(csvAddressId->address, address);
    csvAddressId->id = id;
    return csvAddressId;
}
// Инициализация корня бинарного дерева
struct BinTreeNode* initBinTree(const char* address, const size_t *id){
    struct BinTreeNode *root = malloc(SIZEOF_BIN_TREE);
    if (root == NULL){
        printf("Error, initBinTree(), malloc == NULL");
        exit(EXIT_FAILURE);
    } 
    root->left = NULL;
    root->right = NULL;
    root->data = initCSVAddressId(address, *id);
    return root;
}
// Деление адреса типа А1 на две строки типа А и 1
void splitString(const char *input, char **letters, char **digits){
    size_t length = strlen(input);
    *letters = malloc(length + 1);
    *digits = malloc(length + 1);
    if (*letters == NULL || *digits == NULL){
        printf("Error, splitString(), malloc == NULL");
        exit(EXIT_FAILURE);
    }
    size_t letterid = 0;
    size_t digitid = 0;
    for (size_t i = 0; i < length; ++i){
        if (isalpha(input[i])){
            (*letters)[letterid++] = input[i];
        } else if (isdigit(input[i])){
            (*digits)[digitid++] = input[i];
        }
    }
    (*letters)[letterid] = '\0';
    (*digits)[digitid] = '\0';
}
// Сравнение двух CSVAddressId
int comparisonCSVAddressId(const void* a, const void* b){
    //Дает (1, 0, -1), -1 - a больше b
    //a2 < b1 
    //Если 0 (a==b), то пора проверить все адреса
    char *aString;
    char *aDigit;
    char *bString;
    char *bDigit;
    splitString(((struct CSVAddressId*)a)->address, &aString, &aDigit);
    splitString(((struct CSVAddressId*)b)->address, &bString, &bDigit);
    int result = strcoll(aString, bString);
    if (result == 0){
        result = strcoll(aDigit, bDigit);
    }
    free(aString);
    free(aDigit);
    free(bString);
    free(bDigit);
    return result;
}
// Добавление в бинарное дерево
struct BinTreeNode* addToBinTree(struct BinTreeNode* root, struct BinTreeNode* newNode){
    int boolComparison = comparisonCSVAddressId(root->data, newNode->data);
    if (boolComparison < 0){
        if (root->right == NULL){
            root->right = newNode;
            return root;
        }
        else
            return addToBinTree(root->right, newNode);
    }
    else {
        if (root->left == NULL){
            root->left = newNode;
            return root;
        }
        else {
            return addToBinTree(root->left, newNode);
        }
    } 
}
// Метод возврата сортированного массив по бинарному дереву (отсортированный массив)
struct DynamicArray *returnDynamicArrayFromBinTree(const struct BinTreeNode* root, struct DynamicArray * arr){
    if (root->left != NULL){
        returnDynamicArrayFromBinTree(root->left, arr);
    }
    addToDynamicArray(arr, root->data);
    if (root->right != NULL){
        returnDynamicArrayFromBinTree(root->right, arr);
    }
    return arr;
}
// Инициализация CSVFormat
void * initCSVFormat(const char *str){
    struct CSVFormat *CSV = malloc(SIZEOF_CSV_FORMAT);
    if (CSV == NULL){
        printf("Error, initCSVFormat(), malloc == NULL");
        exit(EXIT_FAILURE);
    } 
    if (str[0] == '='){
        CSV->formula = malloc((strlen(str)+1) * SIZEOF_CHAR);
        if (CSV->formula == NULL){
            printf("Error, initCSVFormat(), malloc == NULL");
            exit(EXIT_FAILURE);
        }       
        CSV->formula = strncpy(CSV->formula, str, strlen(str) + 1);
        CSV->value = 0;
    }
    else {
        CSV->formula = NULL;
        CSV->value = atoi(str);
    }
    CSV->was_detect = 0;
    return CSV;
}
// Чтение файла CSV и формирование таблиц CSV 
//(таблица, названия столбцов, названия рядов, все возможные адреса ячеек таблицы и индексы всех ячеек с формулами)
void readFileFormCSF(const char* pathCSV, struct DynamicArray *arrCSVTable, struct DynamicArray *arrColumnsName,struct DynamicArray *arrRowsName, struct DynamicArray *arrAddressesId, struct DynamicArray *arrIdFormuls){
    FILE* file = fopen(pathCSV, "rb");
    if (!file){
        printf("Error, file opening failed");
        exit(EXIT_FAILURE);
    }
    int numColumns  = 0, 
        numRows    = 0, 
        maxColumns  = 0, 
        bufferIndex = 0;
    char currentChar;
    char buffer[SIZE_STR];
    struct BinTreeNode *binTreeRoot = NULL;
    while ((currentChar = fgetc(file)) != EOF){
        if (currentChar == ',' || currentChar == '\n' || currentChar == '\0'){
            buffer[bufferIndex] = '\0';
            // Если в конце ряда нет значения
            if ((buffer[0] == '\r') && numRows != 0 && numColumns != 0){
                    printf("Error, readFileFormCSF()num1, \"\\r\"");
                    exit(EXIT_FAILURE);   
            }
            // Если первый ряд, то запоминаем название столбцов
            if (numRows == 0){
                if (currentChar == '\n'){
                    buffer[bufferIndex-1] = '\0';
                    addToDynamicArray(arrColumnsName, &buffer);
                    maxColumns = numColumns;
                }
                else if (currentChar == ','){
                    if (numColumns!=0){
                        addToDynamicArray(arrColumnsName, &buffer);
                    }        
                }
            }
            //Если первый столбец, то запоминаем названия ряда
            else if (numColumns == 0 && currentChar == ','){
                if (buffer[0]=='\0'){
                    printf("Error, readFileFormCSF()num2, \"\\0\"");
                    exit(EXIT_FAILURE); 
                }
                addToDynamicArray(arrRowsName, &buffer);
            }
            //Иначе читаем и заполняем таблицу
            else {
                if (buffer[0]=='\0'){
                    printf("Error, readFileFormCSF()num3, \"\\0\"");
                    exit(EXIT_FAILURE); 
                }
                char *rowsName    = arrRowsName->array    + (numRows-1)    * arrRowsName->elementSize;
                char *columnsName = arrColumnsName->array + (numColumns-1) * arrColumnsName->elementSize;
                int lengthAddress = strlen(rowsName) + strlen(columnsName) + 1;
                char *address     = calloc(lengthAddress, SIZEOF_CHAR);
                if (address == NULL){
                    printf("Error, readFileFormCSF()num4, calloc == NULL");
                    exit(EXIT_FAILURE);
                }             
                strcpy(address, columnsName);
                strcat(address, rowsName);
                // Создание дерева
                if (binTreeRoot == NULL){
                    binTreeRoot = initBinTree(address, &(arrCSVTable->size));
                }
                else {
                    addToBinTree(binTreeRoot, initBinTree(address, &(arrCSVTable->size)));
                }
                // Если формула, то запоминаем ее индекс
                if (buffer[0] == '='){
                    addToDynamicArray(arrIdFormuls, &(arrCSVTable->size));
                }
                addToDynamicArray(arrCSVTable, initCSVFormat(buffer));
                free(address);
            }
            if (currentChar == ','){
                numColumns++;
            }
            else if (currentChar == '\n'){
                if (numColumns != maxColumns){
                    printf("Error, readFileFormCSF()num5, invalid table, numColumns(%d) != maxColumns(%d) in numRows(%d)", numColumns, maxColumns, numRows);
                    exit(EXIT_FAILURE);
                }
                numColumns = 0;
                numRows++;
            }
            bufferIndex = 0;
        } 
        else {
            if (numRows == 0 && numColumns == 0){
                printf("Error, readFileFormCSF()num6, invalid table, first character");
                exit(EXIT_FAILURE);
            }
            if (currentChar == ' '){
                printf("Error, readFileFormCSF()num7, invalid table, space");
                exit(EXIT_FAILURE);
            }          
            buffer[bufferIndex++] = currentChar;
        }   
    }
    buffer[bufferIndex] = '\0';
    if (buffer[0]=='\0' && numRows == 1){
        printf("Error, readFileFormCSF()№8, \"\\0\"");
        exit(EXIT_FAILURE); 
    }
    if (buffer[0] != '\0'){
        char *rowsName = arrRowsName->array + (numRows-1) * arrRowsName->elementSize;
        char *columnsName = arrColumnsName->array + (numColumns-1) *    arrColumnsName->elementSize;
        int lengthAddress = strlen(rowsName) + strlen(columnsName) + 1;
        char *address = calloc(lengthAddress, SIZEOF_CHAR);
        if (address == NULL){
            printf("Error, readFileFormCSF(), calloc == NULL");
            exit(EXIT_FAILURE);
        }    
        strcpy(address, columnsName);
        strcat(address, rowsName);
        addToBinTree(binTreeRoot, initBinTree(address, &(arrCSVTable->size)));
        if (buffer[0] == '='){
            addToDynamicArray(arrIdFormuls, &(arrCSVTable->size));
        }
        addToDynamicArray(arrCSVTable, initCSVFormat(buffer)); 
        free(address);   
    }
    arrAddressesId = returnDynamicArrayFromBinTree(binTreeRoot, arrAddressesId);
    fclose(file);
}
// Резделение формулы "=ARG1OPARG2" на три подстроки "ARG1" "OP" "ARG2"
void splitFormula(const char *formula, char **operand1, char **operator, char **operand2){
    size_t lengthFormula = strlen(formula);
    *operand1 = malloc(lengthFormula + 1);
    *operator = malloc(lengthFormula + 1);
    *operand2 = malloc(lengthFormula + 1);
    if (*operand1 == NULL || *operator == NULL || *operand2 == NULL){
        printf("Error, splitFormula, malloc == 0");
        exit(EXIT_FAILURE);
    }
    size_t operand1Index = 0, operatorIndex = 0, operand2Index = 0;
    int currentPart = 1;
    for (size_t i = 1; i < lengthFormula; ++i){
        char currentChar = formula[i];
        if ((currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') && currentPart == 1 && i != 1){
            currentPart = 2; 
        } 
        else if (!isalpha(currentChar) && !isdigit(currentChar) && currentPart == 1 && i != 1){
            currentPart = 2;
        } 
        else if ((isalpha(currentChar) || isdigit(currentChar) || currentChar == '-') && currentPart == 2){
            currentPart = 3;
        }
        switch (currentPart){
            case 1:
                (*operand1)[operand1Index++] = currentChar;
                break;
            case 2:
                (*operator)[operatorIndex++] = currentChar;
                break;
            case 3:
                (*operand2)[operand2Index++] = currentChar;
                break;
            default:
                break;
        }
    }
    (*operand1)[operand1Index] = '\0';
    (*operator)[operatorIndex] = '\0';
    (*operand2)[operand2Index] = '\0';
    if ((*operand1)[0] == '\0' || (*operator)[0] == '\0' || (*operand2)[0] == '\0'){
        printf("Error, splitFormula, didn 't split the formula");
        exit(EXIT_FAILURE);
    }
}
// Проверки корректности адреса
int isValidAddress(const char *address){
    if (*address == '\0'){
        return 0;
    }
    int hasDigit = 0;
    int hasAlpha = 0;
    if (isdigit(*address) || (*address) == '-'){
        hasDigit = 1;
    } 
    else if (isalpha(*address)){
        hasAlpha = 1;
    } 
    else {
        return 0;
    }
    while (*(++address) != '\0'){
        if (hasDigit && !hasAlpha && !isdigit(*address)){
            return 0;
        } 
        else if(hasAlpha && !hasDigit && isdigit(*address)){
            hasDigit = 1;
        }
        else if (!isdigit(*address) && !isalpha(*address)){
            return 0;
        }
    }
    return (hasDigit && !hasAlpha) || (hasAlpha && hasDigit);
}
// Поиск по формуле
long long resultValueFromCSVFormula(struct DynamicArray *arrCSVTable, size_t *id, struct DynamicArray *arrAddressesId){
    struct CSVFormat * currentCSV = (arrCSVTable->array) + (*id) * arrCSVTable->elementSize;
    if (currentCSV->formula==NULL){
        return currentCSV->value;
    }
    if(currentCSV->was_detect == 1){
        printf("Error, resultValueFromCSVFormula(), cycle in search of formulas");
        exit(EXIT_FAILURE);  
    }
    currentCSV->was_detect = 1;
    char *strOperand1, *strOperator, *strOperand2;
    splitFormula(currentCSV->formula, &strOperand1, &strOperator, &strOperand2);
    long long result = 0;
    long long intOperand1 = 0;
    long long intOperand2 = 0;
    short int isDigit = 1;
    if (!isValidAddress(strOperand1) || !isValidAddress(strOperand2)){
        printf("Error, resultValueFromCSVFormula(), invalid address");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < strlen(strOperand1); i++){
        if(strOperand1[i] == '-')
            continue;
        if(isdigit(strOperand1[i]) == 0){
            isDigit = 0;
            break;
        }
    }
    if (isDigit){
        intOperand1 = atoi(strOperand1);
    }
    else {
        struct CSVAddressId *CSVid = bsearch(initCSVAddressId(strOperand1, 0), arrAddressesId->array, arrAddressesId->size, arrAddressesId->elementSize, comparisonCSVAddressId);
        if (CSVid == NULL){
            printf("Error, resultValueFromCSVFormula(), invalid address");
            exit(EXIT_FAILURE);
        }
        intOperand1 = resultValueFromCSVFormula(arrCSVTable, &(CSVid->id), arrAddressesId);
    }
    isDigit = 1;
    for (size_t i = 0; i < strlen(strOperand2); i++){
        if(strOperand2[i] == '-')
            continue;
        if (isdigit(strOperand2[i]) == 0){
            isDigit = 0;
            break;
        }
    }
    if (isDigit){
        intOperand2 = atoi(strOperand2);
    }
    else {
        struct CSVAddressId* CSVid = bsearch(initCSVAddressId(strOperand2, 0), arrAddressesId->array, arrAddressesId->size, arrAddressesId->elementSize, comparisonCSVAddressId);
        if (CSVid == NULL){
            printf("Error, resultValueFromCSVFormula(), invalid address");
            exit(EXIT_FAILURE);
        }
        intOperand2 = resultValueFromCSVFormula(arrCSVTable, &(CSVid->id), arrAddressesId);
    }
    switch (*strOperator){
        case '+':
            result = intOperand1 + intOperand2;
            break;
        case '-':
            result = intOperand1 - intOperand2;
            break;
        case '*':
            result = intOperand1 * intOperand2;
            break;
        case '/':
            if (intOperand2 == 0){
                printf("Error, resultValueFromCSVFormula(), division by zero");
                exit(EXIT_FAILURE);  
            }
            result = intOperand1 / intOperand2;
            break;
        default:
            printf("Error, resultValueFromCSVFormula(), the arithmetic operation is not defined");
            exit(EXIT_FAILURE);  

    }
    free(((struct CSVFormat*)((char*)(arrCSVTable->array) + (*id) * arrCSVTable->elementSize))->formula);
    free(strOperand1);
    free(strOperator);
    free(strOperand2);
    currentCSV->formula = NULL;
    currentCSV->value = result;
    return result;
}
// Пробег по всем ячейкам с формулами
void calculatorCSV(struct DynamicArray *arrCSVTable, struct DynamicArray *arrAddressesId, struct DynamicArray *arrIdFormuls){
    for (size_t i = 0; i < arrIdFormuls->size; i++)
    {
        size_t *idFormula = (arrIdFormuls->array) + i * arrIdFormuls->elementSize; 
        long long result = resultValueFromCSVFormula(arrCSVTable, idFormula, arrAddressesId);
    }
    
}
// Вывод таблицы
void printCSVTable(const struct DynamicArray *arrCSVTable, const struct DynamicArray *arrColumnsName, const struct DynamicArray *arrRowsName){
    int nColomns    = arrColumnsName->size;
    int nRow        = arrRowsName->size;
    for (size_t i = 0; i < arrColumnsName->size; i++)
    {
        char *temp = (char*)(((char*)arrColumnsName->array) + i * arrColumnsName->elementSize);
        printf("%s,", temp);
    }
    printf("\n");
    int cntrow, cntcol;
    cntrow = cntcol = 0; 
    int j = 0;
    while(j < arrCSVTable->size){   
        if (cntcol == nColomns){
           cntcol = 0;
           cntrow++;
           printf("\n"); 
        }
        if (cntcol == 0){
            printf("%s",(arrRowsName->array + cntrow * arrRowsName->elementSize));
        }
        printf(",%d", (((struct CSVFormat*)(arrCSVTable->array + j++ * arrCSVTable->elementSize))->value));
        cntcol++;
    }
}
// Основной цикл работы с таблицей CSV
void processCSV(const char* pathCSV){
    //Инициализация динамических массивов
    struct DynamicArray arrColumnsName; //Имена колонок
    struct DynamicArray arrRowsName;     //Имена рядов
    struct DynamicArray arrCSVTable;    //Содержимое таблицы без названий колонок и рядов 
    struct DynamicArray arrAddressesId; //Все возможные адреса с индексом в таблице
    struct DynamicArray arrIdFormuls;   //Индекс всех ячеек таблицы, в которых есть формула

    //Создание размеров и выделение памяти для массивов
    initDynamicArray(&arrColumnsName, SIZE_CAPASITY_COL     , SIZEOF_CHAR);
    initDynamicArray(&arrRowsName   , SIZE_CAPASITY_ROW     , SIZEOF_CHAR); 
    initDynamicArray(&arrCSVTable   , SIZE_CAPASITY_TABLE   , SIZEOF_CSV_FORMAT);
    initDynamicArray(&arrAddressesId, SIZE_CAPASITY_ADDRESS , SIZEOF_CSV_ADDRESS_ID);
    initDynamicArray(&arrIdFormuls  , SIZE_CAPASITY_FORMULS , sizeof(int));

    //Чтение CSV файла и формирование структур для работы с CSV таблицей
    readFileFormCSF(pathCSV, &arrCSVTable, &arrColumnsName, &arrRowsName, &arrAddressesId, &arrIdFormuls);
    calculatorCSV(&arrCSVTable, &arrAddressesId, &arrIdFormuls);
    printCSVTable(&arrCSVTable, &arrColumnsName, &arrRowsName);

    for (size_t i = 0; i < arrCSVTable.size; i++){
    struct CSVFormat *temp = (struct CSVFormat*)((char*)arrCSVTable.array + i * arrCSVTable.elementSize);
    free(temp->formula);
    }
    freeDynamicArray(&arrCSVTable);
    for (size_t i = 0; i < arrAddressesId.size; i++){
    struct CSVAddressId *temp = arrAddressesId.array + i * arrAddressesId.elementSize;
    free(temp->address);
    }
    freeDynamicArray(&arrAddressesId);
    freeDynamicArray(&arrIdFormuls);
    freeDynamicArray(&arrColumnsName);
    freeDynamicArray(&arrRowsName);
}