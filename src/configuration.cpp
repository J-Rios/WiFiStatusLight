/**************************************************************************************************/
// Project: WiFiStatusLight
// File: configuration.h
// Description: Device configuration and persistent parameters save/load functions file
// Created on: 25 dec. 2018
// Last modified date: 31 dec. 2018
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
    uint8_t error;
    cJSON* json_default_config = NULL;
    cJSON* json_actual_config = NULL;
    char cstr_default_config[MAX_SPIFFS_FILE_CONTENT+5];

    // Get default device config (parameters-values) JSON object
    memset(cstr_default_config, '\0', MAX_SPIFFS_FILE_CONTENT);
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
        debug("Persistent config file doesn't found. Creating a new one with " \
              "default values...\n");
        if(!SPIFFS->file_write(SPIFFS_CONFIG_FILE, cstr_default_config))
        {
            debug("SPIFFS config file can't be created.\n");
            debug("Rebooting the system...\n");
            esp_restart();
        }
        debug("Config file successfully created.\n");
    }
    else
    {
        // SPIFFS has a config file, lets read it
        char cstr_actual_config[MAX_SPIFFS_FILE_CONTENT];
        memset(cstr_actual_config, '\0', MAX_SPIFFS_FILE_CONTENT);
        debug("Persistent config file found. Loading data...\n");
        if(!SPIFFS->file_read(SPIFFS_CONFIG_FILE, cstr_actual_config))
        {
            debug("SPIFFS config file can't be read.\n");
            debug("Rebooting the system...\n");
            esp_restart();
        }

        // Get JSON object of actual device configuration
        const char** return_parse_end = NULL;
        json_actual_config = cJSON_ParseWithOpts(cstr_actual_config, return_parse_end, 1);
        if(json_actual_config == NULL)
        {
            debug("Can't parse JSON data from actual config C string.\n");
            debug("Error before: %s\n", return_parse_end[0]);
            debug("Rebooting the system...\n");
            esp_restart();
        }

        // Load actual device configuration to system
        bool any_missing_param = load_device_data(Global, json_actual_config);
        
        // If there was any missing parameter, overwrite config file with regenerated ones
        if(any_missing_param)
        {
            // Get C string representation of created JSON
            char cstr_actual_config_read[MAX_SPIFFS_FILE_CONTENT+5];
            memset(cstr_actual_config_read, '\0', MAX_SPIFFS_FILE_CONTENT);
            if(cJSON_PrintPreallocated(json_actual_config, cstr_actual_config_read, 
               MAX_SPIFFS_FILE_CONTENT, 1))
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

        debug("\n");
    }

    // Free JSON resources (JSON object and all subobjects)
    cJSON_Delete(json_default_config);
    cJSON_Delete(json_actual_config);
}

// Get corresponding C string from default device config JSON representation
uint8_t get_json_str_default_config(cJSON* json_default_config, char* cstr_json)
{
    uint8_t rc = 0;

    // Default device config (parameters-values) JSON object
    json_default_config = NULL;
    json_default_config = cJSON_CreateObject();
    if(json_default_config != NULL)
    {
        // Add default parameters to JSON object
        cJSON_AddTrueToObject(json_default_config, "first_boot_provision");
        cJSON_AddStringToObject(json_default_config, "wifi_ssid", DEFAULT_WIFI_SSID);
        cJSON_AddStringToObject(json_default_config, "wifi_pass", DEFAULT_WIFI_PASS);
        cJSON_AddStringToObject(json_default_config, "internet_check_url", PING_TO_URL);
        cJSON_AddStringToObject(json_default_config, "firmware_ver", DEFAULT_FIRMWARE_VERSION);

        // Get C string representation of created JSON
        if(!cJSON_PrintPreallocated(json_default_config, cstr_json, MAX_SPIFFS_FILE_CONTENT, 1))
            rc = 2;
    }
    else
        rc = 1;

    return rc;
}

// Load actual persistent device configuration to system data
// Regenerate each missing config parameter (key-value pair) in persistent config file
bool load_device_data(Globals* Global, cJSON* json_actual_config)
{
    bool any_missing_param = false;
    
    // Parameter: first_boot_provision
    cJSON* first_boot_provision = NULL;
    first_boot_provision = cJSON_GetObjectItemCaseSensitive(json_actual_config, 
                                                            "first_boot_provision");
    if(cJSON_IsBool(first_boot_provision))
    {
        if(cJSON_IsFalse(first_boot_provision))
            Global->set_first_boot_provision(false);
        else if(cJSON_IsTrue(first_boot_provision))
            Global->set_first_boot_provision(true);
        debug("First boot provision parameter successfully load from persistent config file.\n");
    }
    else
    {
        debug("First boot provision parameter not found inside persistent config file, " \
              "regenerating...\n");
        bool first_boot_provision = true;
        Global->get_first_boot_provision(first_boot_provision);
        if(first_boot_provision)
            cJSON_AddTrueToObject(json_actual_config, "first_boot_provision");
        else
            cJSON_AddFalseToObject(json_actual_config, "first_boot_provision");
        any_missing_param = true;
    }

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
        debug("WiFi SSID parameter not found inside persistent config file, regenerating...\n");
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
        debug("WiFi PASS parameter not found inside persistent config file, regenerating...\n");
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
        debug("Internet check URL parameter successfully load from persistent config file.\n");
    }
    else
    {
        debug("Internet check URL parameter not found inside persistent config file, " \
                "regenerating...\n");
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
        debug("Firmware version parameter not found inside persistent config file, " \
                "regenerating...\n");
        char firmware_ver_val[MAX_LENGTH_WIFI_SSID+1];
        Global->get_firmware_version(firmware_ver_val);
        cJSON_AddStringToObject(json_actual_config, "internet_check_url", firmware_ver_val);
        any_missing_param = true;
    }

    return any_missing_param;
}

// Save provided parameter value into device persistent config data (save to SPIFFS config file)
bool spiffs_save_parameter_char(const char* parameter, const char* value)
{
    bool fail = false;

    

    return fail;
}

// Save provided parameter value into device persistent config data (save to SPIFFS config file)
bool spiffs_save_parameter_bool(const char* parameter, const bool value)
{
    bool fail = false;



    return fail;
}
