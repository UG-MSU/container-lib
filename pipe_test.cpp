#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main() {
    string str = "";
    cin >> str;
    str += " said kukareku hehe";
    cout << str << endl;
    return 0;
}
