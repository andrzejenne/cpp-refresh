#include <iostream>
#include <string>

using namespace std;

int main()
{
	string alphabet {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    string a_cipher {"ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba"};
    
    string message {};
    string c_message {};
    
    cout << "Enter message to cipher below:" << endl;
    getline(cin, message);
    
    // cipher
    cout << "Encrypting message ..." << endl;
    
    size_t pos {};
    for (char c : message) {
        pos = alphabet.find(c);
        if (pos == string::npos) {
            c_message += c;
        } else {
            c_message += a_cipher.at(pos);
        }        
    }
    
    cout << c_message << endl;
    
    //decipher
    string d_message {};
    cout << "Decrypring message ..." << endl;

    for (char c : c_message) {
        pos = a_cipher.find(c);
        if (pos == string::npos) {
            d_message += c;
        } else {
            d_message += alphabet.at(pos);
        }        
    }
    
    cout << d_message << endl;
    
    
	return 0;
}
