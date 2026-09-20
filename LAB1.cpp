#include <iostream>
#include <ctime>
#include <clocale>
#include <cstdlib>

/*Генерация массива случайных целых чисел в диапазоне [1, 1000]
*
*@param arr     исходный массив.
*@param size    количество элементов массива.
*@return        ничего не возвращает.
*/
void generateArray(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000 + 1;
    }
}

/*Подсчёт количества чётных и нечётных чисел в массиве
*
*@param arr           исходный массив.
*@param size          количество элементов массива.
*@param evenNumbers   ссылка на запись чётных чисел.
*@param oddNumbers    ссылка на запись нечётных чисел.
*@return              ничего не возвращает.
*/
void countEvenOdd(const int* arr, int size, int& evenNumbers, int& oddNumbers) {
    evenNumbers = 0;
    oddNumbers = 0;

    for (int i = 0; i < size; ++i) {
        if (arr[i] % 2 == 0) {
            evenNumbers++;
        }
        else {
            oddNumbers++;
        }
    }
}

/*Подсчёт суммы чисел, кратных 3 и кратных 5
*
*@param arr    исходный массив.
*@param size   количество элементов массива.
*@param sum3   ссылка для записи суммы чисел, кратных 3.
*@param sum5   ссылка для записи суммы чисел, кратных 5.
*@return       ничего не возвращает.
*/
void sum3and5(const int* arr, int size, int& sum3, int& sum5) {
    sum3 = 0;
    sum5 = 0;

    for (int i = 0; i < size; ++i) {
        if (arr[i] % 3 == 0) {
            sum3 += arr[i];
        }
        if (arr[i] % 5 == 0) {
            sum5 += arr[i];
        }
    }
}

/*Вывод результатов в виде таблицы
*
*@param evenNumbers   количество чётных чисел.
*@param oddNumbers    количество нечётных чисел.
*@param sum3          сумма чисел, кратных 3.
*@param sum5          сумма чисел, кратных 5.
*@return              ничего не выводит.
*/
void printResults(int evenNumbers, int oddNumbers, int sum3, int sum5) {
    std::cout << std::endl;
    std::cout << "| Показатель          | Значение      " << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "| Чётные              | " << evenNumbers << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "| Нечётные            | " << oddNumbers << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "| Сумма кратных 3     | " << sum3 << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "| Сумма кратных 5     | " << sum5 << std::endl;
    std::cout << "--------------------------------------";
}

int main()
{
    setlocale(LC_ALL, "RU");

    int n;
    std::cout << "Ввод: N = ";
    std::cin >> n;

    srand(static_cast<unsigned int>(time(nullptr)));

    int arr[1000];

    generateArray(arr, n);

    int evenNumbers = 0, oddNumbers = 0;
    countEvenOdd(arr, n, evenNumbers, oddNumbers);

    int sum3 = 0, sum5 = 0;
    sum3and5(arr, n, sum3, sum5);

    printResults(evenNumbers, oddNumbers, sum3, sum5);

    return 0;
}