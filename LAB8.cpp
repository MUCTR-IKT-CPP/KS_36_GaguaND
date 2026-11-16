#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <limits>
#include <cctype>

/*Проверяет, является ли строка корректным номером телефона.
*
* @param phone Строка с номером телефона для проверки.
* @return true, если номер корректен; false в противном случае.
*/
bool isValidPhone(const std::string& phone) {
    if (phone.length() != 11) {
        return false;
    }
    for (char c : phone) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

class IContact {
public:
    virtual ~IContact() = default;
    virtual int getId() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getPhone() const = 0;
};

class IContactRepo {
public:
    virtual ~IContactRepo() = default;

    virtual void add(const std::shared_ptr<IContact>& contact) = 0;
    virtual void remove(int id) = 0;
    virtual std::shared_ptr<IContact> findById(int id) const = 0;
    virtual std::vector<std::shared_ptr<IContact>> getAll() const = 0;
};

class IGroup {
public:
    virtual ~IGroup() = default;
    virtual std::string getName() const = 0;
    virtual std::vector<int> getMemberIds() const = 0;
};

class Contact : public IContact {
public:
    Contact(int id, std::string name, std::string phone)
        : id_{ id }, name_{ std::move(name) }, phone_{ std::move(phone) } {}

    int getId() const override { return id_; }
    std::string getName() const override { return name_; }
    std::string getPhone() const override { return phone_; }

private:
    int id_;
    std::string name_;
    std::string phone_;
};

class ContactRepo : public IContactRepo {
public:
    /*Добавляет контакт в репозиторий.
    * 
    * @param contact Указатель на контакт для добавления.
    * @return ничего не возвращает.
    */
    void add(const std::shared_ptr<IContact>& contact) override {
        int id = contact->getId();
        if (contacts_map_.find(id) == contacts_map_.end()) {
            contacts_map_[id] = contact;
            insertion_order_.push_back(id);
        }
        else {
            std::cout << "Contact with ID " << id << " already exists!" << std::endl;
        }
    }

    /*Удаляет контакт по его идентификатору.
    * 
    * @param id Идентификатор контакта для удаления.
    * @return ничего не возвращает.
    */
    void remove(int id) override {
        auto it = contacts_map_.find(id);
        if (it != contacts_map_.end()) {
            contacts_map_.erase(it);
            auto order_it = std::find(insertion_order_.begin(), insertion_order_.end(), id);
            if (order_it != insertion_order_.end()) {
                insertion_order_.erase(order_it);
            }
        }
        else {
            std::cout << "Contact with ID " << id << " not found." << std::endl;
        }
    }

    std::shared_ptr<IContact> findById(int id) const override {
        auto it = contacts_map_.find(id);
        if (it != contacts_map_.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<IContact>> getAll() const override {
        std::vector<std::shared_ptr<IContact>> result;
        result.reserve(insertion_order_.size());
        for (int id : insertion_order_) {
            auto it = contacts_map_.find(id);
            if (it != contacts_map_.end()) {
                result.push_back(it->second);
            }
        }
        return result;
    }

private:
    std::map<int, std::shared_ptr<IContact>> contacts_map_;
    std::vector<int> insertion_order_;
};

class Group : public IGroup {
public:
    explicit Group(std::string name) : name_{ std::move(name) } {}

    std::string getName() const override { return name_; }
    std::vector<int> getMemberIds() const override { return member_ids_; }

    /*Добавляет участника в группу, если его ещё нет.
    * 
    * @param id Идентификатор контакта.
    * @return ничего не возвращает.
    */
    void addMember(int id) {
        if (std::find(member_ids_.begin(), member_ids_.end(), id) == member_ids_.end()) {
            member_ids_.push_back(id);
        }
    }

    /*Сливает участников из другой группы в текущую без дублирования.
    * 
    * @param source Исходная группа для слияния.
    * @return ничего не возвращает.
    */
    void mergeFrom(const Group& source) {
        for (int id : source.getMemberIds()) {
            addMember(id);
        }
    }

private:
    std::string name_;
    std::vector<int> member_ids_;
};

class GroupManager {
public:
    /*Создаёт новую группу с указанным названием.
    *
    * @param name Название группы.
    * @return ничего не возвращает.
    */
    void addGroup(const std::string& name) {
        if (groups_map_.find(name) == groups_map_.end()) {
            groups_map_[name] = std::make_shared<Group>(name);
        }
        else {
            std::cout << "Group '" << name << "' already exists." << std::endl;
        }
    }

    std::shared_ptr<Group> getGroup(const std::string& name) const {
        auto it = groups_map_.find(name);
        if (it != groups_map_.end()) {
            return it->second;
        }
        return nullptr;
    }

    /*Сливает участников из исходной группы в целевую.
    * 
    * @param sourceName Название исходной группы.
    * @param targetName Название целевой группы.
    * @return ничего не возвращает.
    */
    void mergeGroups(const std::string& sourceName, const std::string& targetName) {
        auto source = getGroup(sourceName);
        auto target = getGroup(targetName);

        if (!source) {
            std::cout << "Source group '" << sourceName << "' not found." << std::endl;
            return;
        }
        if (!target) {
            std::cout << "Target group '" << targetName << "' not found." << std::endl;
            return;
        }
        if (source == target) {
            std::cout << "Cannot merge a group with itself." << std::endl;
            return;
        }

        target->mergeFrom(*source);
        std::cout << "Groups merged successfully." << std::endl;
    }

    /*Выводит список всех групп и их участников в консоль.
    *
    * @return ничего не возвращает.
    */
    void printAllGroups() const {
        if (groups_map_.empty()) {
            std::cout << "No groups yet." << std::endl;
            return;
        }
        for (const auto& [name, group] : groups_map_) {
            std::cout << "Group '" << name << "': ";
            auto members = group->getMemberIds();
            if (members.empty()) {
                std::cout << "(empty)";
            }
            else {
                for (int id : members) {
                    std::cout << id << " ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    std::map<std::string, std::shared_ptr<Group>> groups_map_;
};

class AddressBook {
public:
    ContactRepo& getRepo() { return repo_; }
    GroupManager& getGroupManager() { return group_manager_; }

private:
    ContactRepo repo_;
    GroupManager group_manager_;
};

int main() {
    AddressBook book;
    auto& repo = book.getRepo();
    auto& gm = book.getGroupManager();

    int choice = 0;

    while (true) {
        std::cout << std::endl << "ADDRESS BOOK" << std::endl;
        std::cout << "1. Add contact" << std::endl;
        std::cout << "2. Remove contact" << std::endl;
        std::cout << "3. Find contact by ID" << std::endl;
        std::cout << "4. Show all contacts" << std::endl;
        std::cout << "5. Create group" << std::endl;
        std::cout << "6. Add contact to group" << std::endl;
        std::cout << "7. Merge two groups" << std::endl;
        std::cout << "8. Show all groups" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Choose an action: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            int id;
            std::string name, phone;

            std::cout << "Enter ID: ";
            if (!(std::cin >> id)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Must be a number." << std::endl;
                break;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Enter name: ";
            std::getline(std::cin, name);

            std::cout << "Enter phone (11 digits): ";
            std::getline(std::cin, phone);

            if (!isValidPhone(phone)) {
                std::cout << "Invalid phone number. Must be exactly 11 digits." << std::endl;
                break;
            }

            repo.add(std::make_shared<Contact>(id, name, phone));
            std::cout << "Contact added." << std::endl;
            break;
        }
        case 2: {
            int id;
            std::cout << "Enter ID to remove: ";
            if (!(std::cin >> id)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Must be a number." << std::endl;
                break;
            }
            repo.remove(id);
            break;
        }
        case 3: {
            int id;
            std::cout << "Enter ID to search: ";
            if (!(std::cin >> id)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Must be a number." << std::endl;
                break;
            }
            auto contact = repo.findById(id);
            if (contact) {
                std::cout << "Found: " << contact->getName()
                    << " (" << contact->getPhone() << ")" << std::endl;
            }
            else {
                std::cout << "Contact not found." << std::endl;
            }
            break;
        }
        case 4: {
            auto all = repo.getAll();
            if (all.empty()) {
                std::cout << "Contact list is empty." << std::endl;
            }
            else {
                for (const auto& c : all) {
                    std::cout << c->getId() << ": " << c->getName()
                        << " (" << c->getPhone() << ")" << std::endl;
                }
            }
            break;
        }
        case 5: {
            std::string name;
            std::cout << "Enter group name: ";
            std::getline(std::cin, name);
            gm.addGroup(name);
            break;
        }
        case 6: {
            std::string group_name;
            int id;
            std::cout << "Enter group name: ";
            std::getline(std::cin, group_name);
            auto group = gm.getGroup(group_name);
            if (!group) {
                std::cout << "Group not found." << std::endl;
                break;
            }
            std::cout << "Enter contact ID to add: ";
            if (!(std::cin >> id)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Must be a number." << std::endl;
                break;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (!repo.findById(id)) {
                std::cout << "Contact with this ID does not exist." << std::endl;
            }
            else {
                group->addMember(id);
                std::cout << "Contact added to group." << std::endl;
            }
            break;
        }
        case 7: {
            std::string src, tgt;
            std::cout << "Enter source group name: ";
            std::getline(std::cin, src);
            std::cout << "Enter target group name: ";
            std::getline(std::cin, tgt);
            gm.mergeGroups(src, tgt);
            break;
        }
        case 8: {
            gm.printAllGroups();
            break;
        }
        case 0: {
            std::cout << "Exiting program." << std::endl;
            return 0;
        }
        default: {
            std::cout << "Invalid option." << std::endl;
            break;
        }
        }
    }

    return 0;
}