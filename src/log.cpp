#include "log.hpp"
#include "xor.hpp"

static byte buf[32];                            // file buffer for reading/storing contents
static char log_fname[13];                      // Buffer containing the "8.3" file naming format
static File f_log, f_boot;                      // Handles to the Log File and Boot ID file
static uint32_t id_boot, id_part;               // store the current invocation's ID information
static size_t log_bytes = LOG_BYTE_LIMIT;       // Keep track of upper limit of remaining bytes

/*
 * A function to set TeenyLogger's "log" module default filename
 */
static void tl_log_set_filename(void){
    memset(log_fname, 0, sizeof(log_fname));
    snprintf(log_fname, sizeof(log_fname), "%04lx_%04lx.TL", id_boot, id_part);
}


/*
 * (Internal Use Only)
 * Close the current logfile, increase the part number, and start a new logfile
 */
static bool tl_log_rotate(void){
    Serial.println("[#] LOG_ROTATE()");
    // Close existing file if it is open
    if(f_log){
        f_log.close();
    }

    // Reset log bytes and increase part #
    log_bytes = LOG_BYTE_LIMIT;
    id_part++;
    // set the filename and replace f_log
    tl_log_set_filename();
    f_log = SD.open(log_fname, FILE_WRITE);
    return !!f_log;
}

/*
 * Log a single byte to the log file
 */
bool tl_log_byte(const byte datum){
    if(!log_bytes--){
        if(!tl_log_rotate()){
            return false;
        }
    }
    f_log.write(datum ^ tl_xor_key_nxt());
    return true;
}

/*
 * Write the provided data to the current log_fname
 */
bool tl_log_data(const byte *data, size_t size){
    if(size > log_bytes){
        tl_log_rotate();
    }
    for(size_t i = 0; i < size; ++i){
        if(!tl_log_byte(data[i])){
            return false;
        }
    }
    return true;
}

/*
 * Initialize the LOG module
 */
bool tl_log_init(int chip) {
    Serial.println("[#] LOG_INIT()");
    size_t bytes_read, bytes_writ;

    // Initialize SD card with the provided chip
    if(!SD.begin(chip)){
        Serial.println("[!] Could not start SD card");
        return false;
    }
    Serial.println("[#] Started SD Card");

    // Open the file containing the boot ID
    f_boot = SD.open(FNAME_BOOT, FILE_WRITE);
    if(!f_boot){
        Serial.println("[!] Could not open file BOOT.ID");
        return false;
    }
    Serial.printf("[#] " FNAME_BOOT " is %u bytes\n", f_boot.size());
    f_boot.seek(SEEK_SET);

    // read the contents of the Boot ID file
    if(!(bytes_read = f_boot.read(buf, sizeof(buf)))){
        id_boot = 0;
        Serial.println("[#] Created Boot ID 0");
    } else {
        // parse the contents as a hexadecimal string
        if(sscanf((const char*)buf, "%lx", &id_boot) != 1){
            Serial.printf("[!] Could not parse " FNAME_BOOT ": '%s'\n", (const char*)buf);
            return false;
        }

        // This is a new boot sequence (a novel or rebooted host) so increase Boot ID
        id_boot += 1;
        Serial.printf("[#] Loaded from " FNAME_BOOT ": %lx\n", id_boot);
    }
    // overwrite the original file (the ID will never be shorter)
    f_boot.seek(SEEK_SET);
    bytes_writ = f_boot.print(id_boot, HEX);

    // We are done with the file
    Serial.printf("[#] Wrote %u bytes to " FNAME_BOOT "\n", bytes_writ);
    f_boot.close();

    // set the id_part to 0 and trigger a initial filename rotation
    id_part = 0;
    tl_log_set_filename();
    Serial.println(log_fname);
    return false;
}
