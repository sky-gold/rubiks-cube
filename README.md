# rubiks-cube
C++ Solver for Rubik's cube

## Quick start

```bash
make build
python3 download_pdb.py # Скачать PDB
./build/src/rubiks_cube_solver \
    [--threads_count <число_потоков>] \
    [--pdb_size <low|mid|high>] \ # 44.1MB, 5.2GB, 13GB
    < "static/5.txt"
```

## Предпросчет 

Скачать pattern databases
```
python3 download_pdb.py
```
Если не их не скачавать то они сами посчитаются при первом запуске и запишутся в файлы, однако это займет много времени.

Предподсчёт всех решений для углов занимет 13-14 минут

Предпросчет решений для боковых частей (7 слева и 7 справа) занимает ~45 минут

Предпросчет решений для позиций боковых частей (без учета ориентации) занимает ~40 минут

## Команды

Скомпилировать проект
```
make build
```
Файл программы появится по пути **build/src/rubiks_cube_solver**

Запустить тесты
```
make test
```

Запустить benchmark на подготовленных файлах
```
python3 benchmark.py
```

## Пример запуска

```
./build/src/rubiks_cube_solver \
    [--threads_count <число_потоков>] \
    [--pdb_size <low|mid|high>] \ # 44.1MB, 5.2GB, 13GB
    < "static/5.txt ""
```


## Скорость работы

| Файл с позициями        | Количество позиций | Общее время (секунды) | Среднее время на позицию |
|-------------------------|--------------------|-----------------------|--------------------------|
| static/random5.txt      |         5          |               47508.4 |             2h 38m 21.7s |
| static/1.txt            |         10         |                   8.8 |                     0.9s |
| static/2.txt            |         10         |                   9.4 |                     0.9s |
| static/3.txt            |         10         |                  12.1 |                     1.2s |
| static/4.txt            |         10         |                  10.6 |                     1.1s |
| static/5.txt            |         10         |                  10.6 |                     1.1s |
| static/6.txt            |         10         |                  10.9 |                     1.1s |
| static/7.txt            |         10         |                  10.3 |                     1.0s |
| static/8.txt            |         10         |                   9.5 |                     0.9s |
| static/9.txt            |         10         |                  13.6 |                     1.4s |
| static/10.txt           |         10         |                  10.8 |                     1.1s |
| static/11.txt           |         10         |                  10.4 |                     1.0s |
| static/12.txt           |         10         |                  15.3 |                     1.5s |
| static/13.txt           |         10         |                  10.7 |                     1.1s |
| static/14.txt           |         10         |                  12.8 |                     1.3s |
| static/15.txt           |         10         |                  27.1 |                     2.7s |
| static/16.txt           |         10         |                 316.4 |                    31.6s |
| static/17.txt           |         10         |                5482.1 |                  9m 8.2s |
