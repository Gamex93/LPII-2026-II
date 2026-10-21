#include <iostream>
#include <string>

// realice un programa que calcule el promedio de 10 notas ingresadas por teclado, validando que cada nota este entre 1 y 20

using namespace std;

int main() {
    string nombre = "Jean Franco";
    float nota;
    float suma = 0;
    float promedio;
    
    for (int i = 1; i <= 10; i++) {
        cout << "Ingrese la nota " << i << endl;
        cin >> nota;
        
        suma += nota;
    }
    
    return 0;
}
