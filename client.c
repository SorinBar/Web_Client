#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);

        
    // Ex 1.1: GET dummy from main server
    message = compute_get_request("34.254.242.81", "/api/v1/dummy", NULL, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s", response);
    char request[] = "GET /api/v1/dummy HTTP/1.1\r\n"
                 "Host: 34.254.242.81:8080\r\n"
                 "Connection: close\r\n\r\n";
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    printf("%s", response);
    close_connection(sockfd);
    

    char test[1111];
    scanf("%s", test);

    // send a message to a server
    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s", response);
    close_connection(sockfd);

    // Ex 1.2: POST dummy and print response from main server
    // Ex 2: Login into main server
    // Ex 3: GET weather key from main server
    // Ex 4: GET weather data from OpenWeather API
    // Ex 5: POST weather data for verification to main server
    // Ex 6: Logout from main server
    // BONUS: make the main server return "Already logged in!"



    // free the allocated data at the end!

    return 0;
}
