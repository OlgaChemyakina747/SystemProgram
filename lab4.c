#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

typedef unsigned char bool;

int find(bool ** map, size_t height, size_t width, size_t startPosX, size_t startPosY);
int stepDirection(int* stepNumber);
int step(bool ** map, size_t height, size_t width, /* [in out] */ int* stepNumber, /* [out] */size_t * x, /* [out] */size_t * y, /* [out] */ int* sum);

int main()
{
    /* Установили кол-во процессов */
    size_t NUMBER_PROCESSES = 5;
    /* Сделали массив для процессов (их айдишников) */
    pid_t *processes = malloc(sizeof(pid_t));

    /* Установили размер карты */
    size_t WIDTH = 5, HEIGHT = 5;

    /* Выделили память под карту */
    bool ** map = malloc(HEIGHT * sizeof(bool *));
    for(int i = 0; i < HEIGHT; ++i)
        map[i] = malloc(WIDTH);

    /* Инициализировали рандом */
    srand(time(0));

    /* Заполнили карту случайными числами и напечатали её */
    for(int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            if(rand() < (RAND_MAX / 2))
                map[i][j] = 0;
            else
                map[i][j] = 1;
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }

    /* Начинаем запускать процессы */
    for(int i = 0; i < NUMBER_PROCESSES; ++i)
    {
        /* Создаём процесс. Созданный процесс начнётся со следующей строки, только у него process = 0
         * у текущего же процесса при переходе на след. строку process = id созданного процесса */
        pid_t process = fork();
        if(!process)    // Если это дочерний процесс
        {
            /* Ищем на карте */
            int sum = find(map, HEIGHT, WIDTH, rand() % WIDTH, rand() % HEIGHT);
            printf("Найдено %d целей\n", sum);
        }
        else    // Если главный
        {
            /* Закидываем созданный процесс в массив */
            processes[i] = process;
            break;
        }
    }

    /* Ждём завершения всех созданных процессов из массива */
    for(int i = 0; i < NUMBER_PROCESSES; ++i)
    {
        waitpid(processes[i], NULL, NULL);
    }

    return 0;
}

/* Функция поиска целей на карте */
int find(bool ** map, size_t height, size_t width, size_t startPosX, size_t startPosY)
{
    printf("Выбрана начальная точка (%d ; %d)\n", startPosX, startPosY);
    int stepNumber = 0;
    int sum = 0;
    /* Если сразу встали на цель */
    if(map[startPosY][startPosX])
        ++sum;
    size_t x = startPosX, y = startPosY;

    /* Пока не дошагали до края */
    while(!step(map, height, width, &stepNumber, &x, &y, &sum))
    {
        printf("%d %d\n", x, y);
    }

    return sum;
}

////////////////////////////////
// Выбрать направление шага в зависимости от его номера
// 0 - вверх
// 1 - вниз
// 2 - влево
// 3 - вправо
////////////////////////////////
int stepDirection(int* stepNumber)
{
    switch(*stepNumber % 4)
    {
        case 0:
            return 0;
        case 1:
            return 3;
        case 2:
            return 1;
        case 3:
            return 2;
    }
}

/* Выполнить шаг */
int step(bool ** map, size_t height, size_t width, /* [in out] */ int* stepNumber, /* [out] */size_t * x, /* [out] */size_t * y, /* [out] */ int* sum)
{
    /* Здесь по сути stepSize = (stepNumber + 1) / 2 и округлённое в большую */
    double temp = (*stepNumber + 1.0) / 2.0;
    int stepSize;
    if((temp - (int)temp) > 0.0)
        stepSize = (int) temp + 1;
    /* Получили направление шага */
    size_t stepDir = stepDirection(stepNumber);
    /* Сразу нарастили номер шага */
    (*stepNumber)++;

    /* В цикле выполняем маленьгие единичные шажки, чтобы проверить все клетки по пути */
    for(int i = 0; i < stepSize; ++i)
        /* Шагаем в зависимости от направления */
        switch(stepDir)
        {
            case 0:
                if((*y + 1) < height)   // Если не конец карты
                {
                    *y = *y + 1;    // наращиваем координаты в нужную сторону
                    if(map[*y][*x])
                        ++(*sum);   // Если нашли цель, наращиваем сумму целей
                }
                else                // Если всё-таки карта закончилась
                    return -1;
                break;

                /* Дальше аналогично предыдущему только в разных направлениях */
            case 1:
                if((int)(*y - 1) >= 0)
                {
                    *y = *y - 1;
                    if(map[*y][*x])
                        ++(*sum);
                }
                else
                    return -1;
                break;

            case 2:
                if((int)(*x - 1) >= 0)
                {
                    *x = *x - 1;
                    if(map[*y][*x])
                        ++(*sum);
                }
                else
                    return -1;
                break;

            case 3:
                if((*x + 1) < width)
                {
                    *x = *x + 1;
                    if(map[*y][*x])
                        ++(*sum);
                }
                else
                    return -1;
                break;
        }

    return 0;
}
