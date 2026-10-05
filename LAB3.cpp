#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <limits>
#include <ctime>

/* Структура книги в библиотечной сестеме
*
* @var Book::isbn          ISBN для выдачи книги
* @var Book::title         Название книги
* @var Book::author        Автор книги
* @var Book::year          Год издания книги
* @var Book::is_available  Статус наличия книги
* @var Book::pages         Количество страниц в книге
*/
struct Book {
    std::string isbn;
    std::string title;
    std::string author;
    int year = 0;
    bool is_available = false;
    int pages = 0;
};

/* Генерация случайного ISBN в формате ХХХ-Х-ХХХ-ХХХХХ-Х
* 
* @return                 Cтрока, содержащая снегерированный ISBN
*/
std::string generateISBN() {
    std::string isbn;
    for (int i = 0; i < 3; i++) isbn += std::to_string(rand() % 10);
    isbn += "-";
    isbn += std::to_string(rand() % 10);
    isbn += "-";
    for (int i = 0; i < 3; i++) isbn += std::to_string(rand() % 10);
    isbn += "-";
    for (int i = 0; i < 5; i++) isbn += std::to_string(rand() % 10);
    isbn += "-";
    isbn += std::to_string(rand() % 10);
    return isbn;
}

/* Выводит информацию о книге
* 
* @param b                Книга для вывода
* @return                 Ничего не возвращает
*/
void printBook(const Book& b) {
    std::cout << b.isbn << " , " << b.title << " , " << b.author
        << " , " << b.year << " , "
        << (b.is_available ? "available" : "unavailable")
        << " , " << b.pages << " pages\n";
}

/* Поиск кники по подстроке в поле "автор"
* 
* @param arr              Вектор книг для поиска
* @param query            Подстрока для поиска в имени автора
* @return                 Ничего не возвращает
*/
void searchByAuthor(const std::vector<Book>& arr, const std::string& query) {
    bool found = false;
    for (const auto& b : arr) {
        if (b.author.find(query) != std::string::npos) {
            printBook(b);
            found = true;
        }
    }
    if (!found) std::cout << "No books found.\n";
}

/* Поиск книги по подстроке в поле "название"
* 
* @param arr              Вектор книг для поиска
* @param query            Подстрока для поиска в имени автора
* @return                 Ничего не возвращает
*/
void searchByTitle(const std::vector<Book>& arr, const std::string& query) {
    bool found = false;
    for (const auto& b : arr) {
        if (b.title.find(query) != std::string::npos) {
            printBook(b);
            found = true;
        }
    }
    if (!found) std::cout << "No books found.\n";
}

/* Фильтрация книг по диапазону годов издания
* 
* @param arr              Вектор книг для поиска
* @param y1               Первая граница диапазона
* @param y2               Вторая граница диапазона
* @return                 Вектор книг в диапазоне [y1, y2]
*/
std::vector<Book> filterByYear(const std::vector<Book>& arr, int y1, int y2) {
    if (y1 > y2) std::swap(y1, y2);
    std::vector<Book> result;
    for (const auto& b : arr) {
        if (b.year >= y1 && b.year <= y2) result.push_back(b);
    }
    return result;
}

/* Вывод статистики по всем книгам
* 
* @param arr              Вектор книг для поиска
* @return                 Ничего не возвращает
*/
void printStatistics(const std::vector<Book>& arr) {
    int total_pages = 0, available = 0, unavailable = 0;
    for (const auto& b : arr) {
        total_pages += b.pages;
        if (b.is_available) ++available;
        else ++unavailable;
    }
    std::cout << "Total books: " << arr.size() << "\n";
    std::cout << "Average pages: "
        << (arr.empty() ? 0.0 : static_cast<double>(total_pages) / arr.size()) << "\n";
    std::cout << "Available: " << available << "\n";
    std::cout << "Unavailable: " << unavailable << "\n";
}

/* Сортировка книг по году выпуска ( от новых к старым )
* 
* @param arr              Вектор книг для поиска
* @return                 Ничего не возвращает
*/
void sortBooks(std::vector<Book>& arr) {
    std::sort(arr.begin(), arr.end(), [](const Book& a, const Book& b) {
        if (a.year != b.year) return a.year > b.year;
        return a.author < b.author;
        });
}

/* Выдача кники по ISBN
* 
* @param arr              Вектор книг для поиска
* @param isbn             ISBN для выдачи книги
* @return true            Если книга успешно выдана, иначе false
*/
bool issueBook(std::vector<Book>& arr, const std::string& isbn) {
    for (auto& b : arr) {
        if (b.isbn == isbn) {
            if (!b.is_available) {
                std::cout << "Error: book is already issued.\n";
                return false;
            }
            b.is_available = false;
            std::cout << "Book \"" << b.title << "\" issued.\n";
            return true;
        }
    }
    std::cout << "Error: book with this ISBN not found.\n";
    return false;
}

/* Возврат книги в библиотеку по ISBN
* 
* @param arr              Вектор книг для поиска
* @param isbn             ISBN для выдачи книги
* @return true            Если книга успешно возвращена, иначе false
*/
bool returnBook(std::vector<Book>& arr, const std::string& isbn) {
    for (auto& b : arr) {
        if (b.isbn == isbn) {
            if (b.is_available) {
                std::cout << "Error: book is already available.\n";
                return false;
            }
            b.is_available = true;
            std::cout << "Book \"" << b.title << "\" returned.\n";
            return true;
        }
    }
    std::cout << "Error: book with this ISBN not found.\n";
    return false;
}

/* Очищение буферф и считывание строки из ввода 
* 
* @param out              Ссылка на строку, в которой будет записан результат
* @return                 Ничего не возвращает
*/
void readLine(std::string& out) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, out);
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    int n;
    std::cout << "Enter number N = ";
    while (!(std::cin >> n) || n <= 0) {
        std::cout << "Invalid input. Try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::vector<Book> arr(n);

    const std::vector<std::string> titles = {
        "War and Peace", "Crime and Punishment", "The Master and Margarita",
        "And Quiet Flows the Don", "Anna Karenina", "Eugene Onegin",
        "Dead Souls", "Fathers and Sons", "A Hero of Our Time",
        "Heart of a Dog", "1984", "Harry Potter and the Philosopher's Stone",
        "The Lord of the Rings", "Three Comrades", "The Catcher in the Rye",
        "To Kill a Mockingbird", "Pride and Prejudice", "Jane Eyre",
        "The Count of Monte Cristo", "The Three Musketeers"
    };

    const std::vector<std::string> authors = {
        "Leo Tolstoy", "Fyodor Dostoevsky", "Mikhail Bulgakov",
        "Mikhail Sholokhov", "Alexander Pushkin", "Nikolai Gogol",
        "Ivan Turgenev", "Mikhail Lermontov", "George Orwell",
        "J.K. Rowling", "J.R.R. Tolkien", "Erich Maria Remarque",
        "Jerome D. Salinger", "Harper Lee", "Jane Austen",
        "Charlotte Bronte", "Alexandre Dumas"
    };

    std::set<std::string> usedISBN;
    for (int i = 0; i < n; ++i) {
        arr[i].isbn = generateISBN();
        arr[i].title = titles[rand() % titles.size()];
        arr[i].author = authors[rand() % authors.size()];
        arr[i].year = 1900 + rand() % 125;
        arr[i].is_available = (rand() % 2 == 0);
        arr[i].pages = 50 + rand() % 950;
    }

    while (true) {
        std::cout << "\nMENU:\n"
            << "1. Search by author\n"
            << "2. Search by title\n"
            << "3. Search by year range\n"
            << "4. Statistics of all books\n"
            << "5. Sort (by year: new -> old)\n"
            << "6. Issue book by ISBN\n"
            << "7. Return book by ISBN\n"
            << "8. Show all books\n"
            << "9. Exit\n"
            << "Choice: ";

        int operation;
        std::cin >> operation;

        switch (operation) {
        case 1: {
            std::string query;
            std::cout << "Enter author: ";
            readLine(query);
            searchByAuthor(arr, query);
            break;
        }
        case 2: {
            std::string query;
            std::cout << "Enter title: ";
            readLine(query);
            searchByTitle(arr, query);
            break;
        }
        case 3: {
            int y1, y2;
            std::cout << "Enter year range (two years): ";
            std::cin >> y1 >> y2;
            auto filtered = filterByYear(arr, y1, y2);
            if (filtered.empty()) std::cout << "No books found.\n";
            else for (const auto& b : filtered) printBook(b);
            break;
        }
        case 4:
            printStatistics(arr);
            break;
        case 5:
            sortBooks(arr);
            for (const auto& b : arr) printBook(b);
            break;
        case 6: {
            std::string isbn;
            std::cout << "Enter ISBN to issue: ";
            readLine(isbn);
            issueBook(arr, isbn);
            break;
        }
        case 7: {
            std::string isbn;
            std::cout << "Enter ISBN to return: ";
            readLine(isbn);
            returnBook(arr, isbn);
            break;
        }
        case 8:
            for (const auto& b : arr) printBook(b);
            break;
        case 9:
            std::cout << "Goodbye.\n";
            return 0;
        default:
            std::cout << "Unknown operation.\n";
        }
    }
}