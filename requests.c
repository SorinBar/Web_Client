#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char *cookie, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3: add cookies
    if (cookie != NULL) {
        sprintf(line, "Cookie: %s;", cookie);
        compute_message(message, line);
    }

    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }
    // Tell the server to not keep the socket alive
    sprintf(line, "Connection: close");
    compute_message(message, line);

    // Step 4: add final new line
    compute_message(message, "");

    // Free line
    free(line);

    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                            char *cookie, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", strlen(body_data));
    compute_message(message, line);

    // Step 4: add cookies
    if (cookie != NULL) {
        sprintf(line, "Cookie: %s;", cookie);
        compute_message(message, line);
    }

    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }
    // Tell the server to not keep the socket alive
    sprintf(line, "Connection: close");
    compute_message(message, line);

    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    strcat(message, body_data);

    // Free line
    free(line);

    return message;
}


char *compute_delete_request(char *host, char *url, char *cookie, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookie != NULL) {
        sprintf(line, "Cookie: %s;", cookie);
        compute_message(message, line);
    }

    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // Tell the server to not keep the socket alive
    sprintf(line, "Connection: close");
    compute_message(message, line);

    compute_message(message, "");

    // Free line
    free(line);

    return message;
}