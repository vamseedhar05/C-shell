#include "headers.h"

void strip_tags(char *html) {
    int inside_tag = 0;
    int i,j;
    for (i = 0, j = 0; i < strlen(html); i++) {
        if (html[i] == '<') {
            inside_tag = 1;
        } else if (html[i] == '>') {
            inside_tag = 0;
        } else if (!inside_tag) {
            html[j++] = html[i];
        }
    }
    html[j] = '\0';
}

void fetch_man_page(const char *command_name) {
    // Step 1: DNS resolution for man.he.net
    struct hostent *host_info;
    host_info = gethostbyname("man.he.net");
    if (host_info == NULL) {
        fprintf(stderr, "DNS resolution failed for man.he.net\n");
        return;
    }

    // Step 2: Open a TCP socket to the IP address
    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr = *((struct in_addr *)host_info->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    // Step 3: Send a GET request to the website's server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        return;
    }

    char request[1024];
    sprintf(request, "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_name);

    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
        close(sockfd);
        return;
    }

    // Step 4: Read and print the body of the website
    char response[8192];
    int bytes_received;
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';
        strip_tags(response);
        printf("%s", response);
    }

    // Step 5: Close the socket
    close(sockfd);
}