#include <iostream>

using namespace std;

int main() {
    int valor;
    int unidad;
    int decena;
    int centena;
    int invertido;
    
    do {
        cout << "Ingrese un numero entre 10 y 999" << endl;
        cin >> valor;
        if (valor <= 9 || valor >= 1000) {
            cout << "Valor invalido" << endl;
        }
    } while (valor <= 9 || valor >= 1000);
    
    if (valor <= 99) {
        unidad = valor % 10;
        decena = valor / 10;
        invertido = (unidad * 10) + decena;
    } else {
        unidad = valor % 10;
        decena = (valor / 10) % 10;
        centena = valor / 100;
        invertido = (unidad * 100) + (decena * 10) + centena;
    }
    
    cout << "El numero invertido es: " << invertido << endl;
    
    if (valor == invertido) {
        cout << "Es capicua." << endl;
    } else {
        cout << "No es capicua." << endl;
    }
    
    return 0;
}
