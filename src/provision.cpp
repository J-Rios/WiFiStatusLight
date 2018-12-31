/**************************************************************************************************/
// Project: WiFiStatusLight
// File: provision.h
// Description: System provisioning functinality based in WiFi AP and WebServer
// Created on: 31 dec. 2018
// Last modified date: 31 dec. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

#include "provision.h"

/**************************************************************************************************/

/* Data Types */


/**************************************************************************************************/

/* Functions */

// Create/Load persistent SPIFFS config file from/to system parameters
void launch_provision(Globals* Global)
{
    // To-Do

    Global->set_first_boot_provision(false);
}
