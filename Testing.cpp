#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> vec;
    vec.push_back(25);
    vec.push_back(35);
    vec.push_back(45);

    cout << "size = " << vec.size() << endl;

    vec.pop_back();
    
    cout << vec.at(0) << endl;

    return 0;
}
