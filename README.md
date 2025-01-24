# rubiks-cube
C++ Solver for Rubik's cube

## Команды для запуска

Скомпилировать проект
```
make build
```
Файл программы появится по пути **build/src/rubiks_cube_solver**

Запустить тесты
```
make test
```

## Пример запуска

```
build/src/rubiks_cube_solver < static/5.txt 
```

## Предпросчет 

Предподсчёт всех решений для углов занимет 13-14 минут

## Скорость работы

Запуск на 10 позициях из файлов 1.txt, 2.txt, ... 20.txt

| Расстояние от решения  | Время (секунды)  | Память (килобайты) |
|------------------------|------------------|--------------------|
| 1                      | 0.01             | 89472              |
| 2                      | 0.02             | 89344              |
| 3                      | 0.01             | 89472              |
| 4                      | 0.02             | 89472              |
| 5                      | 0.01             | 89472              |
| 6                      | 0.02             | 89472              |
| 7                      | 0.04             | 89472              |
| 8                      | 0.1              | 89472              |
| 9                      | 0.23             | 89472              |
| 10                     | 0.87             | 89472              |
| 11                     | 4.93             | 89472              |
| 12                     | 19.75            | 89472              |
| 13                     | 228.36           | 89472              |