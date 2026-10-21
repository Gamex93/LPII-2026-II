#include <iostream>

// realice un programa que diga si un numero es capicua o no, usando solo los operadores aritmeticos / y %

using namespace std;

int main(){
	int valor;
	int unidad;
	int decena;
	int centena;
	int valorCompleto;
	int invertido;
	
	do {
		cout<<"Ingrese un numero de maximo de 4 digitos"<<endl;
		cin>>valor;
		if (valor < 1 || valor > 9999) {
			cout<<"Valor invalido"<<endl;
		}
	} while (valor < 1 || valor > 9999);
	
	if (valor >= 1 && valor <= 9) {
		invertido = valor;
	} else if (valor <= 99) {
		unidad = valor % 10;
		decena = valor / 10;
		invertido = (unidad * 10) + decena;
	} else if (valor <= 999) {
		unidad = valor % 10;
		decena = (valor / 10) % 10;
		centena = valor / 100;
		invertido = (unidad * 100) + (decena * 10) + centena;
	} else if (valor <= 9999) {
		unidad = valor % 10;
		decena = (valor / 10) % 10;
		centena = (valor / 100) % 10;
		valorCompleto = valor / 1000;
		invertido = (unidad * 1000) + (decena * 100) + (centena * 10) + valorCompleto;
	}
	
	cout<<"El numero invertido es: "<<invertido<<endl;
	
	if (valor == invertido) {
		cout<<"Es capicua."<<endl;
	} else {
		cout<<"No es capicua."<<endl;
	}
	
	return 0;
}
