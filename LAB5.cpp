#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <map>
#include <set>
#include <iomanip>
#include <numeric>

/**
 * Базовый класс учебного курса
 * Хранит название, длительность, сложность, тип и статус выполнения,
 * а также задает общий полиморфный интерфейс для всех видов курсов
 */
class TrainingCourse {
public:
    /**
     * Создает учебный курс
     * @param name название курса
     * @param duration длительность в часах, должна быть больше 0
     * @param complexity сложность от 1 до 5
     * @param type тип курса (Programming, Mathematics, Physics)
     * @throws std::logic_error если длительность или сложность некорректны
     */
    TrainingCourse(std::string name, int duration, int complexity, std::string type)
        : name_{ std::move(name) },
        duration_{ duration },
        complexity_{ complexity },
        type_{ std::move(type) }
    {
        if (duration_ <= 0) {
            throw std::logic_error{ "Invalid input" };
        }
        if (complexity_ < 1 || complexity_ > 5) {
            throw std::logic_error{ "Invalid input" };
        }
    }

    /**
     * Виртуальный деструктор
     * Позволяет корректно удалять наследников через указатель на базовый класс
     */
    virtual ~TrainingCourse() = default;

    /**
     * Возвращает название курса
     * @return название курса
     */
    const std::string& name() const noexcept { return name_; }

    /**
     * Возвращает длительность курса
     * @return длительность в часах
     */
    int duration() const noexcept { return duration_; }

    /**
     * Возвращает сложность курса
     * @return сложность от 1 до 5
     */
    int complexity() const noexcept { return complexity_; }

    /**
     * Возвращает тип курса
     * @return тип курса
     */
    const std::string& type() const noexcept { return type_; }

    /**
     * Устанавливает новую длительность курса
     * @param d новая длительность в часах
     * @throws std::invalid_argument если значение не положительное
     */
    void setDuration(int d) {
        if (d <= 0) throw std::invalid_argument{ "Invalid input" };
        duration_ = d;
    }

    /**
     * Устанавливает новую сложность курса
     * @param c новая сложность от 1 до 5
     * @throws std::invalid_argument если значение вне диапазона 1..5
     */
    void setComplexity(int c) {
        if (c < 1 || c > 5) throw std::invalid_argument{ "Invalid input" };
        complexity_ = c;
    }

    /**
     * Проверяет, выполнен ли курс
     * @return true, если курс выполнен
     */
    bool isDone() const noexcept { return done_; }

    /**
     * Устанавливает статус выполнения курса
     * @param value новый статус выполнения
     */
    void setDone(bool value) noexcept { done_ = value; }

    /**
     * Возвращает текстовый статус курса
     * @return "completed" или "not completed"
     */
    std::string status() const { return done_ ? "completed" : "not completed"; }

    /**
     * Полиморфный метод изучения курса
     * Выводит информацию о процессе изучения
     */
    virtual void study() const {
        std::cout << "Studying course \"" << name_ << "\" ("
            << duration_ << " h, complexity " << complexity_ << ")\n";
    }

    /**
     * Полиморфный метод проверки знаний
     * Выводит информацию о проверке знаний по курсу
     */
    virtual void checkKnowledge() const {
        std::cout << "Knowledge check: " << name_ << "\n";
    }

    /**
     * Полиморфный метод, описывающий методику преподавания
     */
    virtual void teachingMethod() const {
        std::cout << "Standard methodology\n";
    }

    /**
     * Полиморфный метод, описывающий способ изучения
     * Переопределяется наследниками
     */
    virtual void methodOfStudy() const {
        std::cout << "Standard study method\n";
    }

    /**
     * Переименовывает курс и возвращает ссылку на себя
     * Позволяет строить цепочки вызовов
     * @param newName новое название курса
     * @return ссылка на текущий объект
     */
    TrainingCourse& rename(std::string newName) {
        name_ = std::move(newName);
        return *this;
    }

    /**
     * Дружественный оператор вывода курса в поток
     * @param output поток вывода
     * @param course выводимый курс
     * @return поток вывода
     */
    friend std::ostream& operator<<(std::ostream& output, const TrainingCourse& course);

private:
    std::string name_;
    int duration_;
    int complexity_;
    std::string type_;
    bool done_ = false;
};

/**
 * Оператор вывода курса в поток
 * @param output поток вывода
 * @param course выводимый курс
 * @return поток вывода
 */
std::ostream& operator<<(std::ostream& output, const TrainingCourse& course) {
    output << course.name_ << " [" << course.type_ << ", "
        << course.duration_ << " h, complexity " << course.complexity_ << "]";
    return output;
}

/**
 * Курс по программированию
 * Хранит язык программирования и переопределяет методы изучения
 */
class Programming : public TrainingCourse {
public:
    /**
     * Создает курс по программированию
     * @param name название курса
     * @param duration длительность в часах
     * @param complexity сложность от 1 до 5
     * @param language изучаемый язык программирования
     */
    Programming(std::string name, int duration, int complexity, std::string language)
        : TrainingCourse{ std::move(name), duration, complexity, "Programming" },
        language_{ std::move(language) } {}

    /**
     * Возвращает изучаемый язык программирования
     * @return язык программирования
     */
    const std::string& language() const noexcept { return language_; }

    /**
     * Выводит информацию об изучении языка программирования
     */
    void study() const override {
        std::cout << language_ << " - programming language to study" << std::endl;
    }

    /**
     * Выводит способ изучения — основы синтаксиса языка
     */
    void methodOfStudy() const override {
        std::cout << "Syntax basics of " << language_ << std::endl;
    }

    /**
     * Выводит методику преподавания — проектный подход
     */
    void teachingMethod() const override {
        std::cout << "Project-oriented approach" << std::endl;
    }

private:
    std::string language_;
};

/**
 * Курс по математике
 * Хранит раздел математики и переопределяет методы изучения
 */
class Mathematics : public TrainingCourse {
public:
    /**
     * Создает курс по математике
     * @param name название курса
     * @param duration длительность в часах
     * @param complexity сложность от 1 до 5
     * @param chapter изучаемый раздел
     */
    Mathematics(std::string name, int duration, int complexity, std::string chapter)
        :TrainingCourse{ std::move(name), duration, complexity, "Mathematics" },
        chapter_{ std::move(chapter) } {}

    /**
     * Возвращает изучаемый раздел математики
     * @return раздел математики
     */
    const std::string& chapter() const noexcept { return chapter_; }

    /**
     * Выводит информацию об изучаемом разделе
     */
    void study() const override {
        std::cout << chapter_ << " - topic to study" << std::endl;
    }

    /**
     * Выводит способ изучения — визуализацию
     */
    void methodOfStudy() const override {
        std::cout << "Visualization - study method" << std::endl;
    }

    /**
     * Выводит методику преподавания — математическое моделирование
     */
    void teachingMethod() const override {
        std::cout << "Mathematical modeling method" << std::endl;
    }

private:
    std::string chapter_;
};

/**
 * Курс по физике
 * Хранит количество лабораторных работ и переопределяет методы изучения
 */
class Physics : public TrainingCourse {
public:
    /**
     * Создает курс по физике
     * @param name название курса
     * @param duration длительность в часах
     * @param complexity сложность от 1 до 5
     * @param lab количество лабораторных работ, не меньше 0
     * @throws std::logic_error если количество лабораторных работ отрицательно
     */
    Physics(std::string name, int duration, int complexity, int lab)
        :TrainingCourse{ std::move(name), duration, complexity, "Physics" },
        lab_{ lab }
    {
        if (lab < 0) {
            throw std::logic_error{ "Invalid input" };
        }
    }

    /**
     * Возвращает количество лабораторных работ
     * @return количество лабораторных работ
     */
    int lab() const noexcept { return lab_; }

    /**
     * Выводит количество лабораторных работ
     */
    void study() const override {
        std::cout << lab_ << " - number of lab works" << std::endl;
    }

    /**
     * Выводит способ изучения — наблюдение
     */
    void methodOfStudy() const override {
        std::cout << "Observation - study method" << std::endl;
    }

    /**
     * Выводит методику преподавания — объяснительно-иллюстративную
     */
    void teachingMethod() const override {
        std::cout << "Explanatory-illustrative - teaching methodology" << std::endl;
    }

private:
    int lab_;
};

/**
 * Преподаватель
 * Хранит имя, базовый рейтинг, набор типов курсов, которые ведет,
 * и статистику за семестр
 */
struct Teacher {
    std::string name;
    double rating;
    std::set<std::string> subjects;
    int lessonsHeld = 0;
    double ratingSum = 0.0;
    int ratingCount = 0;

    /**
     * Возвращает средний фактический рейтинг преподавателя
     * Если занятий не было, возвращает базовый рейтинг
     * @return средний рейтинг
     */
    double avgRating() const {
        return ratingCount ? ratingSum / ratingCount : rating;
    }

    /**
     * Проверяет, может ли преподаватель вести курс заданного типа
     * @param type тип курса
     * @return true, если преподаватель ведет такой тип курсов
     */
    bool canTeach(const std::string& type) const {
        return subjects.count(type) > 0;
    }
};

/**
 * Учебная группа
 * Хранит название и количество студентов
 */
struct Group {
    std::string name;
    int size;
};

/**
 * Занятие
 * Связывает курс и преподавателя, который его ведет
 */
struct Lesson {
    std::shared_ptr<TrainingCourse> course;
    std::shared_ptr<Teacher> teacher;
};

/**
 * Учебный день
 * Хранит название дня и список занятий
 */
struct Day {
    std::string name_day;
    std::vector<Lesson> lessons;
};

/**
 * Расписание на неделю
 * Хранит номер недели и список дней
 */
struct WeekSchedule {
    int weekNumber;
    std::vector<Day> days;
};

std::mt19937 rng{ std::random_device{}() };

/**
 * Генерирует случайное целое число в заданном диапазоне
 * @param lo нижняя граница включительно
 * @param hi верхняя граница включительно
 * @return случайное целое число
 */
int random_number(int lo, int hi) {
    std::uniform_int_distribution<int> d(lo, hi);
    return d(rng);
}

/**
 * Генерирует случайное вещественное число в заданном диапазоне
 * @param lo нижняя граница
 * @param hi верхняя граница
 * @return случайное вещественное число
 */
double random_double(double lo, double hi) {
    std::uniform_real_distribution<double> d(lo, hi);
    return d(rng);
}

static const std::vector<std::string> dayNames = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"
};

static const std::vector<std::string> progNames = {
    "C++", "Python: data analysis", "Web development"
};
static const std::vector<std::string> mathNames = {
    "Linear algebra", "Mathematical analysis",
    "Discrete mathematics", "Probability theory"
};
static const std::vector<std::string> physNames = {
    "Mechanics", "Electricity and magnetism",
    "Optics", "Thermodynamics"
};

/**
 * Создает банк учебных курсов трех типов
 * @return вектор указателей на созданные курсы
 */
std::vector<std::shared_ptr<TrainingCourse>> makeCourseBank() {
    std::vector<std::shared_ptr<TrainingCourse>> bank;
    for (auto& n : progNames)
        bank.push_back(std::make_shared<Programming>(n, random_number(20, 80),
            random_number(1, 5), "C++"));
    for (auto& n : mathNames)
        bank.push_back(std::make_shared<Mathematics>(n, random_number(20, 80),
            random_number(1, 5), "Chapter 1"));
    for (auto& n : physNames)
        bank.push_back(std::make_shared<Physics>(n, random_number(20, 80),
            random_number(1, 5), random_number(1, 8)));
    return bank;
}

/**
 * Создает банк преподавателей с базовыми рейтингами и предметами
 * @return вектор указателей на созданных преподавателей
 */
std::vector<std::shared_ptr<Teacher>> makeTeacherBank() {
    std::vector<std::shared_ptr<Teacher>> t;
    t.push_back(std::make_shared<Teacher>(Teacher{
        "Ivanov I.I.", 4.5, {"Programming"} }));
    t.push_back(std::make_shared<Teacher>(Teacher{
        "Petrov P.P.", 4.0, {"Mathematics"} }));
    t.push_back(std::make_shared<Teacher>(Teacher{
        "Sidorova A.A.", 4.8, {"Mathematics", "Physics"} }));
    t.push_back(std::make_shared<Teacher>(Teacher{
        "Kuznetsov K.K.", 3.9, {"Physics"} }));
    t.push_back(std::make_shared<Teacher>(Teacher{
        "Smirnova S.S.", 4.7, {"Programming", "Mathematics"} }));
    t.push_back(std::make_shared<Teacher>(Teacher{
        "Volkov V.V.", 4.2, {"Programming", "Physics"} }));
    return t;
}

/**
 * Генерирует расписание одной группы на одну неделю
 * Учитывает ограничение не более двух занятий по одному курсу в неделю
 * и отсутствие повторов курса в один день, подбирает подходящего преподавателя
 * @param bank банк доступных курсов
 * @param teachers банк преподавателей
 * @return список дней с занятиями
 */
std::vector<Day> generateWeekForGroup(
    const std::vector<std::shared_ptr<TrainingCourse>>& bank,
    std::vector<std::shared_ptr<Teacher>>& teachers)
{
    std::vector<Day> week;
    std::map<std::string, int> courseCountInWeek;

    for (auto& dn : dayNames) {
        Day day;
        day.name_day = dn;
        int lessonsCount = random_number(1, 4);

        for (int slot = 0; slot < lessonsCount; ++slot) {
            std::shared_ptr<TrainingCourse> chosenCourse;
            std::shared_ptr<Teacher> chosenTeacher;

            for (int attempt = 0; attempt < 300; ++attempt) {
                auto cand = bank[random_number(0, (int)bank.size() - 1)];

                if (courseCountInWeek[cand->name()] >= 2) continue;

                bool dup = false;
                for (auto& l : day.lessons)
                    if (l.course->name() == cand->name()) { dup = true; break; }
                if (dup) continue;

                std::vector<std::shared_ptr<Teacher>> free;
                for (auto& t : teachers) {
                    if (!t->canTeach(cand->type())) continue;
                    free.push_back(t);
                }

                if (free.empty()) continue;

                std::sort(free.begin(), free.end(),
                    [](auto& a, auto& b) { return a->rating > b->rating; });
                int idx = (random_number(0, 9) < 7) ? 0 : random_number(0, (int)free.size() - 1);

                chosenCourse = cand;
                chosenTeacher = free[idx];
                break;
            }

            if (!chosenCourse) continue;

            day.lessons.push_back(Lesson{ chosenCourse, chosenTeacher });
            courseCountInWeek[chosenCourse->name()]++;
        }
        week.push_back(day);
    }
    return week;
}

/**
 * Проводит занятие и обновляет статистику преподавателя
 * Рейтинг за занятие зависит от базового рейтинга, случайного отклонения
 * и штрафа за большую группу
 * @param l проведенное занятие
 * @param g группа, в которой проводится занятие
 */
void conductLesson(Lesson& l, const Group& g) {
    l.teacher->lessonsHeld++;
    double noise = random_double(-0.5, 0.5);
    double penalty = (g.size > 25) ? 0.2 : 0.0;
    double r = l.teacher->rating + noise - penalty;
    r = std::clamp(r, 1.0, 5.0);
    l.teacher->ratingSum += r;
    l.teacher->ratingCount++;
}

/**
 * Выводит расписание недели для группы
 * @param ws расписание недели
 * @param g группа
 */
void printWeek(const WeekSchedule& ws, const Group& g) {
    std::cout << "\n===== Group " << g.name << " | Week " << ws.weekNumber << " =====\n";
    for (auto& d : ws.days) {
        std::cout << "--- " << d.name_day << " ---\n";
        if (d.lessons.empty()) { std::cout << "  (no lessons)\n"; continue; }
        for (size_t i = 0; i < d.lessons.size(); ++i) {
            std::cout << "  " << i + 1 << ") " << *d.lessons[i].course
                << " | " << d.lessons[i].teacher->name
                << " (rating " << std::fixed << std::setprecision(1)
                << d.lessons[i].teacher->rating << ")\n";
        }
    }
}

/**
 * Выводит итоговую статистику рейтингов преподавателей
 * Сортирует преподавателей по среднему фактическому рейтингу
 * @param teachers список преподавателей
 */
void printTeacherStats(const std::vector<std::shared_ptr<Teacher>>& teachers) {
    std::cout << "\n===== TEACHER RATINGS AT THE END OF THE SEMESTER =====\n";
    std::cout << std::left << std::setw(18) << "Teacher"
        << std::setw(12) << "Base"
        << std::setw(12) << "Avg actual"
        << std::setw(10) << "Lessons" << "\n";
    std::cout << std::string(52, '-') << "\n";

    auto sorted = teachers;
    std::sort(sorted.begin(), sorted.end(),
        [](auto& a, auto& b) { return a->avgRating() > b->avgRating(); });

    for (auto& t : sorted) {
        std::cout << std::left << std::setw(18) << t->name
            << std::setw(12) << std::fixed << std::setprecision(1) << t->rating
            << std::setw(12) << t->avgRating()
            << std::setw(10) << t->lessonsHeld << "\n";
    }
}

/**
 * Находит курс по имени в семестре
 * @param semester семестр с расписаниями всех групп
 * @param name имя искомого курса
 * @return указатель на курс или nullptr, если курс не найден
 */
std::shared_ptr<TrainingCourse> findCourseInSemester(
    const std::vector<std::vector<WeekSchedule>>& semester,
    const std::string& name)
{
    for (auto& groupWeeks : semester)
        for (auto& ws : groupWeeks)
            for (auto& d : ws.days)
                for (auto& l : d.lessons)
                    if (l.course->name() == name)
                        return l.course;
    return nullptr;
}

/**
 * Выводит список всех уникальных курсов семестра с их статусом
 * @param semester семестр с расписаниями всех групп
 */
void printCourseListInSemester(const std::vector<std::vector<WeekSchedule>>& semester) {
    std::vector<std::string> shown;
    std::cout << "Course list\n";
    bool any = false;

    for (auto& groupWeeks : semester)
        for (auto& ws : groupWeeks)
            for (auto& d : ws.days)
                for (auto& l : d.lessons) {
                    std::string name = l.course->name();
                    if (std::find(shown.begin(), shown.end(), name) != shown.end())
                        continue;
                    shown.push_back(name);
                    std::cout << "  - " << *l.course
                        << " | status: " << l.course->status() << "\n";
                    any = true;
                }

    if (!any) std::cout << "no courses\n";
    std::cout << "\n";
}

/**
 * Выводит главное меню программы
 */
void printMenu() {
    std::cout << " 1. Generate semester (16 weeks, 2 groups)\n";
    std::cout << " 2. Show week schedule (choose group and week)\n";
    std::cout << " 3. Show course list\n";
    std::cout << " 4. Mark course as completed\n";
    std::cout << " 5. Mark course as not completed\n";
    std::cout << " 6. Show course status\n";
    std::cout << " 7. Call study() for a course\n";
    std::cout << " 8. Call checkKnowledge() for a course\n";
    std::cout << " 9. Call teachingMethod() for a course\n";
    std::cout << "10. Call methodOfStudy() for a course\n";
    std::cout << "11. Show teacher ratings\n";
    std::cout << " 0. Exit\n";
    std::cout << "==========================\n";
    std::cout << "Choice: ";
}

/**
 * Точка входа в программу
 * Генерирует семестр, проводит занятия и обрабатывает меню пользователя
 * @return 0 после завершения программы
 */
int main()
{
    const int WEEKS = 16;
    const int GROUPS = 2;

    std::vector<std::vector<WeekSchedule>> semester;
    std::vector<Group> groups = { { "IS-21", 25 }, { "PM-22", 18 } };

    auto bank = makeCourseBank();
    auto teachers = makeTeacherBank();

    int choice = -1;

    try {
        while (choice != 0) {
            printMenu();
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Input error. Enter a number.\n\n";
                continue;
            }

            std::cout << "\n";

            switch (choice) {
            case 1: {
                semester.clear();
                semester.resize(GROUPS);

                for (int w = 1; w <= WEEKS; ++w) {
                    for (int g = 0; g < GROUPS; ++g) {
                        auto days = generateWeekForGroup(bank, teachers);
                        WeekSchedule ws{ w, days };
                        semester[g].push_back(ws);

                        for (auto& d : days)
                            for (auto& l : d.lessons)
                                conductLesson(l, groups[g]);
                    }
                }
                std::cout << "Semester generated (" << WEEKS
                    << " weeks, " << GROUPS << " groups).\n";

                for (int g = 0; g < GROUPS; ++g) {
                    printWeek(semester[g][0], groups[g]);
                }
                std::cout << "\n";
                break;
            }

            case 2: {
                if (semester.empty()) {
                    std::cout << "Generate the semester first (option 1).\n\n";
                    break;
                }

                std::cout << "Choose group (1.." << GROUPS << "): ";
                int g; std::cin >> g;
                if (g < 1 || g > GROUPS) {
                    std::cout << "Invalid group.\n\n";
                    break;
                }

                std::cout << "Choose week (1.." << WEEKS << "): ";
                int w; std::cin >> w;
                if (w < 1 || w > WEEKS) {
                    std::cout << "Invalid week.\n\n";
                    break;
                }

                printWeek(semester[g - 1][w - 1], groups[g - 1]);
                std::cout << "\n";
                break;
            }

            case 3: {
                if (semester.empty()) {
                    std::cout << "Generate the semester first (option 1).\n\n";
                    break;
                }
                printCourseListInSemester(semester);
                break;
            }

            case 4:
            case 5: {
                if (semester.empty()) {
                    std::cout << "Generate the semester first (option 1).\n\n";
                    break;
                }

                std::cout << "Enter course name: ";
                std::cin.ignore();
                std::string name;
                std::getline(std::cin, name);

                auto course = findCourseInSemester(semester, name);
                if (course == nullptr) {
                    std::cout << "Course \"" << name << "\" not found.\n\n";
                    break;
                }

                bool value = (choice == 4);
                course->setDone(value);
                std::cout << "Course \"" << name << "\" marked as "
                    << course->status() << ".\n\n";
                break;
            }

            case 6: {
                if (semester.empty()) {
                    std::cout << "Generate the semester first (option 1).\n\n";
                    break;
                }

                std::cout << "Enter course name: ";
                std::cin.ignore();
                std::string name;
                std::getline(std::cin, name);

                auto course = findCourseInSemester(semester, name);
                if (course == nullptr) {
                    std::cout << "Course \"" << name << "\" not found.\n\n";
                    break;
                }

                std::cout << "Course: " << *course << "\n";
                std::cout << "Status: " << course->status() << "\n\n";
                break;
            }

            case 7:
            case 8:
            case 9:
            case 10: {
                if (semester.empty()) {
                    std::cout << "Generate the semester first (option 1).\n\n";
                    break;
                }

                std::cout << "Enter course name: ";
                std::cin.ignore();
                std::string name;
                std::getline(std::cin, name);

                auto course = findCourseInSemester(semester, name);
                if (course == nullptr) {
                    std::cout << "Course \"" << name << "\" not found.\n\n";
                    break;
                }

                if (choice == 7)  course->study();
                if (choice == 8)  course->checkKnowledge();
                if (choice == 9)  course->teachingMethod();
                if (choice == 10) course->methodOfStudy();
                std::cout << "\n";
                break;
            }

            case 11: {
                if (semester.empty()) {
                    std::cout << "Generate the semester first (option 1).\n\n";
                    break;
                }
                printTeacherStats(teachers);
                std::cout << "\n";
                break;
            }

            case 0: {
                std::cout << "Exit.\n";
                break;
            }

            default: {
                std::cout << "Invalid menu option.\n\n";
                break;
            }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}