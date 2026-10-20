/*
 * ============================================================================
 Nombre del proyecto : Cajero Automatico Banco PSM (Proyecto N. 1)
 Modelo A
 Asignatura          : Lenguaje de Programacion II
 Integrantes         : Jean Franco y Luis Torres
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

// Declaraciones de funciones del sistema (incluidas por defecto en g++/MinGW)
extern "C" int system(const char* comando);
#ifdef _WIN32
extern "C" int _getch(void);
#endif

// ============================================================================
// PROTOTIPOS DE FUNCIONES
// ============================================================================
void configurarConsola();
void limpiarPantalla();
void pausarPantalla();
void mostrarEncabezado(int seccion);
void mostrarMenu(double saldoActual);
int leerPINOculto();
bool validarPIN(int pinIngresado);
int leerEnteroEnRango(int min, int max, bool esPromptMenu);
double leerMontoPositivo();
double depositar(double saldo, double monto);
bool esMultiploDeCinco(double monto);
bool puedeRetirar(double saldo, double monto, double retiradoHoy);
void mostrarErrorRetiro(double saldo, double monto, double retiradoHoy);
double retirar(double saldo, double monto);
double calcularComision(double monto);
void mostrarResumen(int cantDepositos, int cantRetiros, int cantTransferencias,
                    double totalDepositado, double totalRetirado,
                    double totalTransferido, double totalComisiones,
                    double saldoFinal);

// ============================================================================
// FUNCION PRINCIPAL
// ============================================================================

// Que hace: Controla el flujo principal del cajero (ingreso con PIN y menu).
// Que recibe (Parametros): Ninguno.
// Que retorna: 0 al finalizar la ejecucion del programa.
int main() {
    configurarConsola();

    // Configuracion de salida con 2 decimales fijos para todos los montos
    cout << fixed << setprecision(2);

    double saldo = 500.00;
    int cantDepositos = 0;
    int cantRetiros = 0;
    int cantTransferencias = 0;
    double totalDepositado = 0.00;
    double totalRetirado = 0.00;
    double totalTransferido = 0.00;
    double totalComisiones = 0.00;

    // 1. Ingreso con PIN (Maximo 3 intentos)
    int intentos = 0;
    bool accesoConcedido = false;

    mostrarEncabezado(0);

    while (intentos < 3 && !accesoConcedido) {
        cout << "\n                                                Ingrese su PIN: ";
        int pinIngresado = leerPINOculto();

        if (pinIngresado == -1) {
            intentos++;
            cout << "                                  \033[91m[!] Error: el PIN solo debe contener numeros.\033[0m" << endl;
            if (intentos < 3) {
                cout << "                                  \033[91m[!] PIN incorrecto. Le quedan " << (3 - intentos) << " intento(s).\033[0m" << endl;
            }
        } else if (pinIngresado < 1000 || pinIngresado > 9999) {
            intentos++;
            cout << "                                  \033[91m[!] Error: el PIN debe tener 4 digitos (1000 a 9999).\033[0m" << endl;
            if (intentos < 3) {
                cout << "                                  \033[91m[!] PIN incorrecto. Le quedan " << (3 - intentos) << " intento(s).\033[0m" << endl;
            }
        } else if (validarPIN(pinIngresado)) {
            accesoConcedido = true;
        } else {
            intentos++;
            if (intentos < 3) {
                cout << "                                  \033[91m[!] PIN incorrecto. Le quedan " << (3 - intentos) << " intento(s).\033[0m" << endl;
            }
        }
    }

    if (!accesoConcedido) {
        cout << "\n                                    \033[91m============================================\033[0m" << endl;
        cout << "                                    \033[91m[X] TARJETA BLOQUEADA. INTENTE MAS TARDE.\033[0m" << endl;
        cout << "                                    \033[91m============================================\033[0m" << endl;
#ifdef _WIN32
        _getch();
#endif
        return 0;
    }

    // Menu principal con do-while y switch
    int opcion = 0;
    do {
        mostrarMenu(saldo);
        // Prompt alineado a la izquierda con la flecha de mayor (>)
        cout << "\n\033[36m>\033[0m ";
        opcion = leerEnteroEnRango(1, 6, true);

        switch (opcion) {
            case 1: {
                // Consultar saldo
                mostrarEncabezado(1);
                cout << "                                          Estado : \033[92mACTIVA\033[0m" << endl;
                cout << "                                          Saldo  : \033[92m" << saldo << " Bs\033[0m" << endl;
                cout << "                                          Limite : " << (2000.00 - totalRetirado) << " Bs disponibles hoy" << endl;
                pausarPantalla();
                break;
            }
            case 2: {
                // Depositar: mayor que 0 y hasta 10 000.00 Bs por operacion
                mostrarEncabezado(2);
                cout << "                                          Saldo actual    : " << saldo << " Bs" << endl;
                cout << "                                          Maximo permitido: 10000.00 Bs\n" << endl;

                double montoDeposito = 0.00;
                bool depositoValido = false;

                do {
                    cout << "                                          Monto a depositar: ";
                    montoDeposito = leerMontoPositivo();

                    if (montoDeposito > 10000.00) {
                        cout << "                                \033[91m[!] Error: el monto maximo por deposito es 10000.00 Bs.\033[0m\n" << endl;
                    } else {
                        depositoValido = true;
                    }
                } while (!depositoValido);

                saldo = depositar(saldo, montoDeposito);
                cantDepositos++;
                totalDepositado += montoDeposito;

                cout << "\n                                    \033[92m[+] Deposito exitoso. Saldo actual: " << saldo << " Bs\033[0m" << endl;
                pausarPantalla();
                break;
            }
            case 3: {
                // Retirar: multiplo de 5, no superar saldo y limite diario de 2000.00 Bs
                mostrarEncabezado(3);
                cout << "                                          Saldo disponible: " << saldo << " Bs" << endl;
                cout << "                                          Retirado hoy    : " << totalRetirado << " / 2000.00 Bs" << endl;
                cout << "                                          Billetes        : Multiplos de 5 Bs\n" << endl;

                if (saldo < 5.00) {
                    cout << "                                \033[91m[!] Error: saldo menor al retiro minimo (5.00 Bs).\033[0m" << endl;
                } else if (totalRetirado >= 2000.00) {
                    cout << "                                \033[91m[!] Error: ya alcanzo el limite diario de 2000.00 Bs.\033[0m" << endl;
                } else {
                    double montoRetiro = 0.00;
                    bool retiroValido = false;

                    do {
                        cout << "                                          Monto a retirar: ";
                        montoRetiro = leerMontoPositivo();

                        if (puedeRetirar(saldo, montoRetiro, totalRetirado)) {
                            retiroValido = true;
                        } else {
                            mostrarErrorRetiro(saldo, montoRetiro, totalRetirado);
                        }
                    } while (!retiroValido);

                    saldo = retirar(saldo, montoRetiro);
                    cantRetiros++;
                    totalRetirado += montoRetiro;

                    cout << "\n                                    \033[92m[+] Retiro exitoso. Saldo actual: " << saldo << " Bs\033[0m" << endl;
                }
                pausarPantalla();
                break;
            }
            case 4: {
                // Transferir: cuenta de 8 digitos, comision del 1%, saldo cubre monto + comision
                mostrarEncabezado(4);
                cout << "                                          Saldo disponible : " << saldo << " Bs" << endl;
                cout << "                                          Comision bancaria: 1% sobre el monto\n" << endl;

                if (saldo < 0.02) {
                    cout << "                                \033[91m[!] Error: saldo insuficiente para transferir.\033[0m" << endl;
                } else {
                    cout << "                                          Cuenta destino (8 digitos): ";
                    int cuentaDestino = leerEnteroEnRango(10000000, 99999999, false);

                    double montoTransferir = 0.00;
                    double comision = 0.00;
                    bool transferenciaValida = false;

                    do {
                        cout << "                                          Monto a transferir: ";
                        montoTransferir = leerMontoPositivo();
                        comision = calcularComision(montoTransferir);

                        if ((montoTransferir + comision) > saldo) {
                            cout << "                                \033[91m[!] Error: saldo insuficiente (requiere "
                                 << (montoTransferir + comision) << " Bs con comision).\033[0m\n" << endl;
                        } else {
                            transferenciaValida = true;
                        }
                    } while (!transferenciaValida);

                    saldo = saldo - (montoTransferir + comision);
                    cantTransferencias++;
                    totalTransferido += montoTransferir;
                    totalComisiones += comision;

                    cout << "\n                                    \033[92m[+] Transferencia exitosa a la cuenta " << cuentaDestino << ".\033[0m" << endl;
                    cout << "                                    Monto: " << montoTransferir << " Bs | Comision (1%): " << comision << " Bs" << endl;
                    cout << "                                    Saldo actual: " << saldo << " Bs" << endl;
                }
                pausarPantalla();
                break;
            }
            case 5: {
                // Resumen de la sesion
                mostrarEncabezado(5);
                mostrarResumen(cantDepositos, cantRetiros, cantTransferencias,
                               totalDepositado, totalRetirado, totalTransferido,
                               totalComisiones, saldo);
                pausarPantalla();
                break;
            }
            case 6: {
                // Salir mostrando el resumen
                mostrarEncabezado(6);
                mostrarResumen(cantDepositos, cantRetiros, cantTransferencias,
                               totalDepositado, totalRetirado, totalTransferido,
                               totalComisiones, saldo);
                cout << "\n                                     Gracias por usar el Banco PSM. Hasta luego." << endl;
#ifdef _WIN32
                _getch();
#endif
                break;
            }
        }
    } while (opcion != 6);

    return 0;
}

// ============================================================================
// DEFINICION DE FUNCIONES
// ============================================================================

// Que hace: Ajusta el tamano de la ventana de consola a 120 columnas y 30 lineas
//           para eliminar la barra de desplazamiento gigante y fijar el centrado.
// Que recibe (Parametros): Ninguno.
// Que retorna: Nada (void).
void configurarConsola() {
#ifdef _WIN32
    system("mode con: cols=120 lines=30");
#endif
}

// Que hace: Limpia todo el contenido previo de la consola segun el sistema operativo.
// Que recibe (Parametros): Ninguno.
// Que retorna: Nada (void).
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Que hace: Pausa la ejecucion hasta que el usuario presione ENTER para continuar.
// Que recibe (Parametros): Ninguno.
// Que retorna: Nada (void).
void pausarPantalla() {
    cout << "\n                                       Presione ENTER para volver al menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Que hace: Limpia la pantalla e imprime de forma centrada el logo de "Banco"
//           alineado a media altura junto al isotipo geometrico del PSM en azul.
// Que recibe (Parametros): int seccion (identificador de la pantalla actual).
// Que retorna: Nada (void).
void mostrarEncabezado(int seccion) {
    limpiarPantalla();

    cout << "\n";
    cout << "                                                                    \033[94m___________ ___________\033[0m" << endl;
    cout << "                                                                   \033[94m|\\_________ | _________/|\033[0m" << endl;
    cout << "                                                                   \033[94m||\\_______ ||| _______/||\033[0m" << endl;
    cout << "                            ____                                   \033[94m|||\\_____ ||||| _____/|||\033[0m" << endl;
    cout << "                           | __ )   __ _  _ __    ___   ___        \033[94m||||\\___ ||||||| ___/||||\033[0m" << endl;
    cout << "                           |  _ \\  / _` || '_ \\  / __| / _ \\       \033[94m|||||  |___| |||||  |||||\033[0m" << endl;
    cout << "                           | |_) || (_| || | | || (__ | (_) |      \033[94m|||||  \\_____|||||  |||||\033[0m" << endl;
    cout << "                           |____/  \\__,_||_| |_| \\___| \\___/       \033[94m|||||   \\_____||||  |||||\033[0m" << endl;
    cout << "                                                                   \033[94m|||||    \\_____|||  |||||\033[0m" << endl;
    cout << "                                                                   \033[94m|||||     \\_____||  |||||\033[0m" << endl;
    cout << "                           \033[36m_________________________________________________________________\033[0m\n" << endl;

    switch (seccion) {
        case 0:
            cout << "                                                 [ CONTROL DE ACCESO ]\n" << endl;
            break;
        case 1:
            cout << "                                                 [ CONSULTA DE SALDO ]\n" << endl;
            break;
        case 2:
            cout << "                                                 [ DEPOSITAR DINERO ]\n" << endl;
            break;
        case 3:
            cout << "                                                 [ RETIRAR EFECTIVO ]\n" << endl;
            break;
        case 4:
            cout << "                                                 [ TRANSFERIR DINERO ]\n" << endl;
            break;
        case 5:
            cout << "                                                [ RESUMEN DE LA SESION ]\n" << endl;
            break;
        case 6:
            cout << "                                                  [ CIERRE DE SESION ]\n" << endl;
            break;
        default:
            break;
    }
}

// Que hace: Muestra en pantalla el encabezado centrado y las opciones del menu.
// Que recibe (Parametros): double saldoActual (para mostrar el estado en el menu).
// Que retorna: Nada (void).
void mostrarMenu(double saldoActual) {
    mostrarEncabezado(99);
    cout << "                                        Estado: \033[92mActiva\033[0m   |   Saldo: \033[92m" << saldoActual << " Bs\033[0m\n" << endl;
    cout << "                                           \033[36m[ 1 ]\033[0m  Consultar saldo" << endl;
    cout << "                                           \033[36m[ 2 ]\033[0m  Depositar dinero" << endl;
    cout << "                                           \033[36m[ 3 ]\033[0m  Retirar efectivo" << endl;
    cout << "                                           \033[36m[ 4 ]\033[0m  Transferir a otra cuenta" << endl;
    cout << "                                           \033[36m[ 5 ]\033[0m  Resumen de la sesion\n" << endl;
    cout << "                                                   \033[36m[ 6 ]\033[0m  Salir" << endl;
}

// Que hace: Lee el PIN caracter por caracter mostrando asteriscos (*) en pantalla.
// Que recibe (Parametros): Ninguno.
// Que retorna: El PIN entero si son 4 digitos, 0 si la cantidad de digitos no es 4,
//              o -1 si el usuario escribio letras o simbolos.
int leerPINOculto() {
#ifdef _WIN32
    int cantidad = 0;
    char c1 = '0', c2 = '0', c3 = '0', c4 = '0';
    char cExtra = '0';

    while (true) {
        int tecla = _getch();

        // Tecla ENTER (13)
        if (tecla == 13) {
            if (cantidad > 0) {
                cout << endl;
                break;
            }
        }
        // Tecla BACKSPACE (8) para borrar
        else if (tecla == 8) {
            if (cantidad > 0) {
                cantidad--;
                cout << "\b \b";
            }
        }
        // Teclas especiales (flechas, F1-F12)
        else if (tecla == 0 || tecla == 224) {
            _getch();
        }
        // Caracteres imprimibles
        else if (tecla >= 32 && tecla <= 126) {
            cantidad++;
            if (cantidad == 1) c1 = (char)tecla;
            else if (cantidad == 2) c2 = (char)tecla;
            else if (cantidad == 3) c3 = (char)tecla;
            else if (cantidad == 4) c4 = (char)tecla;
            else cExtra = (char)tecla;
            cout << "*";
        }
    }

    if (cantidad >= 1 && (c1 < '0' || c1 > '9')) return -1;
    if (cantidad >= 2 && (c2 < '0' || c2 > '9')) return -1;
    if (cantidad >= 3 && (c3 < '0' || c3 > '9')) return -1;
    if (cantidad >= 4 && (c4 < '0' || c4 > '9')) return -1;
    if (cantidad >= 5 && (cExtra < '0' || cExtra > '9')) return -1;

    if (cantidad != 4) {
        return 0;
    }

    return (c1 - '0') * 1000 + (c2 - '0') * 100 + (c3 - '0') * 10 + (c4 - '0');
#else
    int pin = 0;
    cin >> pin;
    if (cin.fail() || cin.peek() != '\n') {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return pin;
#endif
}

// Que hace: Compara el PIN escrito por el usuario con el PIN constante correcto (2026).
// Que recibe (Parametros): int pinIngresado (el numero de PIN introducido).
// Que retorna: true si el PIN coincide con 2026, false en caso contrario.
bool validarPIN(int pinIngresado) {
    const int PIN_CORRECTO = 2026;
    return pinIngresado == PIN_CORRECTO;
}

// Que hace: Lee un numero entero desde el teclado y valida que no contenga letras
//           ni decimales y que se encuentre dentro del rango [min, max].
// Que recibe (Parametros): int min, int max, bool esPromptMenu (indica si usa '>' a la izquierda).
// Que retorna: El numero entero validado dentro del rango.
int leerEnteroEnRango(int min, int max, bool esPromptMenu) {
    int numero = 0;
    bool entradaValida = false;

    while (!entradaValida) {
        cin >> numero;

        if (cin.fail() || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (esPromptMenu) {
                cout << "\033[91m[!] Entrada invalida (" << min << "-" << max << ")\033[0m\n\033[36m>\033[0m ";
            } else {
                cout << "                                \033[91m[!] Entrada invalida. Ingrese un entero ("
                     << min << "-" << max << "): \033[0m";
            }
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (numero < min || numero > max) {
                if (esPromptMenu) {
                    cout << "\033[91m[!] Fuera de rango (" << min << "-" << max << ")\033[0m\n\033[36m>\033[0m ";
                } else {
                    cout << "                                \033[91m[!] Fuera de rango (" << min << "-" << max
                         << "). Intente de nuevo: \033[0m";
                }
            } else {
                entradaValida = true;
            }
        }
    }
    return numero;
}

// Que hace: Lee un monto real desde el teclado y valida que sea un numero mayor que 0.
// Que recibe (Parametros): Ninguno.
// Que retorna: El monto positivo (double) validado.
double leerMontoPositivo() {
    double monto = 0.00;
    bool entradaValida = false;

    while (!entradaValida) {
        cin >> monto;

        if (cin.fail() || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "                                \033[91m[!] Entrada invalida. Ingrese un monto numerico: \033[0m";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (monto <= 0.00) {
                cout << "                                \033[91m[!] El monto debe ser mayor que 0. Intente de nuevo: \033[0m";
            } else {
                entradaValida = true;
            }
        }
    }
    return monto;
}

// Que hace: Calcula el nuevo saldo sumando el monto depositado al saldo actual.
// Que recibe (Parametros): double saldo (saldo actual), double monto (monto a depositar).
// Que retorna: El nuevo saldo despues del deposito.
double depositar(double saldo, double monto) {
    return saldo + monto;
}

// Que hace: Verifica matematicamente si un monto es un entero multiplo exacto de 5.
// Que recibe (Parametros): double monto (cantidad que se desea evaluar).
// Que retorna: true si es multiplo de 5, false si no lo es.
bool esMultiploDeCinco(double monto) {
    return fmod(monto, 5.0) == 0.0;
}

// Que hace: Evalua si el retiro cumple todas las reglas del banco (multiplo de 5,
//           no mayor al saldo disponible y no supera el limite diario de 2000.00 Bs).
// Que recibe (Parametros): double saldo (saldo actual), double monto (monto a retirar),
//                          double retiradoHoy (acumulado retirado en la sesion).
// Que retorna: true si el retiro es valido, false si incumple alguna regla.
bool puedeRetirar(double saldo, double monto, double retiradoHoy) {
    if (!esMultiploDeCinco(monto)) {
        return false;
    }
    if (monto > saldo) {
        return false;
    }
    if ((retiradoHoy + monto) > 2000.00) {
        return false;
    }
    return true;
}

// Que hace: Imprime el mensaje especifico por el cual un retiro fue rechazado.
// Que recibe (Parametros): double saldo (saldo actual), double monto (monto intentado),
//                          double retiradoHoy (acumulado retirado en la sesion).
// Que retorna: Nada (void).
void mostrarErrorRetiro(double saldo, double monto, double retiradoHoy) {
    if (!esMultiploDeCinco(monto)) {
        cout << "                                \033[91m[!] Error: el monto debe ser multiplo de 5.\033[0m\n" << endl;
    } else if (monto > saldo) {
        cout << "                                \033[91m[!] Error: saldo insuficiente (Disponible: " << saldo << " Bs).\033[0m\n" << endl;
    } else if ((retiradoHoy + monto) > 2000.00) {
        cout << "                                \033[91m[!] Error: supera el limite diario (Disponible hoy: "
             << (2000.00 - retiradoHoy) << " Bs).\033[0m\n" << endl;
    }
}

// Que hace: Calcula el nuevo saldo restando el monto retirado al saldo actual.
// Que recibe (Parametros): double saldo (saldo actual), double monto (monto a retirar).
// Que retorna: El nuevo saldo despues del retiro.
double retirar(double saldo, double monto) {
    return saldo - monto;
}

// Que hace: Calcula el 1% de comision sobre el monto de una transferencia.
// Que recibe (Parametros): double monto (monto a transferir).
// Que retorna: El valor de la comision equivalente al 1% del monto.
double calcularComision(double monto) {
    return monto * 0.01;
}

// Que hace: Imprime el resumen detallado de las operaciones realizadas en la sesion.
// Que recibe (Parametros): Contadores de depositos, retiros y transferencias;
//                          acumuladores de montos depositados, retirados, transferidos,
//                          comisiones cobradas y el saldo final.
// Que retorna: Nada (void).
void mostrarResumen(int cantDepositos, int cantRetiros, int cantTransferencias,
                    double totalDepositado, double totalRetirado,
                    double totalTransferido, double totalComisiones,
                    double saldoFinal) {
    cout << "                                       ------------------------------------------" << endl;
    cout << "                                       Cantidad de depositos      : " << cantDepositos << endl;
    cout << "                                       Cantidad de retiros        : " << cantRetiros << endl;
    cout << "                                       Cantidad de transferencias : " << cantTransferencias << endl;
    cout << "                                       ------------------------------------------" << endl;
    cout << "                                       Total depositado           : " << totalDepositado << " Bs" << endl;
    cout << "                                       Total retirado             : " << totalRetirado << " Bs" << endl;
    cout << "                                       Total transferido          : " << totalTransferido << " Bs" << endl;
    cout << "                                       Total de comisiones (1%)   : " << totalComisiones << " Bs" << endl;
    cout << "                                       ------------------------------------------" << endl;
    cout << "                                       Saldo final en cuenta      : \033[92m" << saldoFinal << " Bs\033[0m" << endl;
    cout << "                                       ------------------------------------------" << endl;
}