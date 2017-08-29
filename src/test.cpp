//https://stackoverflow.com/questions/45944116/how-to-get-a-c-program-built-in-visual-studio-code-to-accept-user-input
#include<iostream>
#include<string>

using namespace std;

int main() {

    string name = "";

    cout << "HELLO WORLD\n\n";

    cin >> name;

    cout << "Hello " << name << '\n';
    
    return 0;
}
