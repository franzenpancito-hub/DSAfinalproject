#include <iostream>
#include <string>
using namespace std;

// Function para i-compute ang factorial
long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    string pangalan;
    int numero;

    cout << "Ano ang pangalan mo? ";
    getline(cin, pangalan);

    cout << "Kumusta, " << pangalan << "!" << endl;

    cout << "Maglagay ng numero (0-20): ";
    cin >> numero;

    if (numero < 0 || numero > 20) {
        cout << "Hindi valid ang numero." << endl;
        return 1;
    }

    cout << "Ang factorial ng " << numero << " ay "
         << factorial(numero) << endl;

    return 0;
}
