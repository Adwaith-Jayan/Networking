#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define FTP_PORT 21
#define BUFFER_SIZE 1024

void send_command(int sock, const char *cmd) {
    send(sock, cmd, strlen(cmd), 0);
    printf("Sent: %s", cmd);
}

void receive_response(int sock) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("Received: %s", buffer);
    }
}

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(FTP_PORT);
    server.sin_addr.s_addr = inet_addr("192.168.1.1"); // Change to your FTP server IP

    // Connect to FTP server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Receive initial FTP server response
    receive_response(sock);

    // Send USER command
    send_command(sock, "USER anonymous\r\n");
    receive_response(sock);

    // Send PASS command (anonymous login)
    send_command(sock, "PASS guest\r\n");
    receive_response(sock);

    // Send PASV command (Passive mode for data transfer)
    send_command(sock, "PASV\r\n");
    receive_response(sock);

    // Send LIST command to get directory listing
    send_command(sock, "LIST\r\n");
    receive_response(sock);

    // Close socket
    close(sock);
    return 0;
}
