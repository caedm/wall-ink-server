
//program to check that the data from the server is correct
//
//
//TODO:
//generate pbm file for image
//or display with ascii
//

//wall-ink-server data checking program
//
//purpose: verify if data from a wall-ink-server host is valid
//
//usage: ./check_wall_ink_data.bin                     Run with default settings
//   or: ./check_wall_ink_data.bin [options] <args>    Run with given options and arguments
//
//The program expects the data to be passed to it via Standard Input (STDIN) typically through a pipe
//
//options:
//    -h:         Display Help and Usage
//    -d:         Run in Debug mode
//    -b: <bytes>     Set the input buffer/data size to # <bytes>
//    -i: <key>   Set the image key to that given in <key>
//    -v:         Run in Verbose mode
//

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>

#include "sha1.h"

//amount of bytes to have as the default buffer size for
//data coming from stdin
#define DEFAULT_BUFF_SIZE 50000

#ifndef DEFAULT_IMAGE_KEY
//checks if a default image key has been defined elsewhere 
//if not it defines it here
#define DEFAULT_IMAGE_KEY "hunter2"
#endif

//size in bytes of the head data (hash)
#define HEAD_DATA_SIZE 20
//size of a hash in bytes
#define HASH_SIZE 20
//size in bytes of the combined time data
#define TIME_DATA_SIZE 8
//size in bytes of one part of the combined time data
#define TIME_DATA_PART_SIZE 4

//return values
#define RET_OK          0
#define RET_ERROR       1
#define RET_CRITICAL    2

//twenty spaces, for use in output formatting in debug
#define SPACE_20 "                    "

//buffer for output strings
#define OUTPUT_STR_BUFF 128

//initialize the buff_size to the default
int buff_size = DEFAULT_BUFF_SIZE;

//blank string which we will use to store the program name
char * prog_name = NULL;

//variable flag to store if in debug mode or not
enum {
    DEBUG_OFF = 0,
    DEBUG_ON = 1,
} debug_mode = DEBUG_OFF;

//flag for verbose mode
bool verbose = false;

//prototype funtion for processing the data
int process_data(const char * image_key, 
                    const uint8_t * data, 
                    int data_size);

//helper function to generate hex string of hash data
char * get_hex_string(uint8_t * out_string, 
                        const uint8_t * data, 
                        uint16_t data_size);

//function to print a hex string of given data to standard output
int print_hex_string(const uint8_t * data, uint16_t data_size);

//function to convert data to C string format with '\0' at the end
char * get_raw_string(uint8_t * out_string, 
                        const uint8_t * data, 
                        uint16_t data_size);



//initialize the main()
//
//
int main(int argc, char **argv){

    //variable to store the program name
    prog_name = argv[0];

    //variable to store the image key
    char * image_key = NULL;
    //variable to store the default value 
    char default_image_key[] = DEFAULT_IMAGE_KEY;

    //variable to store argument characters
    int c;
    //set opterr to zero before parsing operators
    opterr = 0;

    //operator parsing code
    while((c = getopt(argc, argv, ":i:hb:dv")) != -1){
        
        //variable to store value from input
        int temp_value = 0;

        //check all the options
        switch(c){
            case 'i':
                //if given -i arg, take in the value after as the image key
                image_key = optarg;
                break;
            case 'b':
                //if given -b arg, take in the value after as the buffer size
                //
                //if the argument correctly reads in as a decimal number
                //doesn't give NULL result or zero characters
                if(sscanf(optarg,"%d", &temp_value) > 0){
                    //make sure it is a positive value
                    if(temp_value > 0){
                        //store the decimal as the input buffer size
                        buff_size = temp_value;
                    } else {
                        //print out error if it has a bad value
                        fprintf(stderr, "%s ERROR: "
                                "input buffer size %d is zero or negative."
                                "using default buffer size of %d\n",
                                prog_name, temp_value, buff_size);
                    }
                }
                break;
            case 'd':
                //for debug output
                //set debug_mode to DEBUG_ON
                debug_mode = DEBUG_ON; 
                break;
            case 'h':
                //print help
                printf("HELP:\n"
                        "wall-ink-server data checking program\n"
                        "\n"
                        "purpose: verify if data from a wall-ink-server host is valid\n"
                        "\n"
                        "usage: %s \t\t\tRun with default settings\n"
                        "   or: %s [options] <args> \tRun with given options and arguments\n"
                        "\n"
                        "The program expects the data to be passed to it via "
                        "Standard Input (STDIN) typically through a pipe\n"
                        "\n"
                        "options:\n"
                        "\t-h: \t\tDisplay Help and Usage\n"
                        "\t-d: \t\tRun in Debug mode\n"
                        "\t-b: <bytes> \tSet the input buffer/data size to # <bytes>\n"
                        "\t-i: <key> \tSet the image key to that given in <key>\n"
                        "\t-v: \t\tRun in Verbose mode\n",
                        prog_name, prog_name);
                return 0;
                break;
            case 'v':
                //set verbose to true
                verbose = true;
                break;
            case ':':
                //if it is missing an argument
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                return RET_ERROR; 
                break;
            case '?':
                //if invalid option
                if(isprint(optopt)){
                    fprintf(stderr, "Invalid option '-%c' \n", optopt);
                } else {
                    fprintf(stderr, 
                            "Invalid option character '\\x%x' \n", optopt);
                } 
                return RET_ERROR;
                break;

            default:
                abort();
        }
    } 

    //after done parsing args, 
    //if image_key is still NULL then set with default value
    if(image_key == NULL){
        image_key = default_image_key; 
    }

    if(debug_mode || verbose){
        //for debug, print the buff_size value
        printf("buff_size = %d\n", buff_size);
    }

    //make a buffer variable to store the incoming data
    uint8_t buff[buff_size];

    //get data from stdin

    //variable to store total number of characters read
    int total_n = 0;

    //READ IN DATA CODE
    //
    //variable to store character temporarily
    int in_c;
    //loop and get input until EOF or max data reached
    //using fgetc() to go one unsigned char at a time
    while(((in_c = fgetc(stdin)) != EOF) && (total_n < buff_size)){
        //write the input char into the input buffer
        buff[total_n] = (uint8_t) in_c;
        //increment the total input characters/bytes number
        total_n++; 
    }
    
    if(debug_mode || verbose){
        printf("total read %d bytes\n", total_n);
        printf("size of input bytes: %lu \n", sizeof(buff));
    }
    if(debug_mode){
        //write the data to stdio for testing
        printf("input content = \n");
        int write_n = write(STDOUT_FILENO, buff, buff_size);
        printf("\nwrote %d bytes\n", write_n); 
    }

    //check the data
    //and store any error codes
    int ret_code = process_data(image_key, buff, buff_size);
    //return the error code which then gets passed to the shell or script
    return ret_code;
}



//function to process the data
int process_data(const char * image_key, 
                    const uint8_t * data, 
                    int data_size){

    //storage variables
    SHA1Context sha;
    int err;

    //get the head data hash (20 bytes)
    uint8_t head_data[HEAD_DATA_SIZE];
    memcpy(head_data, data, HEAD_DATA_SIZE);

    if(debug_mode){
        //variable for raw string to pass through get_raw_string() function
        uint8_t * raw_str = NULL;

        //print the raw head data
        printf("head data raw: \n%s\n", get_raw_string(raw_str, 
                                                head_data, sizeof(head_data)));
        //free the raw_str after the function used malloc on it.
        free(raw_str);

        //print head data in hex 
        printf("head data hex: \t\t"); 
        //print the hex string
        print_hex_string(head_data, HEAD_DATA_SIZE);
        //newline
        printf("\n");
    }

    //to store the image key hash
    uint8_t image_key_hash[HASH_SIZE];

    //get the image key hash
    err = SHA1Reset(&sha);

    //***
    //this ignores this NULL at the end of the image_key string, 
    //but as the wall-ink-server code is now, it hashes with the ending NULL
    //so this code is not needed. But will keep it here just in case.
    //This may change later.
    //***
    //err = SHA1Input(&sha, (uint8_t *) image_key, strlen(image_key));
    
    //since the current wall-ink-server code hashes the string with the NULL
    //then use this code that does the same
    err = SHA1Input(&sha, (uint8_t *) image_key, strlen(image_key) + 1);

    //put the hash into image_key_hash
    err = SHA1Result(&sha, image_key_hash);

    if(debug_mode){
        //print out the hash of the image key
        printf("image key hash (hex): \t" SPACE_20 SPACE_20);
        //print the hex string
        print_hex_string(image_key_hash, HASH_SIZE);
        //newline 
        printf("\n");
    }

    //get the time data
    uint8_t time_data[TIME_DATA_SIZE];
    memcpy(time_data, data + HASH_SIZE, TIME_DATA_SIZE);

    if(debug_mode){  
        //set raw_str to NULL 
        uint8_t * raw_str = NULL;
        //print out raw time data 
        printf("time data raw: \n%s\n", get_raw_string(raw_str, time_data, 
                                                        sizeof(time_data)));
        //free the raw_str after the function used malloc on it.
        free(raw_str);
    }

    //to store the time data hash
    uint8_t time_data_hash[HASH_SIZE];

    //get hash of time data 
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t *) time_data, TIME_DATA_SIZE);
    err = SHA1Result(&sha, time_data_hash);

    //get the time when data was compressed
    uint8_t compressed_time[TIME_DATA_PART_SIZE];
    memcpy(compressed_time, time_data, TIME_DATA_PART_SIZE);

    //get the time to next wake up
    uint8_t next_time[TIME_DATA_PART_SIZE];
    memcpy(next_time, time_data + TIME_DATA_PART_SIZE, TIME_DATA_PART_SIZE);

    //get time_t variables of the time
    //casting to uint32_t*  seems to work fine for this
    time_t compressed_time_t = (time_t)*((uint32_t *) compressed_time);
    time_t next_time_t = (time_t)*((uint32_t *) next_time);

    //get ascii format of the time, store in strings 
    //this keeps the values because if not stored in strings
    //when you call localtime() it uses an internal pointer which
    //can be overwritten with multiple calls.
    char compressed_str[OUTPUT_STR_BUFF];
    strcpy(compressed_str, asctime(localtime(&compressed_time_t)));
    char next_str[OUTPUT_STR_BUFF];
    strcpy(next_str, asctime(localtime(&next_time_t)));

    if(debug_mode || verbose){
        //print out time info
        printf("time when compressed: \t%s", compressed_str);
        printf("time to next wake: \t%s", next_str);
    }
    if(debug_mode){
        //print out the hash of the time data
        printf("time data hash (hex): \t");
        //print the hex string
        print_hex_string(time_data_hash, HASH_SIZE);
        //newline 
        printf("\n");
    }

    //
    //next put the two hashes together, then hash that to get the final hash
    //
    
    //combine the two hashes
    uint8_t combined_hashes[HASH_SIZE * 2];
    //copy the time data hash into the lower bytes
    memcpy(combined_hashes, time_data_hash, HASH_SIZE);
    //copy the image key hash into the upper bytes
    memcpy(combined_hashes + HASH_SIZE, image_key_hash, HASH_SIZE);

    //variable to store the final hash of the time hash and image key hash
    uint8_t final_time_hash[HASH_SIZE]; 

    //hash the combined hashes
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t *) combined_hashes, HASH_SIZE * 2);
    if(err){
        fprintf(stderr, "%s ERROR %d: error in SHA1Input function on"
                        " combined_hashes\n", prog_name, err);
    }
    err = SHA1Result(&sha, final_time_hash);
    if(err){
        fprintf(stderr, "%s ERROR %d: error in SHA1Result function on"
                        " final_time_hash\n", prog_name, err);
    }
   
    if(debug_mode){
        //print the combined hashes
        printf("combined hashes: \t");
        print_hex_string(combined_hashes, HASH_SIZE * 2);
        printf("\n");

        //print the final hash
        printf("final time hash (hex): \t");
        print_hex_string(final_time_hash, HASH_SIZE);
        printf("\n");
    }

    //test if the final hash matches the head data
    if(memcmp(head_data, final_time_hash, HASH_SIZE) != 0){
        //if they don't match print an error
        fprintf(stderr, "%s ERROR: final time hash is not matching" 
                        " the hash in the head data\n", prog_name);
        //return error critical
        return RET_CRITICAL;
    } else {
        //if they match print output
        if(verbose || debug_mode){
            printf("success! final time hash matches the hash in the head data!\n");
        }
    }

    //nothing fails, return OK 
    return RET_OK; 
}


//function to print a hex string of given data to standard output
int print_hex_string(const uint8_t * data, uint16_t data_size){
    //variable to store printf output
    int err; 
    //string pointer to pass to get_hex_string
    uint8_t * hex_str = NULL;
    //get the string and print it
    hex_str = (uint8_t *) get_hex_string(hex_str, data, data_size);
    err = printf("%s", hex_str);
    //free the pointer data to prevent leaks, as it was malloc'd in get_hex_string
    free(hex_str);
    //return whatever printf's output was
    return err;
} 

//function to that returns a string of the hexadecimal format of given data
char * get_hex_string(uint8_t * out_string, 
                        const uint8_t * data, 
                        uint16_t data_size){
    //variable to store output string, size is double that of
    //the data size because for each byte there are two hex characters
    //then add one for the ending NULL
    uint16_t out_size = (data_size * 2) + 1;
    //malloc the space needed
    out_string = (uint8_t *) malloc(out_size);
    //copy over blank string to clear data in case of memory overwrite
    //fixed a bug in output before
    strcpy((char *)out_string, ""); 
    //loop through 
    for(int c=0; c < data_size; c++){
        //variable to store sprintf output
        char tmp_str[] = "__";
        //get hex of one byte
        //%02hhx for 2 digits, 8-bit input, output as hex
        sprintf(tmp_str, "%02hhx", (uint8_t)(data[c]));
        //concatenate on to the output string
        strcat((char *)out_string, tmp_str);
    }
    
    return (char *)out_string;
}

//function to convert data to string format with '\0' at the end
char * get_raw_string(uint8_t * out_string, 
                        const uint8_t * data, 
                        uint16_t data_size){
    //variable for the string size
    uint16_t out_size = data_size + 1;
    //make the output string variable
    out_string = (uint8_t *) malloc(out_size);

    //copy data over
    memcpy(out_string, data, data_size);
    //append the ending '\0'
    out_string[out_size] = '\0';
    //return the string
    return (char *)out_string; 
}
