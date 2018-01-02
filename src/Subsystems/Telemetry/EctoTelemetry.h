//
// Created by Abiel on 12/12/17.
//

/*
 * Settings
 * Group: Main (settings.ini)
 * TelemetryNetworkTableName: Name of network table to publish general telemetry
 * DrivetrainSpecificTelemetryNetworkTableName:  Name of network table to publish drivetrain specific telemetry
 *
 * Group: Telemetry (drivetrain settings file)
 * pdpId: PDP CAN Id
 */

#ifndef BOTBUSTERSREBIRTH_ECTOTELEMETRY_H
#define BOTBUSTERSREBIRTH_ECTOTELEMETRY_H

class EctoTelemetry {
public:
	//Telemetry which normally should only be updated once
	virtual void sendInitialTelemetry() const = 0;
	
	//Telemetry which should be updated periodically
	virtual void sendTelemetry() const = 0;
	
	void enableTelemetry() const;
	
	void disableTelemetry() const;
	
	bool getTelemetryStatus() const;

private:
	mutable bool enabled = false;
};


#endif //BOTBUSTERSREBIRTH_ECTOTELEMETRY_H
