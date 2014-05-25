/*
 * telemetry_MSP.c
 *
 *  Created on: 22 Apr 2014
 *      Author: trey marc
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "drivers/serial_common.h"
#include "serial_common.h"

#include "telemetry_common.h"
#include "serial_msp.h"

static telemetryConfig_t *telemetryConfig;

#define MSP_TELEMETRY_BAUDRATE 19200 // TODO make this configurable
#define MSP_TELEMETRY_INITIAL_PORT_MODE MODE_TX

static serialPort_t *mspTelemetryPort;

static portMode_t previousPortMode;
static uint32_t previousBaudRate;

void initMSPTelemetry(telemetryConfig_t *initialTelemetryConfig)
{
    telemetryConfig = initialTelemetryConfig;
}

void handleMSPTelemetry(void)
{
    sendMspTelemetry();
}

void freeMSPTelemetryPort(void)
{
    // FIXME only need to reset the port if the port is shared
    serialSetMode(mspTelemetryPort, previousPortMode);
    serialSetBaudRate(mspTelemetryPort, previousBaudRate);

    endSerialPortFunction(mspTelemetryPort, FUNCTION_TELEMETRY);
}

void configureMSPTelemetryPort(void)
{
    mspTelemetryPort = findOpenSerialPort(FUNCTION_TELEMETRY);
    if (mspTelemetryPort) {
        previousPortMode = mspTelemetryPort->mode;
        previousBaudRate = mspTelemetryPort->baudRate;

        //waitForSerialPortToFinishTransmitting(mspTelemetryPort); // FIXME locks up the system

        serialSetBaudRate(mspTelemetryPort, MSP_TELEMETRY_BAUDRATE);
        serialSetMode(mspTelemetryPort, MSP_TELEMETRY_INITIAL_PORT_MODE);
        beginSerialPortFunction(mspTelemetryPort, FUNCTION_TELEMETRY);
    } else {
        mspTelemetryPort = openSerialPort(FUNCTION_TELEMETRY, NULL, MSP_TELEMETRY_BAUDRATE, MSP_TELEMETRY_INITIAL_PORT_MODE, SERIAL_NOT_INVERTED);

        // FIXME only need these values to reset the port if the port is shared
        previousPortMode = mspTelemetryPort->mode;
        previousBaudRate = mspTelemetryPort->baudRate;
    }
}

uint32_t getMSPTelemetryProviderBaudRate(void) {
    return MSP_TELEMETRY_BAUDRATE;
}

