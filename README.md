Сбор прокта MinGW
gcc -o CSVReader CSVReader.c CSVfunction.c DynamicArr.c
или запуск Build Project.bat

Запуск программы
.\CSV_Reader.exe file.csv
или
.\CSV_Reader.exe <filename>

Структура
//основная функция
CSVReader.c 
//полная обработка CSV файла
CSVfunction.h
CSVfunction.c
//Динамический массив
DynamicArr.c
DynamicArr.h