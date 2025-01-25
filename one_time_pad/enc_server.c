/***********************************************
 * File Name:
 * Date: 12/7/2024
 * File Description:
 * Author(s): Ivan Wong
 ***********************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
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


void setup_address(struct sockaddr_in* address, 
                        int port_number){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(port_number);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}


char* encrypt_message(char* plain_text, char* key) {
    int length = strlen(plain_text);
    char* cypher_text = (char*) calloc(length, sizeof(char));
    int plain_value = 0;
    int key_value = 0;
    int cypher_value = 0;

    int i = 0;
    // Encrypt message using plain text and key
    for (i = 0; i < length; i++) {
        // Check for spaces
        plain_value = (plain_text[i] == ' ') ? 26 : plain_text[i] - 'A';
        key_value = (key[i] == ' ') ? 26 : key[i] - 'A';
        cypher_value = (plain_value + key_value) % 27;

        // Encrypt message
        cypher_text[i] = (cypher_value == 26) ? ' ' : 'A' + cypher_value;
    }
    return cypher_text;
}


int split_message(char* message, char** plain_text, char** key) {
    char* delimiter = strstr(message, "^");

    if (delimiter == NULL)
        return 1;

    *delimiter = '\0';
    *plain_text = message;
    *key = delimiter + 1;
    return 0;
}


int send_message(int socket, const char* message) {
    int total_sent = 0;
    int message_length = strlen(message);

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
    char client_type[4];

    // Get client type
    int source_received = recv(socket, client_type, 4, 0);

    if (source_received < 0)
        return NULL;

    // ENC client seen, return
    if (strcmp(client_type, "DEC") == 0)
        return "DEC";

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

        if (chars_read < 0) {
            free(message);
            return NULL;
        }        
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
    int connection_socket;
    int listen_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t size_of_client_info = sizeof(client_address);

    int port_number;
    char* plain_text = NULL;
    char* key = NULL;
    char* cypher_text = NULL;
    char* received_message = NULL;

    // Check for valid args
    if (argc < 2)
        error("ENC_SERVER Error: Not enough arguments");

    // Setup socket and port number
    port_number = atoi(argv[1]);
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_socket < 0)
        error("ENC_SERVER Error: Couldn't open socket");

    setup_address(&server_address, port_number);

    // Bind socket and port
    if ((bind(listen_socket,
        (struct sockaddr*) &server_address,
         sizeof(server_address))) < 0)
         error("ENC_SERVER Error: Couldn't bind");

    // Listen for up to 5 connections
    listen(listen_socket, 5);

    while (1) {
        connection_socket = accept(listen_socket,
                                  (struct sockaddr*) &client_address,
                                   &size_of_client_info);
        if (connection_socket < 0)
            error("ENC_SERVER Error: Couldn't accept socket");

        pid_t pid = fork();

        // Error forking
        if (pid < 0)
            error("ENC_SERVER Error: Cannot fork process");

        // Child process handles encryption
        else if (pid == 0) {
            // Receive full message
            if ((received_message = receive_message(connection_socket)) == 0) {
                free_data(received_message, NULL, NULL);
                error("ENC_SERVER Error: Cannot read from socket");
            }

            else if (strcmp(received_message, "DEC") == 0)
                error("ENC_SERVER Error: Cannot connect to decryption client");

            // Process message
            if (split_message(received_message, &plain_text, &key) != 0) {
                free_data(received_message, NULL, NULL);
                error("ENC_SERVER Error: Text or key not provided");
            }
            cypher_text = encrypt_message(plain_text, key);

            // Send data
            if (send_message(connection_socket, cypher_text) != 0) {
                free_data(received_message, NULL, NULL);
                error("ENC_SERVER Error: Cannot write to socket");
            }

            // Done handling client, free memory and exit
            free_data(received_message, cypher_text, NULL);
            close(connection_socket);
            exit(EXIT_SUCCESS);
        }
        else {
            pid = wait(NULL);
            close(connection_socket);
        }
    }
    close(listen_socket);
    return 0;
}