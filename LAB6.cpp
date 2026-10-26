#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <limits>
#include <clocale>

/*Абстрактный класс банковского счета
*
*@param balance  начальный баланс счета
*@throws std::logic_error если баланс отрицательный
*/
class Account {
public:
    explicit Account(double balance) : balance_{ balance } {
        if (balance < 0)
            throw std::logic_error("Баланс отрицательный");
    }

    virtual ~Account() = default;

    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual void printBalance() const = 0;
    virtual void applyInterest() {}

    double getBalance() const { return balance_; }

protected:
    double balance_;
};

/*Сберегательный счет с процентной ставкой
*
*@param balance       начальный баланс счета
*@param interestRate  процентная ставка
*@throws std::logic_error если процентная ставка отрицательная
*/
class SavingsAccount : public Account {
public:
    SavingsAccount(double balance, double interestRate)
        : Account(balance), interestRate_{ interestRate } {
        if (interestRate < 0)
            throw std::logic_error("Процентная ставка отрицательная");
    }

    /*Пополнение сберегательного счета
    *
    *@param amount  сумма пополнения
    */
    void deposit(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма должна быть положительной\n";
            return;
        }
        balance_ += amount;
        std::cout << "Сберегательный счет пополнен на " << amount << std::endl;
    }

    /*Снятие средств со сберегательного счета
    *
    *@param amount  сумма снятия
    */
    void withdraw(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма должна быть положительной" << std::endl;
            return;
        }
        if (amount > balance_) {
            std::cout << "Недостаточно средств" << std::endl;
            return;
        }
        balance_ -= amount;
        std::cout << "Со сберегательного счета снято " << amount << std::endl;
    }

    /*Вывод баланса сберегательного счета*/
    void printBalance() const override {
        std::cout << "Баланс: " << balance_
            << " (ставка " << interestRate_ << "%)" << std::endl;
    }

    /*Начисление процентов на остаток*/
    void applyInterest() override {
        double interest = balance_ * interestRate_ / 100.0;
        balance_ += interest;
        std::cout << "Начислены проценты: " << interest
            << " (" << interestRate_ << "%)" << std::endl;
    }

private:
    double interestRate_;
};

/*Расчетный счет с возможностью овердрафта
*
*@param balance         начальный баланс счета
*@param overdraftLimit  лимит овердрафта
*@throws std::logic_error если лимит овердрафта отрицательный
*/
class CheckingAccount : public Account {
public:
    CheckingAccount(double balance, double overdraftLimit)
        : Account(balance), overdraftLimit_{ overdraftLimit } {
        if (overdraftLimit < 0)
            throw std::logic_error("Лимит овердрафта отрицательный");
    }

    /*Пополнение расчетного счета
    */
    void deposit(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма должна быть положительной" << std::endl;
            return;
        }
        balance_ += amount;
        std::cout << "Расчетный счет пополнен на " << amount << std::endl;
    }

    /*Снятие средств с расчетного счета с учетом овердрафта
    */
    void withdraw(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма должна быть положительной" << std::endl;
            return;
        }
        if (amount > balance_ + overdraftLimit_) {
            std::cout << "Превышен лимит овердрафта" << std::endl;
            return;
        }
        balance_ -= amount;
        std::cout << "С расчетного счета снято " << amount << std::endl;
    }

    /*Вывод баланса расчетного счета
    */
    void printBalance() const override {
        std::cout << "Баланс: " << balance_
            << " (овердрафт до " << overdraftLimit_ << ")" << std::endl;
    }

private:
    double overdraftLimit_;
};

/*Кредитный счет с кредитным лимитом
*
*@param balance      начальный баланс счета
*@param creditLimit  кредитный лимит
*@throws std::logic_error если кредитный лимит отрицательный
*/
class CreditAccount : public Account {
public:
    CreditAccount(double balance, double creditLimit)
        : Account(balance), creditLimit_{ creditLimit } {
        if (creditLimit < 0)
            throw std::logic_error("Кредитный лимит отрицательный");
    }

    /*Пополнение кредитного счета
    */
    void deposit(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма должна быть положительной\n";
            return;
        }
        balance_ += amount;
        std::cout << "Кредитный счет пополнен на " << amount << "\n";
    }

    /*Снятие средств с кредитного счета с учетом кредитного лимита
    */
    void withdraw(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма должна быть положительной" << std::endl;
            return;
        }
        if (balance_ - amount < -creditLimit_) {
            std::cout << "Превышен кредитный лимит" << std::endl;
            return;
        }
        balance_ -= amount;
        std::cout << "С кредитного счета снято " << amount << std::endl;
    }

    /*Вывод баланса кредитного счета
    */
    void printBalance() const override {
        std::cout << "Баланс: " << balance_
            << " (кредитный лимит " << creditLimit_ << ")" << std::endl;
    }

private:
    double creditLimit_;
};

/*Класс банка, управляющий коллекцией счетов
*/
class Bank {
public:
    /*Добавление счета в банк
    *
    *@param account  unique_ptr на счет
    */
    void addAccount(std::unique_ptr<Account> account) {
        if (account) {
            accounts_.push_back(std::move(account));
            std::cout << "Аккаунт создан\n";
        }
    }

    /*Начисление процентов по всем счетам, поддерживающим это
    */
    void applyInterest() {
        for (auto& acc : accounts_) {
            acc->applyInterest();
        }
    }

    /*Вывод балансов всех счетов с порядковыми номерами*/
    void showAllBalances() const {
        for (size_t i = 0; i < accounts_.size(); ++i) {
            std::cout << i + 1 << ". ";
            accounts_[i]->printBalance();
        }
    }

    /*Получение количества счетов в банке
    *
    *@return количество счетов
    */
    size_t getAccountsCount() const {
        return accounts_.size();
    }

    /*Получение счета по индексу
    *
    *@param index  индекс счета
    *@return указатель на счет
    *@throws std::out_of_range если индекс вне диапазона
    */
    Account* getAccount(size_t index) {
        return accounts_.at(index).get();
    }

private:
    std::vector<std::unique_ptr<Account>> accounts_;
};

static bool readInt(int& value) {
    if (std::cin >> value) return true;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Ошибка ввода. Введите целое число.\n";
    return false;
}

static bool readDouble(double& value) {
    if (std::cin >> value) return true;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Ошибка ввода. Введите число.\n";
    return false;
}

int main() {
    setlocale(LC_ALL, "Ru");

    Bank myBank;
    int choice = 0;

    while (true) {
        std::cout << "\n1. Показать баланс всех счетов" << std::endl;
        std::cout << "2. Открыть новый счет" << std::endl;
        std::cout << "3. Пополнить счет" << std::endl;
        std::cout << "4. Снять средства со счета" << std::endl;
        std::cout << "5. Начислить проценты (на сберегательные)" << std::endl;
        std::cout << "6. Выйти из программы" << std::endl;
        std::cout << "Выбор: ";
        if (!readInt(choice)) continue;

        if (choice == 1) {
            if (myBank.getAccountsCount() == 0) {
                std::cout << "В банке нет счетов\n";
                continue;
            }
            myBank.showAllBalances();
        }
        else if (choice == 2) {
            int type = 0;
            std::cout << "\nВыберите тип счета:\n1 - Сберегательный\n2 - Расчетный\n3 - Кредитный\nВыбор: ";
            if (!readInt(type)) continue;
            if (type < 1 || type > 3) {
                std::cout << "Неверный тип счета\n";
                continue;
            }

            double balance = 0;
            std::cout << "Введите начальный баланс: ";
            if (!readDouble(balance)) continue;

            try {
                if (type == 1) {
                    double rate = 0;
                    std::cout << "Введите процентную ставку: ";
                    if (!readDouble(rate)) continue;
                    myBank.addAccount(std::make_unique<SavingsAccount>(balance, rate));
                }
                else if (type == 2) {
                    double overdraft = 0;
                    std::cout << "Введите лимит овердрафта: ";
                    if (!readDouble(overdraft)) continue;
                    myBank.addAccount(std::make_unique<CheckingAccount>(balance, overdraft));
                }
                else {
                    double creditLimit = 0;
                    std::cout << "Введите кредитный лимит: ";
                    if (!readDouble(creditLimit)) continue;
                    myBank.addAccount(std::make_unique<CreditAccount>(balance, creditLimit));
                }
            }
            catch (const std::logic_error& e) {
                std::cout << "Ошибка создания счета: " << e.what() << std::endl;
            }
        }
        else if (choice == 3 || choice == 4) {
            if (myBank.getAccountsCount() == 0) {
                std::cout << "В банке еще нет счетов для проведения операций" << std::endl;
                continue;
            }

            myBank.showAllBalances();

            int accountIdx = 0;
            std::cout << "Выберите номер счета (1-" << myBank.getAccountsCount() << "): ";
            if (!readInt(accountIdx)) continue;

            if (accountIdx < 1 ||
                accountIdx > static_cast<int>(myBank.getAccountsCount())) {
                std::cout << "Неверный номер счета\n";
                continue;
            }

            Account* selectedAcc = myBank.getAccount(static_cast<size_t>(accountIdx - 1));

            double amount = 0;
            std::cout << "Введите сумму: ";
            if (!readDouble(amount)) continue;

            if (choice == 3) {
                selectedAcc->deposit(amount);
            }
            else {
                selectedAcc->withdraw(amount);
            }
        }
        else if (choice == 5) {
            myBank.applyInterest();
        }
        else if (choice == 6) {
            break;
        }
        else {
            std::cout << "Неверный пункт меню" << std::endl;
        }
    }
    return 0;
}
