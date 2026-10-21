#include <iostream>

//realice un programa que reciba la tasa del euro del dia bcv en bs, y un valor monetario en euros, y retorne la conversion a bolivares
//con dos decimales. al terminar debe preguntar si desea otra conversion (s/n) y repetir mientras la respuesta sea si.

using namespace std;

int main(){
	float tasaBcv = 0;
	float valorEuro = 0;
	float conversion = 0;
	char respuesta;
	
	do {
		cout<<"Escriba la tasa BCV del dia: "<<endl;
		cin>>tasaBcv;
		
		cout<<"Escriba un valor monetario en Euros: "<<endl;
		cin>>valorEuro;
		
		conversion = tasaBcv * valorEuro;
		
		cout<<"La conversion a bolivares seria: "<<conversion<<" Bs. "<<endl;
		cout<<"Desea realizar otra conversion? S/N: "<<endl;
		cin>>respuesta;
			
	} while(respuesta == 'S' || respuesta == 's');

	return 0;
}