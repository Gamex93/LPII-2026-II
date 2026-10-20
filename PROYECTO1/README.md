# LPII-2026-II
## **INTEGRANTES:**
- JEAN FRANCO  C.I. (33.863.924)
- LUIS TORRES C.I. (33.165.266)
## **MODELO A: CAJERO AUTOMATICO**
 Ingresas el PIN (El PIN es: 2026), una vez ingresado el PIN correcto
 se le llevará al menu de operacion, para escoger un apartado, ingresa
 el numero correspondiente y presiona ENTER. Por ejemplo: para consultar el saldo presiona [1], para hacer un deposito presiona [2].
 ## **CASOS DE PRUEBA**
 1. PIN incorrecto 3 veces (debe bloquear) y PIN correcto al segundo intento:
 El programa pasó esta prueba

 2. Letras en el menú y en un monto:
 El programa pasó esta prueba, cuando se ponen letras, el sistema le dice al usuario que porfavor ingrese numeros

 3. Depósito de 0, de -50 y de 10 000.01:
 El programa paso esta prueba, al colocar cualquiera de estos en el monto del deposito, el sistema dirá que el monto es invalido

 4. Retiro de 503 (no es múltiplo de 5), retiro mayor al saldo y retiros que sumen más de 2000:
 El programa pasó esta prueba, si el retiro es mayor al saldo el sistema dirá: "Saldo insuficiente" y tambien el sistema notificara cuando se exceda el limite diario de 2000
 5.  Transferencia a una cuenta de 7 dígitos y transferencia cuyo monto más comisión supera el saldo: El programa pasó esta prueba.

 6. Una sesión completa normal verificando que el saldo final del resumen sea correcto: El programa pasó esta prueba
