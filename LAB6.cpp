#include <iostream>
#include <stdexcept>
#include <vector>

/*Базовый класс банковского счета
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
    virtual void getBalance() const = 0;

    virtual void applyInterest() {}

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
    *@return ничего не возвращает
    */
    void deposit(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма отрицательная\n";
            return;
        }
        balance_ += amount;
        std::cout << "Сберегательный счет пополнен на " << amount << std::endl;
    }

    /*Снятие средств со сберегательного счета
    *
    *@param amount  сумма снятия
    *@return ничего не возвращает
    */
    void withdraw(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма снятия отрицательная" << std::endl;
            return;
        }
        if (amount > balance_) {
            std::cout << "Недостаточно средств" << std::endl;
            return;
        }
        balance_ -= amount;
        std::cout << "Со сберегательного счета снято " << amount << std::endl;
    }

    /*Вывод баланса сберегательного счета
    *
    *@return ничего не возвращает
    */
    void getBalance() const override {
        std::cout << "Баланс: " << balance_ << std::endl;
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
    *
    *@param amount  сумма пополнения
    *@return ничего не возвращает
    */
    void deposit(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма пополнения отрицательная" << std::endl;
            return;
        }
        balance_ += amount;
        std::cout << "Расчетный счет пополнен на " << amount << std::endl;
    }

    /*Снятие средств с расчетного счета с учетом овердрафта
    *
    *@param amount  сумма снятия
    *@return ничего не возвращает
    */
    void withdraw(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма снятия отрицательная" << std::endl;
            return;
        }
        if (amount > balance_ + overdraftLimit_) {
            std::cout << "Превышен лимит" << std::endl;
            return;
        }
        balance_ -= amount;
        std::cout << "С расчетного счета снято " << amount << " (включая овердрафт, если баланс < 0)" << std::endl;
    }

    /*Вывод баланса расчетного счета
    *
    *@return ничего не возвращает
    */
    void getBalance() const override {
        std::cout << "Баланс: " << balance_ << std::endl;
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
    *
    *@param amount  сумма пополнения
    *@return ничего не возвращает
    */
    void deposit(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма пополнения отрицательная\n";
            return;
        }
        balance_ += amount;
        std::cout << "Кредитный счет пополнен на " << amount << "\n";
    }

    /*Снятие средств с кредитного счета с учетом кредитного лимита
    *
    *@param amount  сумма снятия
    *@return ничего не возвращает
    */
    void withdraw(double amount) override {
        if (amount <= 0) {
            std::cout << "Сумма снятия отрицательная" << std::endl;
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
    *
    *@return ничего не возвращает
    */
    void getBalance() const override {
        std::cout << "Баланс: " << balance_ << std::endl;
    }

private:
    double creditLimit_;
};

/*Класс банка, управляющий коллекцией счетов
*
*@return ничего не возвращает
*/
class Bank {
public:
    void addAccount(Account* account) {
        if (account != nullptr) {
            accounts_.push_back(account);
            std::cout << "Аккаунт создан\n";
        }
    }

    /*Начисление процентов по всем счетам, поддерживающим это
    *
    *@return ничего не возвращает
    */
    void applyInterest() {
        for (Account* acc : accounts_) {
            acc->applyInterest();
        }
    }

    /*Вывод балансов всех счетов с порядковыми номерами
    *
    *@return ничего не возвращает
    */
    void showAllBalances() const {
        for (size_t i = 0; i < accounts_.size(); ++i) {
            std::cout << i + 1 << ". ";
            accounts_[i]->getBalance();
        }
    }

    /*Получение количества счетов в банке
    *
    *@return количество счетов
    */
    size_t getAccountsCount() const {
        return accounts_.size();
    }

    /*Получение счета по индексу.
    *
    *@param index  индекс счета (0-based).
    *@return указатель на счет.
    */
    Account* getAccount(size_t index) {
        return accounts_[index];
    }

    ~Bank() {
        for (Account* acc : accounts_) {
            delete acc;
        }
    }

private:
    std::vector<Account*> accounts_;
};

int main() {
    setlocale(LC_ALL, "Ru");

    Bank myBank;
    int choice = 0;

    while (true) {
        std::cout << "1. Показать баланс всех счетов" << std::endl;
        std::cout << "2. Открыть новый счет" << std::endl;
        std::cout << "3. Пополнить счет" << std::endl;
        std::cout << "4. Снять средства со счета" << std::endl;
        std::cout << "5. Начислить проценты (на сберегательные)" << std::endl;
        std::cout << "6. Выйти из программы" << std::endl;
        std::cout << "Выбор: ";
        std::cin >> choice;

        if (choice == 1) {
            myBank.showAllBalances();
        }
        else if (choice == 2) {
            int type = 0;
            std::cout << "\nВыберите тип счета:\n1 - Сберегательный\n2 - Расчетный\n3 - Кредитный\nВыбор: ";
            std::cin >> type;

            double balance = 0;
            std::cout << "Введите начальный баланс: ";
            std::cin >> balance;

            try {
                if (type == 1) {
                    double rate = 0;
                    std::cout << "Введите процентную ставку: ";
                    std::cin >> rate;
                    myBank.addAccount(new SavingsAccount(balance, rate));
                }
                else if (type == 2) {
                    double overdraft = 0;
                    std::cout << "Введите лимит овердрафта: ";
                    std::cin >> overdraft;
                    myBank.addAccount(new CheckingAccount(balance, overdraft));
                }
                else if (type == 3) {
                    double creditLimit = 0;
                    std::cout << "Введите кредитный лимит: ";
                    std::cin >> creditLimit;
                    myBank.addAccount(new CreditAccount(balance, creditLimit));
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
            size_t accountIdx = 0;
            std::cout << "Выберите номер счета (1-" << myBank.getAccountsCount() << "): ";
            std::cin >> accountIdx;

            Account* selectedAcc = myBank.getAccount(accountIdx - 1);
            double amount = 0;
            std::cout << "Введите сумму: ";
            std::cin >> amount;

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