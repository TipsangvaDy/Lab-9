// CS 2414 Lab 9
// Part A: Separate chaining (complete – read and trace)
// Part B: Coalesced chaining (you complete insert and find)

#include <iostream>
#include <vector>
#include <list>
#include <string>

class Student {
public:
    int id;
    std::string name;
    double gpa;
    Student(int i, std::string n, double g) : id(i), name(n), gpa(g) {}
    Student() : id(0), name(""), gpa(0.0) {}
};

// ========== Part A: Separate chaining (do not change) ==========

struct Entry {
    int key;
    Student value;
};

class HashTable {
private:
    std::vector<std::list<Entry>> buckets;
    size_t M;
    size_t hash(int key) const { return static_cast<size_t>(key) % M; }
public:
    explicit HashTable(size_t m = 7) : buckets(m), M(m) {}

    void insert(const Student& s) {
        size_t i = hash(s.id);
        Entry e{s.id, s};
        buckets[i].push_front(e);
    }

    bool find(int id, Student& out) const {
        size_t i = hash(id);
        for (const auto& e : buckets[i]) {
            if (e.key == id) {
                out = e.value;
                return true;
            }
        }
        return false;
    }

    void print() const {
        std::cout << "Separate chaining (H(id) = id % " << M << "):\n";
        for (size_t i = 0; i < M; ++i) {
            std::cout << "[" << i << "] ";
            if (buckets[i].empty()) std::cout << "—";
            else {
                bool first = true;
                for (const auto& e : buckets[i]) {
                    if (!first) std::cout << " -> ";
                    std::cout << "(" << e.value.id << ", " << e.value.name
                              << ", " << e.value.gpa << ")";
                    first = false;
                }
            }
            std::cout << "\n";
        }
    }
};

// ========== Part B: Coalesced chaining (you complete insert and find) ==========

struct CoalescedSlot {
    int key = -1;          // -1 means empty
    Student value;
    int next = -1;         // index of next slot in chain, -1 = end
    bool occupied() const { return key != -1; }
};

class CoalescedTable {
private:
    std::vector<CoalescedSlot> table;
    size_t M;

    size_t hash(int key) const { return static_cast<size_t>(key) % M; }

    // Helper: return the index of the first free slot, or -1 if the table is full.
    int findFree() {
        for (int i = static_cast<int>(M) - 1; i >= 0; --i) {
            if (!table[i].occupied()) return i;
        }
        return -1;
    }

public:
    explicit CoalescedTable(size_t m = 7) : table(m), M(m) {}

    void insert(const Student& s) {
        // Your code here
    }

    
    bool find(int id, Student& out) const {
        // Your code here
        return false;
    }

    void print() const {
        std::cout << "Coalesced chaining (H(id) = id % " << M << "):\n";
        for (size_t i = 0; i < M; ++i) {
            std::cout << "[" << i << "] ";
            if (!table[i].occupied()) std::cout << "—";
            else {
                std::cout << "(" << table[i].value.id << ", "
                          << table[i].value.name << ", "
                          << table[i].value.gpa << ")";
                if (table[i].next != -1)
                    std::cout << " next=" << table[i].next;
            }
            std::cout << "\n";
        }
    }
};

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<Student> students;
    for (int i = 0; i < n; ++i) {
        int id;
        std::string name;
        double gpa;
        std::cin >> id >> name >> gpa;
        students.emplace_back(id, name, gpa);
    }

    std::cout << "========== Part A: Separate chaining ==========\n";
    HashTable table(7);
    for (const auto& s : students) table.insert(s);
    std::cout << "\nAfter all insertions:\n";
    table.print();

    std::cout << "\n========== Part B: Coalesced chaining ==========\n";
    CoalescedTable ct(7);
    for (const auto& s : students) ct.insert(s);
    std::cout << "\nAfter all insertions:\n";
    ct.print();

    // Find tests using IDs from the input (plus one missing ID)
    std::cout << "\n--- Part A finds ---\n";
    Student found;
    std::vector<int> test_ids;
    if (!students.empty()) {
        test_ids.push_back(students.front().id);
        test_ids.push_back(students.back().id);
        if (students.size() > 2) test_ids.push_back(students[students.size()/2].id);
    }
    test_ids.push_back(999);   // missing
    for (int id : test_ids) {
        if (table.find(id, found))
            std::cout << "Found id " << id << ": " << found.name << "\n";
        else
            std::cout << "id " << id << " not found\n";
    }

    std::cout << "\n--- Part B finds ---\n";
    for (int id : test_ids) {
        if (ct.find(id, found))
            std::cout << "Found id " << id << ": " << found.name << "\n";
        else
            std::cout << "id " << id << " not found\n";
    }

    return 0;
}
