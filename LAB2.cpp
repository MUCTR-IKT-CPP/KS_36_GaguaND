#include <iostream>
#include <clocale>
#include <cstdlib>
#include <ctime>

/*Генерация массива размера N*N случайных чисел в диапазоне [0; 9]
* 
*@param arr  исходный двумерный массив.
*@param n    количество элементов массива.
*return      ничего не возвращает.
*/
void numberGeneration(int** arr, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            arr[i][j] = std::rand() % 10;
        }
    }
}

/*Зеркальное отражение матрицы относительно вертикальной оси
* 
*@param arr  исходный двумерный массив.
*@param n    размерность матрицы.
*@return     ничего не возвращает
*/
void mirrorVertical(int** arr, int n) {
    for ( int i = 0; i < n; ++i) {
        for ( int j = 0; j < n / 2; ++j) {
            std::swap(arr[i][j], arr[i][n - 1 - j]);
        }
    }
}

/*Зеркальное отражение матрицы относительно горизонтальной оси
*
*@param arr  исходный двумерный массив.
*@param n    размерность матрицы.
*@return     ничего не возвращает
*/
void mirrorHorizontal(int** arr, int n) {
    for (int i = 0; i < n / 2; ++i) {
        std::swap(arr[i], arr[n - 1 - i]);
    }
}

/*Построить матрицу, где каждый элемент равен сумме соседних элементов
*
*@param arr  исходный двумерный массив.
*@param n    размерность матрицы. 
*@return     ничего не возвращает.
*/
void sumNeighbors(int** arr, int n) {
    int** a = new int* [n];
    for (int i = 0; i < n; ++i) {
        a[i] = new int[n];
    }

    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0,  0, -1, 1 };

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int sum = 0;

            for (int k = 0; k < 4; ++k) {
                int ni = i + dx[k];
                int nj = j + dy[k];

                if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                    sum += arr[ni][nj];
                }
            }
            a[i][j] = sum;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            arr[i][j] = a[i][j];
        }
        delete[] a[i];
    }
    delete[] a;
}

/*Передача массива в функцию через параметр void*
*
*@param ptr  указатель на двумерный массив, переданный как void*.
*@param n    размерность матрицы.
*@return     ничего не возвращает.
*/
void processVoidPointer(void* ptr, int n) {
    int** arr = static_cast<int**>(ptr);
    mirrorVertical(arr, n);
}

/*Вывод матрицы в консоль
*
*@param arr  двумерный массив.
*@param n    размерность матрицы.
*@return     ничего не возвращает.
*/
void printMatrix(int** arr, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            std::cout << arr[i][j] << ' ';
        std::cout << std::endl;
    }
}

int main()
{
    setlocale(LC_ALL, "RU");

    int n;
    std::cout << "Введите N = "; 
    std::cin >> n;

    int** arr = new int* [n];

    for (int i = 0; i < n; ++i) {
        arr[i] = new int[n];
    }

    std::srand(std::time(nullptr));
    numberGeneration(arr, n);

    std::cout << "Первоначальная матрица" << std::endl;
    printMatrix(arr, n);
    std::cout << std::endl;

    int operation;
    std::cout << "Операция:\n"
        << "1. Создать зеркальное отражение относительно вертикальной оси\n"
        << "2. Создать зеркальное отражение относительно горизонтальной оси\n"
        << "3. Построить матрицу, где каждый элемент равен сумме соседних элементов\n"
        << "4. Передать массив в функцию через параметр void*\n";
    std::cin >> operation;

    switch (operation) {
    case 1:
        mirrorVertical(arr, n);
        std::cout << "Результат: " << std::endl;
        printMatrix(arr, n);
        break;

    case 2:
        mirrorHorizontal(arr, n);
        std::cout << "Результат: " << std::endl;
        printMatrix(arr, n);
        break;

    case 3:
        sumNeighbors(arr, n);
        std::cout << "Результат: " << std::endl;
        printMatrix(arr, n);
        break;

    case 4:
        processVoidPointer(arr, n);
        std::cout << "Результат: " << std::endl;
        printMatrix(arr, n);
        break;
    }


    for (int i = 0; i < n; ++i) {
        delete[] arr[i];
    }

    delete[] arr;
}