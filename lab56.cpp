#include <string.h>
#include <stdlib.h>
#include <random>
#include <math.h>

#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <memory>
#include <future>

struct find_args_t
{
    bool** map;
    size_t height;
    size_t width;
    size_t startPosX;
    size_t startPosY;
};

void find(find_args_t* args, std::shared_future<bool>& f);
int stepDirection(int* stepNumber);
int step(bool** map, size_t height, size_t width, int* stepNumber, size_t* x, size_t* y, int* sum, std::mutex& m);

std::mutex m;
std::mt19937 rand_gen;

int main()
{
    size_t NUMBER_THREADS = 1000;
    std::vector<std::thread*> threads;
    threads.reserve(NUMBER_THREADS);
    size_t WIDTH = 500, HEIGHT = 500;
    bool** map = new bool* [HEIGHT];
    for (int i = 0; i < HEIGHT; ++i)
        map[i] = new bool[WIDTH];

    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            map[i][j] = rand_gen() % 2;
        }
    }
    std::promise<bool> p;
    std::shared_future<bool> f = p.get_future();
    for (int i = 0; i < NUMBER_THREADS; ++i)
    {
        find_args_t* args = new find_args_t;
        args->map = map;
        args->height = HEIGHT;
        args->width = WIDTH;
        std::thread* t = new std::thread(find, args, std::ref(f));
        threads.push_back(t);
    }
    p.set_value(true);
    for (int i = 0; i < NUMBER_THREADS; ++i)
    {
        threads[i]->join();
    }

    for (int i = 0; i < HEIGHT; ++i)
    {
        delete[] map[i];
    }

    return 0;
}
void find(find_args_t* args, std::shared_future<bool>& f)
{
    f.get();
    args->startPosX = rand_gen() % args->width;
    args->startPosY = rand_gen() % args->height;
    m.lock();
    std::cout << "Поток " << std::this_thread::get_id() << "\nВыбрана начальная точка (" << args->startPosX << "; " << args->startPosY << ")\n";
    m.unlock();
    int stepNumber = 0;
    int sum = 0;
    if (args->map[args->startPosY][args->startPosX])
    {
        ++sum;
    }
    size_t x = args->startPosX, y = args->startPosY;
    while (!step(args->map, args->height, args->width, &stepNumber, &x, &y, &sum, m))
    {
    }
    m.lock();
    std::cout << "Поток " << std::this_thread::get_id() << " найдено " << sum << " целей\n";
    m.unlock();

    delete args;
}
// Выбрать направление шага в зависимости от его номера
// 0 - вверх
// 1 - вниз
// 2 - влево
// 3 - вправо
int stepDirection(int* stepNumber)
{
    switch (*stepNumber % 4)
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
int step(bool** map, size_t height, size_t width, int* stepNumber, size_t* x, size_t* y, int* sum, std::mutex& m)
{
    int stepSize = ceil(((double)*stepNumber + 1.0) / 2.0);
    size_t stepDir = stepDirection(stepNumber);
    (*stepNumber)++;
    for (int i = 0; i < stepSize; ++i)
        switch (stepDir)
        {
        case 0:
            if ((*y + 1) < height)
            {
                *y = *y + 1;
                if (map[*y][*x])
                {
                    ++(*sum);
                }
            }
            else
                return -1;
            break;
        case 1:
            if ((int)(*y - 1) >= 0)
            {
                *y = *y - 1;
                if (map[*y][*x])
                {
                    ++(*sum);
                }
            }
            else
                return -1;
            break;
        case 2:
            if ((int)(*x - 1) >= 0)
            {
                *x = *x - 1;
                if (map[*y][*x])
                {
                    ++(*sum);
                }
            }
            else
                return -1;
            break;
        case 3:
            if ((*x + 1) < width)
            {
                *x = *x + 1;
                if (map[*y][*x])
                {
                    ++(*sum);
                }
            }
            else
                return -1;
            break;
        }
    return 0;
}
