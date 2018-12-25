/**************************************************************************************************/
// Project: WiFiStatusLight
// File: configuration.h
// Description: Device configuration and persistent parameters save/load functions file
// Created on: 25 dec. 2018
// Last modified date: 25 dec. 2018
// Version: 0.0.1
/**************************************************************************************************/

/* Libraries */

#include "configuration.h"

/**************************************************************************************************/

/* Data Types */


/**************************************************************************************************/

/* Functions */

// Create/Load persistent SPIFFS config file from/to system parameters
void device_config_init(SimpleSPIFFS* SPIFFS, Globals* Global)
{
    cJSON* json_default_config = NULL;
    cJSON* json_actual_config = NULL;
    char* cstr_default_config = NULL;
    uint8_t error;

    // Get default device config (parameters-values) JSON object
    error = get_json_str_default_config(json_default_config, cstr_default_config);
    if(error != 0)
    {
        debug("Error when generating default configs: ");
        if(error == 1)
            debug("Can't create JSON object.\n");
        else if(error == 2)
            debug("Can't parse JSON object to C string.\n");
        else
            debug("Unexpected error.");
        
        return;
    }

    // If config file doesnt exists in SPIFFS
    if(!SPIFFS->file_exists(SPIFFS_CONFIG_FILE))
    {
        // SPIFFS doesnt has any config file, create it with default configs
        debug("Persistent SPIFFS JSON device config file doesn't found. Creating a new one with " \
              "default values...\n");
        if(!SPIFFS->file_write(SPIFFS_CONFIG_FILE, cstr_default_config))
        {
            debug("SPIFFS config file can't be created.\n");
            debug("Rebooting the system...\n");
            esp_restart();
        }
        debug("Config file with default values successfully created.\n");
    }
    else
    {
        // SPIFFS has a config file, lets read it
        char cstr_actual_config[MAX_SPIFFS_FILE_CONTENT];
        memset(cstr_actual_config, '\0', MAX_SPIFFS_FILE_CONTENT);
        if(!SPIFFS->file_read(SPIFFS_CONFIG_FILE, cstr_actual_config))
        {
            debug("SPIFFS config file can't be read.\n");
            debug("Rebooting the system...\n");
            esp_restart();
        }

        // Get JSON object of actual device configuration
        const char** return_parse_end = NULL;
        debug("\n\n%s.\n\n", cstr_actual_config);
        json_actual_config = cJSON_ParseWithOpts(cstr_actual_config, return_parse_end, 1);
        if(json_actual_config == NULL)
        {
            debug("Can't parse JSON data from actual config C string.\n");
            debug("Error before: %s\n", *return_parse_end);
            debug("Rebooting the system...\n");
            esp_restart();
        }

        /* Load actual device configuration to system
           Regenerate each missing config parameter (key-value pair) in persistent config file */
        bool any_missing_param = false;

        // Parameter: wifi_ssid
        cJSON* wifi_ssid = NULL;
        wifi_ssid = cJSON_GetObjectItemCaseSensitive(json_actual_config, "wifi_ssid");
        if(cJSON_IsString(wifi_ssid) && (wifi_ssid->valuestring != NULL))
        {
            Global->set_wifi_ssid(wifi_ssid->valuestring);
            debug("WiFi SSID parameter successfully load from persistent config file.\n");
        }
        else
        {
            debug("WiFi SSID parameter not found inside persistent config file, regenerating...");
            char wifi_ssid_val[MAX_LENGTH_WIFI_SSID+1];
            Global->get_wifi_ssid(wifi_ssid_val);
            cJSON_AddStringToObject(json_actual_config, "wifi_ssid", wifi_ssid_val);
            any_missing_param = true;
        }

        // Parameter: wifi_pass
        cJSON* wifi_pass = NULL;
        wifi_pass = cJSON_GetObjectItemCaseSensitive(json_actual_config, "wifi_pass");
        if(cJSON_IsString(wifi_pass) && (wifi_pass->valuestring != NULL))
        {
            Global->set_wifi_pass(wifi_pass->valuestring);
            debug("WiFi PASS parameter successfully load from persistent config file.\n");
        }
        else
        {
            debug("WiFi PASS parameter not found inside persistent config file, regenerating...");
            char wifi_pass_val[MAX_LENGTH_WIFI_PASS+1];
            Global->get_wifi_pass(wifi_pass_val);
            cJSON_AddStringToObject(json_actual_config, "wifi_pass", wifi_pass_val);
            any_missing_param = true;
        }

        // Parameter: internet_check_url
        cJSON* ping_check_url = NULL;
        ping_check_url = cJSON_GetObjectItemCaseSensitive(json_actual_config, "internet_check_url");
        if(cJSON_IsString(ping_check_url) && (ping_check_url->valuestring != NULL))
        {
            Global->set_internet_check_url(ping_check_url->valuestring);
            debug("Internet check ping URL parameter successfully load from persistent config file.\n");
        }
        else
        {
            debug("Internet check ping URL parameter not found inside persistent config file, regenerating...");
            char ping_check_url_val[MAX_LENGTH_IPV4+1];
            Global->get_internet_check_url(ping_check_url_val);
            cJSON_AddStringToObject(json_actual_config, "internet_check_url", ping_check_url_val);
            any_missing_param = true;
        }

        // Parameter: firmware_ver
        cJSON* firmware_ver = NULL;
        firmware_ver = cJSON_GetObjectItemCaseSensitive(json_actual_config, "firmware_ver");
        if(cJSON_IsString(firmware_ver) && (firmware_ver->valuestring != NULL))
        {
            Global->set_firmware_version(firmware_ver->valuestring);
            debug("Firmware version parameter successfully load from persistent config file.\n");
        }
        else
        {
            debug("Firmware version parameter not found inside persistent config file, regenerating...");
            char firmware_ver_val[MAX_LENGTH_WIFI_SSID+1];
            Global->get_firmware_version(firmware_ver_val);
            cJSON_AddStringToObject(json_actual_config, "internet_check_url", firmware_ver_val);
            any_missing_param = true;
        }

        // If there was any missing parameter, overwrite config file with regenerated ones
        if(any_missing_param)
        {
            // Get C string representation of created JSON
            char* cstr_actual_config_read;
            cstr_actual_config_read = NULL;
            cstr_actual_config_read = cJSON_Print(json_default_config);
            if(cstr_actual_config_read != NULL)
            {
                debug("Rewriting SPIFFS JSON device config file with missing parameters...\n");
                if(!SPIFFS->file_write(SPIFFS_CONFIG_FILE, cstr_actual_config_read))
                {
                    debug("SPIFFS config file can't be overwrite.\n");
                    debug("Rebooting the system...\n");
                    esp_restart();
                }
                debug("Config file successfully overwritten.\n");
            }
            else
                debug("Can't parse JSON object to C string.\n");
        }

        // Free JSON resources (JSON object and all subobjects)
        cJSON_Delete(wifi_ssid);
        cJSON_Delete(wifi_pass);
        cJSON_Delete(ping_check_url);
        cJSON_Delete(firmware_ver);
    }

    // Free JSON resources (JSON object and all subobjects)
    cJSON_Delete(json_default_config);
    cJSON_Delete(json_actual_config);
}

// Get corresponding C string from default device config JSON representation
uint8_t get_json_str_default_config(cJSON* json_default_config, char* cstr_json)
{
    uint8_t rc = 0;

    // Set input arguments to NULL
    json_default_config = NULL;
    cstr_json = NULL;

    // Default device config (parameters-values) JSON object
    json_default_config = cJSON_CreateObject();
    if(json_default_config != NULL)
    {
        // Add default parameters to JSON object
        cJSON_AddStringToObject(json_default_config, "wifi_ssid", DEFAULT_WIFI_SSID);
        cJSON_AddStringToObject(json_default_config, "wifi_pass", DEFAULT_WIFI_PASS);
        cJSON_AddStringToObject(json_default_config, "internet_check_url", PING_TO_URL);
        cJSON_AddStringToObject(json_default_config, "firmware_ver", DEFAULT_FIRMWARE_VERSION);

        // Get C string representation of created JSON
        cstr_json = NULL;
        cstr_json = cJSON_Print(json_default_config);
        if(cstr_json == NULL)
            rc = 2;
    }
    else
        rc = 1;

    return rc;
}
