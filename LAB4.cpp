#include <iostream>
#include <clocale>
#include <string>
#include <utility>
#include <cstddef>

using namespace std;

enum DeviceType {
    LIGHT,
    THERMOSTAT,
    CAMERA
};

/* Структура, описывающая лампу.
 *
 * @var Light::brightness          хранит информацию о яркости свечения.
 */
struct Light {
    int brightness = 0;
};

/* Структура, описывающая термостат.
 *
 * @var Thermostat::temperature    хранит целевую температуру в градусах Цельсия.
 */
struct Thermostat {
    double temperature = 30.0;
};

/* Структура, описывающая камеру видеонаблюдения.
 *
 * @var Camera::is_recording       хранит текущий статус записи.
 */
struct Camera {
    bool is_recording = false;
};

class SmartDevice {
private:
    void* device_data = nullptr;
    DeviceType type = LIGHT;
    bool is_online = false;

public:

    /* Конструктор с параметрами. Выделяет память под конкретные данные устройства.
     *
     * @param t                     тип устройства.
     * @param online                статус подключения к сети.
     */
    SmartDevice(DeviceType t, bool online) : type(t), is_online(online), device_data(nullptr) {
        switch (type) {
        case LIGHT:      device_data = new Light();      break;
        case THERMOSTAT: device_data = new Thermostat(); break;
        case CAMERA:     device_data = new Camera();     break;
        default:         device_data = nullptr;          break;
        }
    }

    /* Деструктор. Освобождает память, приводя указатель к нужному типу. */
    ~SmartDevice() {
        if (device_data) {
            switch (type) {
            case LIGHT:      delete (Light*)device_data;      break;
            case THERMOSTAT: delete (Thermostat*)device_data; break;
            case CAMERA:     delete (Camera*)device_data;     break;
            default: break;
            }
            device_data = nullptr;
        }
    }

    /* Конструктор копирования. Выполняет глубокое копирование данных устройства.
     *
     * @param other                  копируемый объект.
     */
    SmartDevice(const SmartDevice& other) : type(other.type), is_online(other.is_online), device_data(nullptr) {
        switch (type) {
        case LIGHT:
            device_data = new Light(*(Light*)other.device_data);
            break;
        case THERMOSTAT:
            device_data = new Thermostat(*(Thermostat*)other.device_data);
            break;
        case CAMERA:
            device_data = new Camera(*(Camera*)other.device_data);
            break;
        default:
            device_data = nullptr;
            break;
        }
    }

    /* Оператор копирующего присваивания. Выполняет глубокое копирование данных.
     *
     * @param other                  копируемый объект.
     * @return                       ссылка на текущий объект.
     */
    SmartDevice& operator=(const SmartDevice& other) {
        if (this == &other) return *this;

        if (device_data) {
            switch (type) {
            case LIGHT:      delete (Light*)device_data;      break;
            case THERMOSTAT: delete (Thermostat*)device_data; break;
            case CAMERA:     delete (Camera*)device_data;     break;
            default: break;
            }
            device_data = nullptr;
        }

        type = other.type;
        is_online = other.is_online;
        switch (type) {
        case LIGHT:
            device_data = new Light(*(Light*)other.device_data);
            break;
        case THERMOSTAT:
            device_data = new Thermostat(*(Thermostat*)other.device_data);
            break;
        case CAMERA:
            device_data = new Camera(*(Camera*)other.device_data);
            break;
        default:
            device_data = nullptr;
            break;
        }
        return *this;
    }

    /* Конструктор перемещения. Забирает указатель и обнуляет старый.
     *
     * @param other                  перемещаемый объект.
     */
    SmartDevice(SmartDevice&& other) noexcept
        : device_data(other.device_data), type(other.type), is_online(other.is_online) {
        other.device_data = nullptr;
    }

    /* Оператор перемещающего присваивания.
     *
     * @param other                  перемещаемый объект.
     * @return                       ссылка на текущий объект.
     */
    SmartDevice& operator=(SmartDevice&& other) noexcept {
        if (this == &other) return *this;

        if (device_data) {
            switch (type) {
            case LIGHT:      delete (Light*)device_data;      break;
            case THERMOSTAT: delete (Thermostat*)device_data; break;
            case CAMERA:     delete (Camera*)device_data;     break;
            default: break;
            }
        }

        device_data = other.device_data;
        type = other.type;
        is_online = other.is_online;

        other.device_data = nullptr;
        return *this;
    }

    friend class SmartHome;
};

class SmartHome {
private:
    SmartDevice** devices;
    size_t size;
    size_t capacity;
    string name;

public:

    /* Конструктор по умолчанию. */
    SmartHome() : devices(nullptr), size(0), capacity(0), name("Empty Home") {
        cout << "Вызван конструктор по умолчанию\n";
    }

    /* Конструктор с параметрами.
     *
     * @param cap                     максимальный размер массива устройств.
     * @param n                       имя дома.
     */
    SmartHome(size_t cap, string n = "My Smart Home")
        : devices(nullptr), size(0), capacity(cap > 0 ? cap : 0), name(n) {
        if (capacity > 0) {
            devices = new SmartDevice * [capacity];
        }
        cout << "Вызван конструктор с параметрами (Размер массива: " << capacity << ")\n";
    }

    /* Конструктор копирования. Выполняет глубокое копирование системы.
     *
     * @param other                   копируемый объект.
     */
    SmartHome(const SmartHome& other)
        : devices(nullptr), size(other.size), capacity(other.capacity), name(other.name + " (Копия)") {
        if (capacity > 0) {
            devices = new SmartDevice * [capacity];
        }
        for (size_t i = 0; i < size && i < capacity; ++i) {
            devices[i] = new SmartDevice(*other.devices[i]);
        }
        cout << "Вызван конструктор копирования\n";
    }

    /* Оператор копирующего присваивания. Выполняет глубокое копирование системы.
     *
     * @param other                  копируемый объект.
     * @return                       ссылка на текущий объект.
     */
    SmartHome& operator=(const SmartHome& other) {
        cout << "Вызван оператор копирующего присваивания\n";
        if (this == &other) return *this;

        for (size_t i = 0; i < size; ++i) delete devices[i];
        delete[] devices;

        size = other.size;
        capacity = other.capacity;
        name = other.name;

        devices = nullptr;
        if (capacity > 0) {
            devices = new SmartDevice * [capacity];
        }
        for (size_t i = 0; i < size && i < capacity; ++i) {
            devices[i] = new SmartDevice(*other.devices[i]);
        }
        return *this;
    }

    /* Конструктор перемещения.
     *
     * @param other                  перемещаемый объект.
     */
    SmartHome(SmartHome&& other) noexcept
        : devices(other.devices), size(other.size), capacity(other.capacity), name(other.name) {
        other.devices = nullptr;
        other.size = 0;
        other.capacity = 0;
        cout << "Вызван конструктор перемещения\n";
    }

    /* Оператор перемещающего присваивания.
     *
     * @param other                  перемещаемый объект.
     * @return                       ссылка на текущий объект.
     */
    SmartHome& operator=(SmartHome&& other) noexcept {
        cout << "Вызван оператор перемещающего присваивания\n";
        if (this == &other) return *this;

        for (size_t i = 0; i < size; ++i) delete devices[i];
        delete[] devices;

        devices = other.devices;
        size = other.size;
        capacity = other.capacity;
        name = other.name;

        other.devices = nullptr;
        other.size = 0;
        other.capacity = 0;
        return *this;
    }

    /* Деструктор класса-менеджера. Зачищает всю динамическую память */
    ~SmartHome() {
        for (size_t i = 0; i < size; ++i) {
            delete devices[i];
        }
        delete[] devices;
        cout << "Деструктор очистил память для дома: " << name << "\n";
    }

    /* Добавление нового устройства в систему.
     *
     * @param type                    тип устройства.
     * @param online                  статус подключения к сети.
     * @return true                   если устройство добавлено; false, если массив переполнен.
     */
    bool addDevice(DeviceType type, bool online) {
        if (devices == nullptr || size >= capacity) {
            cout << "Массив переполнен! Невозможно добавить устройство.\n";
            return false;
        }
        devices[size] = new SmartDevice(type, online);
        ++size;
        return true;
    }

    /* Отправка команды (включить свет, изменить температуру, начать запись).
     *
     * @param index                  индекс устройства в массиве.
     */
    void sendCommand(size_t index) {
        if (index >= size) {
            cout << "Устройства с индексом " << index << " не существует!\n";
            return;
        }
        SmartDevice* dev = devices[index];
        cout << "Отправка команды на устройство " << index << ": ";

        switch (dev->type) {
        case LIGHT: {
            Light* light = (Light*)dev->device_data;
            light->brightness = 100;
            cout << "Свет включен на максимальную яркость (" << light->brightness << "%)\n";
            break;
        }
        case THERMOSTAT: {
            Thermostat* thermo = (Thermostat*)dev->device_data;
            thermo->temperature = 22.5;
            cout << "Целевая температура изменена на " << thermo->temperature << "°C\n";
            break;
        }
        case CAMERA: {
            Camera* camera = (Camera*)dev->device_data;
            camera->is_recording = true;
            cout << "Запись камеры активирована\n";
            break;
        }
        default:
            break;
        }
    }

    /* Проверка состояния.
     *
     * @param index                  индекс устройства в массиве.
     */
    void checkStatus(size_t index) const {
        if (index >= size) {
            cout << "Индекс вне диапазона.\n";
            return;
        }
        SmartDevice* dev = devices[index];
        cout << "Устройство №" << index << " | Сеть: "
            << (dev->is_online ? "ONLINE" : "OFFLINE") << " | Тип: ";
        switch (dev->type) {
        case LIGHT:      cout << "Освещение\n";         break;
        case THERMOSTAT: cout << "Термостат\n";         break;
        case CAMERA:     cout << "Камера наблюдения\n"; break;
        default:         cout << "Неизвестно\n";        break;
        }
    }

    /* Поиск по типу.
     *
     * @param type                    тип устройства.
     * @return                        количество устройств указанного типа в системе.
     */
    size_t countByType(DeviceType type) const {
        size_t count = 0;
        for (size_t i = 0; i < size; ++i) {
            if (devices[i]->type == type) count++;
        }
        return count;
    }

    /* Метод подсчета устройств, находящихся в сети.
     *
     * @return                        количество устройств с is_online == true.
     */
    size_t countOnlineDevices() const {
        size_t count = 0;
        for (size_t i = 0; i < size; ++i) {
            if (devices[i]->is_online) count++;
        }
        return count;
    }

    /* Удаление устройства по индексу с устранением дыры в массиве.
     *
     * @param index                   индекс устройства в массиве.
     * @return true                   если устройство удалено; false, если индекс неверный.
     */
    bool removeDevice(size_t index) {
        if (index >= size) {
            cout << "Ошибка. Неверный индекс.\n";
            return false;
        }
        delete devices[index];
        for (size_t i = index; i + 1 < size; ++i) {
            devices[i] = devices[i + 1];
        }
        devices[size - 1] = nullptr;
        size--;
        cout << "Устройство на индексе " << index << " удалено.\n";
        return true;
    }
};

int main() {

    setlocale(LC_ALL, "Ru");

    cout << "Инициализация системы умного дома\n";
    SmartHome myHome(5, "Villa");
    myHome.addDevice(LIGHT, true);
    myHome.addDevice(THERMOSTAT, false);
    myHome.addDevice(CAMERA, true);
    myHome.checkStatus(0);
    myHome.checkStatus(1);
    myHome.checkStatus(2);
    myHome.sendCommand(0);
    myHome.sendCommand(1);
    myHome.sendCommand(2);
    cout << "Всего термостатов в системе: " << myHome.countByType(THERMOSTAT) << "\n";
    cout << "Устройств подключено к сети: " << myHome.countOnlineDevices() << "\n";
    myHome.removeDevice(1);
    myHome.checkStatus(1);
    myHome.sendCommand(1);
    cout << "\nТестирование глубокого копирования (Rule of 5)\n";
    {
        cout << "Создание копии во внутреннем блоке\n";
        SmartHome temporaryCopy = myHome;
        temporaryCopy.checkStatus(0);
        cout << "Выход из внутреннего блока (работа деструктора копии)\n";
    }
    return 0;
}
