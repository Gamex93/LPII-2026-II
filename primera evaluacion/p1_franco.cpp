#include <iostream>

// realice un programa que lea un numero entero n (n >= 1) y, usando un bucle, muestre en pantalla la suma de los n primeros numeros pares y la suma de los n primeros numeros impares

using namespace std;

int main() {
    int n;
    int sumaPares = 0;
    int sumaImpares = 0;
    
    do {
        cout<<"Ingrese un numero entero N mayor o igual a 1:"<<endl;
        cin>>n;
        if (n < 1) {
            cout<<"Error, el numero debe ser mayor o igual a 1"<<endl;
        }
    } while (n < 1);
    
    for (int i = 1; i <= n; i++) {
        sumaPares += 2 * i;
        sumaImpares += (2 * i) - 1;
    }
    
    cout<<"La suma de los "<<n<<" primeros pares es: "<<sumaPares<<endl;
    cout<<"La suma de los "<<n<<" primeros impares es: "<<sumaImpares<<endl;
    
    return 0;
}
