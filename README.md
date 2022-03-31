# Proyecto de ejemplo iOS

Este proyecto de ejemplo permite la comunicación con el POS Bluetooth. El ejemplo contiene una función para calcular el LRC de los comandos y permite probar todas las operaciones del POS.

## Requisitos

- El proyecto fue creado utilizando Xcode 13.3
- iOS 10.0 o superior.

## Dependencias

Puedes descargar la librería desde la web de [Transbank developers](https://www.transbankdevelopers.cl/documentacion/pos-bluetooth#descarga-de-librerias).

### Framework iSMP(PCL)

Este framework es necesario para establecer la comunicación Bluetooth entre el terminal de pago (Link2500) y el smartphone.

### Framework Mpos Integrado

Este framework es necesario para iniciar la transacción y capturar la respuesta.

## Ejecutar ejemplo

Es importante mencionar que este proyecto solo puede ser probado utilizando un dispositivo real.

Para ejecutar el proyecto se debe seleccionar el dispositivo donde se va a probar el proyecto de ejemplo.

Posteriormente se debe debe dar click en el ícono de run. Esto comenzará a compilar el proyecto y a instalarlo en el dispositivo.

Es probable que si se ejecuta la primera vez, muestre un error de certificado y no pueda ejecutar la aplicación. **Para poder solucionar esto, se debe ir al menú de configuración del dispositivo -> General -> Admón. de dispositivos y VPN**. Ahí se mostrará dentro de la App del desarrollador el proyecto de ejemplo y se debe autorizar el certificado. Con lo anterior ya se podrá ejecutar la aplicación.
