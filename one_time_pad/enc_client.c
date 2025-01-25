/***********************************************
 * File Name: enc_client.c
 * Date: 12/7/2024
 * File Description: Program connects to encryption
 *                   server and asks it to perform
 *                   one time pad encryption.
 * Author(s): Ivan Wong
 ***********************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#ifndef CHUNK_SIZE_
#define CHUNK_SIZE 1000
#endif

// Error handle
void error(const char* message) {
    fprintf(stderr, "%s\n", message);
    fflush(stderr);
    exit(EXIT_SUCCESS);
}


char* read_file(const char* file_name) {
    // Open file and error handle
    FILE* file = fopen(file_name, "r");
    int file_length;
    char* buffer;

    if (file == NULL)
        error("ENC_CLIENT Error: Unable to open file");

    // Get length of file
    fseek(file, 0, SEEK_END);
    file_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Create buffer and check memory allocation
    buffer = (char*) calloc(file_length + 1, sizeof(char));

    if (buffer == NULL)
        error("ENC_CLIENT Error: Memory allocation failed");

    fread(buffer, 1, file_length, file);

    // Close file
    fclose(file);

    // Get rid of new line and replace with null char
    buffer[strcspn(buffer, "\n")] = '\0';

    return buffer;
}


void setup_address(struct sockaddr_in* address,
                   int port_number,
                   char* host_name) {

    // Clear address struct, make it network capable, store port
    memset((char*) address, '\0', sizeof(*address));
    address->sin_family = AF_INET;
    address->sin_port = htons(port_number);

    // Get DNS entry
    struct hostent* host_info = gethostbyname(host_name);

    if (host_info == NULL)
        error("ENC_CLIENT Error: No such host\n");

    // Copy IP address from DNS to address
    memcpy((char*) &address->sin_addr.s_addr,
           host_info->h_addr_list[0],
           host_info->h_length);

    return;
}


int check_bad_characters(const char* text) {
    int length = strlen(text);

    int i = 0;
    for (i = 0 ; i < length; i++) {
        // Skip spaces
        if (text[i] == ' ')
            continue;

        // Check for invalid char
        if (text[i] < 'A' || text[i] > 'Z')
            return 1;
    }
    return 0;
}


int send_message(int socket, const char* message) {
    int total_sent = 0;
    int message_length = strlen(message);

    // Send noti to server
    int source_sent = send(socket, "ENC", 4, 0);

    if (source_sent < 0)
        return 1;

    int size_sent = send(socket, &message_length, sizeof(int), 0);

    if (size_sent < (int) sizeof(int))
        return 1;

    // Keep sending until full message sent
    while (total_sent < message_length) {
        // Calculate number of bytes needed by chunks
        int sent_bytes = (message_length - total_sent > CHUNK_SIZE) ? CHUNK_SIZE : (message_length - total_sent);

        // Send and error handle
        int chars_sent = send(socket, message + total_sent, sent_bytes, 0);

        if (chars_sent < 0)
            return 1;

        // Update total chars
        total_sent += chars_sent;
    }
    return 0;
}


char* receive_message(int socket) {
    int total_received = 0;
    int message_length = 0;
    char* message = NULL;

    // Get length of message first
    int bytes_read = recv(socket, &message_length, sizeof(int), 0);

    if (message_length < (int) sizeof(int) || bytes_read < 0)
        return NULL;

    // Allocate memory for message
    message = (char*) calloc(message_length + 1, sizeof(char));

    if (message == NULL)
        return NULL;

    // Keep receiving until full message obtained
    while (total_received < message_length) {
        int read_bytes = (message_length - total_received > CHUNK_SIZE) ? CHUNK_SIZE : (message_length - total_received);

        int chars_read = recv(socket, message + total_received, read_bytes, 0);

        if (chars_read < 0)
            return NULL;
        
        // Update total chars
        total_received += chars_read;
    }
    return message;
}


void free_data(char* data_one, char* data_two, char* data_three) {
    if (data_one != NULL)
        free(data_one);
    if (data_two != NULL)
        free(data_two);
    if (data_three != NULL)
        free(data_three);
    return;
}


int main(int argc, char** argv) {
    // Check for valid num of args
    if (argc < 4)
        error("ENC_CLIENT Error: Not enough argumentss");

    int socket_fd;
    struct sockaddr_in server_address;

    // Get data from command line args
    int port_number = atoi(argv[3]);
    char* plain_text = read_file(argv[1]);
    char* key = read_file(argv[2]);
    char* message = NULL;

    // Check for valid key size
    if (strlen(plain_text) > strlen(key)) {
        free_data(plain_text, key, NULL);
        error("ENC_CLIENT Error: Key is too short for plain text");
    }
    // Check for bad chars
    if (check_bad_characters(plain_text) != 0) {
        free_data(plain_text, key, NULL);
        error("ENC_CLIENT Error: Plain text contains invalid characters");
    }
    // Combine message with a delimiter ^
    message = (char*) calloc(strlen(plain_text) + strlen(key) + 2, sizeof(char));

    if (message == NULL) {
        free_data(plain_text, key, NULL);
        error("ENC_CLIENT Error: Memory allocation failed");
    }

    sprintf(message, "%s^%s", plain_text, key);

    // Create and error handle socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        free_data(plain_text, key, message);
        error("ENC_CLIENT Error: Cannot open socket");
    }
    setup_address(&server_address, port_number, "localhost");

    // Connect and error handle server connection
    if (connect(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        free_data(plain_text, key, message);
        error("ENC_CLIENT Error: Cannot connect to server");
    }
    // Send message to server and error handle
    if (send_message(socket_fd, message) != 0) {
        free_data(plain_text, key, message);
        error("ENC_CLIENT Error: Cannot write to socket");
    }
    // Reset message
    memset(message, '\0', strlen(message));

    // Get encrypted message
    if ((message = receive_message(socket_fd)) == NULL) {
        free_data(plain_text, key, message);
        error("ENC_CLIENT Error: Cannot read from socket");
    }
    // Print cypher text to stdout
    fprintf(stdout, "%s\n", message);
    fflush(stdout);

    // Free memory and close socket
    free_data(plain_text, key, message);
    close(socket_fd);

    return 0;
}
