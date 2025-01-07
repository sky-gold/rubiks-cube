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

## Скорость работы

Запуск на 10 позициях из файлов 1.txt, 2.txt, ... 20.txt

| Расстояние от решения  | Время (секунды)  | Память (килобайты) |
|------------------------|------------------|--------------------|
| 1                      | 0.01             | 3584               |
| 2                      | 0.01             | 3584               |
| 3                      | 0.01             | 3584               |
| 4                      | 0.01             | 3584               |
| 5                      | 0.01             | 3584               |
| 6                      | 0.03             | 3584               |
| 7                      | 0.25             | 3584               |
| 8                      | 1.1              | 3584               |
| 9                      | 6.77             | 3456               |
| 10                     | 86.93            | 3584               |
| 11                     | >15 мин          | ???                |