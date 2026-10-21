#include <iostream>

//realice un programa que diga si un numero es capicua o no, usando solo los operadores aritmeticos / y % (no se permite convertir el numero
//en cadena de texto).el numero debe estar dentro del siguiente rango> 9 < x < 1000.

using namespace std;

int main(){
	int valor;
	int unidad;
	int decena;
	int centena;
	int valorCompleto;
	int invertido;
	
	
	do {
		cout<<"Ingrese un numero de maximo de 4 digitos"<<end;
		cin>>valor;
	} while (valor < 1 || valor > 9999) {
		cout<<"Valor invalido"<<end;
	}
	
	if (valor <= 9) {
		invertido = valor;
		
	} else if (valor <= 99) {
		unidad = valor % 10;
		decena = (valor / 10) % 10;
		
		suma = unidad + decena;
		
		invertido = (unidad * 10) + decena;
		
	} else if (valor <= 999) {
		unidad = valor % 10;
		decena = (valor / 10) % 10;
		centena = (valor / 100) % 10;
		
		suma = unidad + decena + centena;
		
		invertido = (unidad * 100) + (decena * 10) + centena;
		
	} else if (valor <= 9999) {
		unidad = valor % 10;
		decena = (valor / 10) % 10;
		centena = (valor / 100) % 10;
		valorCompleto = valor / 1000;
		
		suma = unidad + decena + centena + valorCompleto;
		
		invertido = (unidad * 1000) + (decena * 100) + (centena * 10) + valorCompleto;
	}
	
	cout<<"El numero invertido es: "<<invertido<<end;
	
	if (valor == invertido) {
		cout<<"Es capicua. "<<end;
	} else {
		cout<<"No es capicua. "<<end;
	}
	
	return 0;
}