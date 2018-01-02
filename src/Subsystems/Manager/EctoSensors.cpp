#include "EctoSensors.h"

using namespace std;

EctoSensors::EctoSensors() = default;

void EctoSensors::TakeValues(SerialPort &puertoSerial) {
	
	switch (estado) {
		case 0:
			bufferPtr = buffer;
			recibido = puertoSerial.Read(bufferPtr, 255);
			if (recibido > 0) {
				estado = 1;        //Se recibio una line con mas de 1 caracter
				j = 0;            //Contador de datos recibidos
			}
			break;
		
		case 1:    //Voy a buscar el caracter de inicio
			i = 0;
			byte = buffer[i++];
			while ((byte != 0x02) && (byte != '\n') && (i < 255)) {
				byte = buffer[i++];
			}
			if (byte == 0x02) {
				estado = 2;    //Recibi el caracter de inicio
			} else {
				estado = 0;    //No encontre el caracter de inicio en la linea recibia
			}
			break;
		
		case 2:    //Encontre el caracter de inicio, voy a buscar el dato y meterlo en lecturasSensores[j]
			t = 0;
			byte = buffer[i++];
			while ((byte != ',') && (byte != '\n') && (i < 255)) {
				byte = buffer[i++];
				t++;
			}
			if (t == 2) {    //Se encontraron 2 bytes
				estado = 3;    //Se recibieron dos bytes entre los separadores, el dato esta en buffer[i-2]:buffer[i-1]
			} else {
				estado =
						0; //Se recibieron mas de dos bytes, o se llego al tope, se descarta;	//Se recibieron dos bytes entre los separadores, el dato esta en buffer[i-2]:buffer[i-1]
			}
			break;
		
		case 3:    //Se detectaron dos bytes con datos, vienen en buffer[i-2]:buffer[i-1]
			lecturasSensores[j] = buffer[i - 3];
			lecturasSensores[j] = lecturasSensores[j] << 8;
			lecturasSensores[j] = lecturasSensores[j] | buffer[i - 2];
			j++;
			if (j == cantidadSensores) {
				estado = 0;
			} else {
				estado = 2;
			}
			break;
		default:
			estado = 0;
	}
}

int EctoSensors::GetValues(int indice) {
	if (indice < cantidadSensores) {
		return lecturasSensores[indice];
	} else {
		return -1;
	}
	
}