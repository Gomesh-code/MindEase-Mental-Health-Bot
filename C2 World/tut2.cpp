#include <iostream>
#include <string>
using namespace std;

class Student {
    public:
    string name;
    double* cgpaPtr;
    Student(string name, double cgpa) {
        this->name = name;
        cgpaPtr = new double;
        *cgpaPtr = cgpa;
    }
    ~Student() {
        cout << "Hi, I delete everything\n";
    }
    void getInfo() {
        cout << "name: " << name << endl;
        cout << "cgpa: " << *cgpaPtr << endl;
    }
};
int main() {
    Student s1("Heena Sharma", 9.69);
    s1.getInfo();
    return 0;
}

