#include <iostream>
#include <string>

using namespace std;

int countChar(string line, char argument){
    int result = 0;
    for(unsigned i = 0; i < line.size();i++){
        if(line[i]==argument)result++;
    }
    return result;
}
int main(){
    string line = "";
    char arg = '0';
    cout << "Regel: ";
    cin >> line;
    cout << "Karakter: ";
    cin >> arg;

    cout << countChar(line,arg) << endl;
    return 0;
}