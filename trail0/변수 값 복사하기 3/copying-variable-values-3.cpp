#include <iostream>
using namespace std;

int main() {
    int a = 1;
    int b = 5;
    int c = 3;

    a = c;
    a = a+c;
    b = b-c;

    cout << a<<"\n"<<b<<"\n"<<c;
    return 0;
}