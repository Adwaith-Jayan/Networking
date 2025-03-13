#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <dirent.h>

#define FTP_PORT 21
#define DATA_PORT 2020  // Port for passive mode
#define BUFFER_SIZE 1024

void send_response(int client_sock, const char *message) {
    send(client_sock, message, strlen(message), 0);
    printf("Sent: %s", message);
}

void handle_list_command(int client_sock) {
    int data_sock;
    struct sockaddr_in data_addr;
    char buffer[BUFFER_SIZE];

    // Create a data socket for the LIST command
    data_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (data_sock == -1) {
        perror("Data socket creation failed");
        return;
    }

    data_addr.sin_family = AF_INET;
    data_addr.sin_addr.s_addr = INADDR_ANY;
    data_addr.sin_port = htons(DATA_PORT);

    if (bind(data_sock, (struct sockaddr *)&data_addr, sizeof(data_addr)) < 0) {
        perror("Data socket bind failed");
        return;
    }

    listen(data_sock, 1);
    send_response(client_sock, "150 Opening data connection for directory list.\r\n");

    int data_client = accept(data_sock, NULL, NULL);
    if (data_client < 0) {
        perror("Data connection failed");
        return;
    }

    // Get directory listing
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(".")) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            snprintf(buffer, sizeof(buffer), "%s\r\n", entry->d_name);
            send(data_client, buffer, strlen(buffer), 0);
        }
        closedir(dir);
    }

    close(data_client);
    close(data_sock);

    send_response(client_sock, "226 Directory send OK.\r\n");
}

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    
    // Send welcome message
    send_response(client_sock, "220 FTP Server Ready.\r\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        printf("Received: %s", buffer);

        if (strncmp(buffer, "USER", 4) == 0) {
            send_response(client_sock, "331 User name okay, need password.\r\n");
        } else if (strncmp(buffer, "PASS", 4) == 0) {
            send_response(client_sock, "230 Login successful.\r\n");
        } else if (strncmp(buffer, "PASV", 4) == 0) {
            send_response(client_sock, "227 Entering Passive Mode (127,0,0,1,7,228).\r\n"); // 7*256+228 = 2020
        } else if (strncmp(buffer, "LIST", 4) == 0) {
            handle_list_command(client_sock);
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            send_response(client_sock, "221 Goodbye.\r\n");
            break;
        } else {
            send_response(client_sock, "500 Unknown command.\r\n");
        }
    }

    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(FTP_PORT);

    // Bind server to port 21
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Start listening for connections
    listen(server_sock, 5);
    printf("FTP Server running on port %d...\n", FTP_PORT);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }
        printf("Client connected.\n");

        if (fork() == 0) { // Handle client in a separate process
            close(server_sock);
            handle_client(client_sock);
            exit(0);
        }
        close(client_sock);
    }

    close(server_sock);
    return 0;
}
