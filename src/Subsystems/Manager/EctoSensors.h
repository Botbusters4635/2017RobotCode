#include <IterativeRobot.h>
#include <Joystick.h>
#include <LiveWindow/LiveWindow.h>
#include <RobotDrive.h>
#include <Timer.h>
//#include <I2C.h>
#include <iostream>
#include <DigitalOutput.h>
#include <SerialPort.h>

class EctoSensors {
public:
	EctoSensors();
	
	void TakeValues(SerialPort &puertoSerial);
	
	int GetValues(int indice);
	
	//void CantidadSensores (int NumeroSensores);



private:
	int estado = 0;
	int i, j, t, recibido;
	char buffer[255];
	char *bufferPtr;
	char byte;
	static const int cantidadSensores = 2;
	uint16_t lecturasSensores[cantidadSensores];
	
	
};

    