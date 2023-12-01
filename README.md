# Тестовое задание Yadro

## Сборка прокта
- gcc -o CSVReader CSVReader.c CSVfunction.c DynamicArr.c -O3
- Запуск сборки проекта на Linux BuildProjectLinuxGCC
- Запуск сборки проекта на Windows BuildProjectWindowsGCC.bat

## Запуск программы

- .\CSV_Reader.exe file.csv
- .\CSV_Reader.exe filename

## Данные проект представляет собой решение задачи парсинга файла CSV формата
## Порядок работы программы:
1. Чтение и формирование таблицы
   - Чтение файла CSV формата
   - Проверка на корректность таблицы
   - Запись названий столбцов и рядов в массив
   - Запись адресов ячеек в бинарное дерево
   - Запись ячеек в массив
   - Запись индексов ячеек в массив, где присутсвует формула
   - Формирование отсортированного массив адресов ячеек из бинарного дерева
1. Калькулятор
   - Проход по массив с ячейками, где есть формула
   - Рекурсиный проход по адресам, указанных в формуле(бинарный поиск для поиска адресов)
3. Вывод полученной таблицы

# Структура
+ основная функция
  CSVReader.c 
+ полная обработка CSV файла
  CSVfunction.h
  CSVfunction.c
+ Динамический массив
  DynamicArr.c
  DynamicArr.h
