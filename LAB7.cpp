#pragma once
#include <vector>
#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <limits>
#include <string>
#include <clocale>

namespace containers {

    /*Базовый интерфейс типизированного контейнера.
     *
     * @tparam T Тип хранимых элементов.
     */
    template <typename T>
    class IContainer {
    public:
        using size_type = std::size_t;

        virtual ~IContainer() = default;

        virtual size_type size() const noexcept = 0;
        virtual T& at(size_type index) = 0;
        virtual const T& at(size_type index) const = 0;
    };

    /*Статический массив фиксированной вместимости.
     *
     * @tparam T Тип хранимых элементов.
     * @tparam N Максимальная вместимость массива.
     */
    template <typename T, std::size_t N>
    class Array : public IContainer<T> {
    public:
        using size_type = std::size_t;

        Array() = default;

        /*Конструктор из списка инициализации.
        * 
         * @param init Список инициализации элементов.
         * @throws std::length_error Если размер списка больше N.
         */
        Array(std::initializer_list<T> init) {
            if (init.size() > N) {
                throw std::length_error("initializer_list длиннее N");
            }
            size_type i = 0;
            for (const auto& value : init) {
                data_[i++] = value;
            }
            size_ = i;
        }

        size_type size() const noexcept override { return size_; }

        /*Получить элемент по индексу.
        * 
         * @param index Индекс элемента.
         * @return Ссылка на элемент.
         * @throws std::out_of_range Если индекс вне диапазона.
         */
        T& at(size_type index) override {
            checkBounds(index);
            return data_[index];
        }

        /*Получить элемент по индексу (константная версия).
        * 
         * @param index Индекс элемента.
         * @return Константная ссылка на элемент.
         * @throws std::out_of_range Если индекс вне диапазона.
         */
        const T& at(size_type index) const override {
            checkBounds(index);
            return data_[index];
        }

        /*Вложенный итератор массива (random-access итератор).
         *
         * Позволяет проходить по элементам массива в диапазонном цикле for,
         * а также использовать алгоритмы STL (например, std::sort).
         */
        class Iterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            Iterator() = default;
            explicit Iterator(pointer ptr) : ptr_(ptr) {}

            reference operator*() const { return *ptr_; }
            pointer   operator->() const { return ptr_; }

            Iterator& operator++() { ++ptr_; return *this; }
            Iterator  operator++(int) { Iterator tmp = *this; ++ptr_; return tmp; }
            Iterator& operator--() { --ptr_; return *this; }
            Iterator  operator--(int) { Iterator tmp = *this; --ptr_; return tmp; }

            Iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
            Iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }

            Iterator operator+(difference_type n) const { return Iterator(ptr_ + n); }
            Iterator operator-(difference_type n) const { return Iterator(ptr_ - n); }

            difference_type operator-(const Iterator& other) const { return ptr_ - other.ptr_; }

            reference operator[](difference_type n) const { return *(ptr_ + n); }

            bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
            bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }
            bool operator< (const Iterator& other) const { return ptr_ < other.ptr_; }
            bool operator> (const Iterator& other) const { return ptr_ > other.ptr_; }
            bool operator<=(const Iterator& other) const { return ptr_ <= other.ptr_; }
            bool operator>=(const Iterator& other) const { return ptr_ >= other.ptr_; }

        private:
            pointer ptr_ = nullptr;
        };

        Iterator begin() { return Iterator(data_); }
        Iterator end() { return Iterator(data_ + size_); }
        Iterator begin() const { return Iterator(data_); }
        Iterator end() const { return Iterator(data_ + size_); }

    private:
        /*Проверить индекс на попадание в диапазон.
        * 
         * @param index Проверяемый индекс.
         * @throws std::out_of_range Если индекс вне диапазона.
         */
        void checkBounds(size_type index) const {
            if (index >= size_) {
                throw std::out_of_range("Индекс вне диапазона");
            }
        }

        T data_[N]{};
        size_type size_ = 0;
    };

    /*Коллекция контейнеров типа @c int.
     *
     * Хранит полиморфные контейнеры и позволяет объединять их элементы
     * в один вектор в порядке добавления.
     */
    class Collection {
    public:
        /*Добавить контейнер в коллекцию.
        * 
         * @param container Умный указатель на контейнер.
         */
        void add(std::shared_ptr<IContainer<int>> container) {
            containers_.push_back(std::move(container));
        }

        /*Объединить элементы всех контейнеров в один вектор.
        * 
         * @return Вектор, содержащий элементы всех контейнеров.
         */
        std::vector<int> flatten() const {
            std::vector<int> result;
            for (const auto& c : containers_) {
                for (std::size_t i = 0; i < c->size(); ++i) {
                    result.push_back(c->at(i));
                }
            }
            return result;
        }

        /*Получить количество контейнеров в коллекции.
        * 
         * @return Количество добавленных контейнеров.
         */
        std::size_t count() const noexcept { return containers_.size(); }

    private:
        std::vector<std::shared_ptr<IContainer<int>>> containers_;
    };

    /*CRTP-примесь для добавления операции сортировки.
    * 
     * @tparam Derived Производный класс.
     */
    template <typename Derived>
    class Sortable {
    public:
        /*Отсортировать элементы производного класса.
         *
         * Делегирует сортировку методу @c sort_impl() производного класса.
         */
        void sort() {
            static_cast<Derived*>(this)->sort_impl();
        }
    };

    /*Сортируемый массив фиксированной вместимости.
     *
     * @tparam T Тип хранимых элементов.
     * @tparam N Максимальная вместимость массива.
     */
    template <typename T, std::size_t N>
    class SortableArray
        : public Array<T, N>,
        public Sortable<SortableArray<T, N>> {
    public:
        using Array<T, N>::Array;

    private:
        friend Sortable<SortableArray<T, N>>;

        /*Реализация сортировки элементов массива по возрастанию. */
        void sort_impl() {
            std::sort(this->begin(), this->end());
        }
    };

}

int main() {
    setlocale(LC_ALL, "Ru");

    using namespace containers;

    Array<int, 10> array{ 3, 1, 4, 1, 5, 9, 2, 6 };
    SortableArray<int, 8> sortable{ 5, 2, 8, 1, 9, 3, 7, 4 };
    Collection collection;

    int choice = -1;
    while (choice != 0) {
        std::cout << "Меню\n"
            << "1 - показать Array\n"
            << "2 - получить элемент по индексу\n"
            << "3 - изменить элемент по индексу\n"
            << "4 - добавить копию Array в Collection\n"
            << "5 - показать flatten()\n"
            << "6 - показать SortableArray (до/после sort)\n"
            << "0 - выход\n"
            << "Выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Нужно ввести число.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            std::cout << "size = " << array.size() << ", элементы: ";
            for (int v : array) std::cout << v << ' ';
            std::cout << '\n';
            break;
        }
        case 2: {
            std::cout << "Индекс: ";
            std::size_t idx{};
            if (!(std::cin >> idx)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            try {
                std::cout << "at(" << idx << ") = " << array.at(idx) << '\n';
            }
            catch (const std::out_of_range& e) {
                std::cout << "Ошибка: " << e.what() << '\n';
            }
            break;
        }
        case 3: {
            std::cout << "Индекс и новое значение: ";
            std::size_t idx{}; int value{};
            if (!(std::cin >> idx >> value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            try {
                array.at(idx) = value;
                std::cout << "Готово.\n";
            }
            catch (const std::out_of_range& e) {
                std::cout << "Ошибка: " << e.what() << '\n';
            }
            break;
        }
        case 4: {
            auto copy = std::make_shared<Array<int, 10>>(array);
            collection.add(copy);
            std::cout << "Добавлено. Контейнеров в Collection: "
                << collection.count() << '\n';
            break;
        }
        case 5: {
            auto all = collection.flatten();
            std::cout << "flatten (" << all.size() << " элементов): ";
            for (int v : all) std::cout << v << ' ';
            std::cout << '\n';
            break;
        }
        case 6: {
            std::cout << "До sort:    ";
            for (int v : sortable) std::cout << v << ' ';
            sortable.sort();
            std::cout << "\nПосле sort: ";
            for (int v : sortable) std::cout << v << ' ';
            std::cout << '\n';
            break;
        }
        case 0:
            std::cout << "Выход.\n";
            break;
        default:
            std::cout << "Нет такого пункта.\n";
            break;
        }
    }
    return 0;
}