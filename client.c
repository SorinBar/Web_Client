#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "utils.h"

char *host = "34.254.242.81";
int port = 8080;

char *route_register = "/api/v1/tema/auth/register";
char *route_login = "/api/v1/tema/auth/login";
char *route_enter = "/api/v1/tema/library/access";
char *route_books = "/api/v1/tema/library/books/";
char *route_logout = "/api/v1/tema/auth/logout";

char *type = "application/json";
char *cookie = NULL;
char *token = NULL;

// Send message to the server and return the response
char *HTTP_send_recv(char *message);
void cookie_free();
void token_free();
void client_register();
void client_login();
void client_enter_library();
void client_get_books();
void client_get_book();
void client_add_book();
void client_delete_book();
void client_logout();

int main(int argc, char *argv[])
{
    char cmd[50];

    while (1) {
        fgets(cmd, 50, stdin);
        cmd[strlen(cmd) - 1] = '\0';

        if (!strcmp(cmd, "exit")) {
            break;
        } else if (!strcmp(cmd, "register")) {
            client_register();
        } else if (!strcmp(cmd, "login")) {
            client_login();
        } else if (!strcmp(cmd, "enter_library")) {
            client_enter_library();
        } else if (!strcmp(cmd, "get_books")) {
            client_get_books();
        } else if (!strcmp(cmd, "get_book")) {
            client_get_book();
        } else if (!strcmp(cmd, "add_book")) {
            client_add_book();
        } else if (!strcmp(cmd, "delete_book")) {
            client_delete_book();
        } else if (!strcmp(cmd, "logout")) {
            client_logout();
        } else {
            printf("Wrong command.\n");
        }
    }

    cookie_free();
    token_free();

    return 0;
}

char *HTTP_send_recv(char *message) {
    int sockfd;
    char *response;
    sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

void cookie_free() {
    if (cookie != NULL) {
        free(cookie);
        cookie = NULL;
    }
}

void token_free() {
    if (token != NULL) {
        free(token);
        token = NULL;
    }
}

void client_register() {
    int code;
    char *message;
    char *response;
    char *body_data;
    char username[100];
    char password[100];

    printf("username=");
    fgets(username, 100, stdin);
    username[strlen(username) - 1] = '\0';

    printf("password=");
    fgets(password, 100, stdin);
    password[strlen(password) - 1] = '\0';

    // Bad input
    if (!isUsername(username) || !isPassword(password)) {
        printf("Invalid credentials.\n");
        printf("Username: Alphanumeric characters\n");
        printf("Password: No space characters\n");
        
        return;
    }

    // Create JSON
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    body_data = json_serialize_to_string(root_value);

    // Create message
    message = compute_post_request(host, route_register, type, body_data, cookie, token);

    // Server communication
    response = HTTP_send_recv(message);
    
    // Code verification
    code = extractCode(response);
    if (code == 201) {
        printf("Registered Successfully!\n");
    } else if (code == 400) {
        printf("The username is taken.\n");
    }

    // Free memory
    json_free_serialized_string(body_data);
    json_value_free(root_value);
    free(message);
    free(response);
}

void client_login() {
    int code;
    char *message;
    char *response;
    char *body_data;
    char username[100];
    char password[100];

    printf("username=");
    fgets(username, 100, stdin);
    username[strlen(username) - 1] = '\0';

    printf("password=");
    fgets(password, 100, stdin);
    password[strlen(password) - 1] = '\0';

    // Bad input
    if (!isUsername(username) || !isPassword(password)) {
        printf("Invalid credentials!\n");
        printf("Username: Alphanumeric characters\n");
        printf("Password: No space characters\n");

        return;
    }

    // Create JSON
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    body_data = json_serialize_to_string(root_value);

    // Create message
    message = compute_post_request(host, route_login, type, body_data, cookie, token);

    // Server communication
    response = HTTP_send_recv(message);

    // Code verification
    code = extractCode(response);
    if (code == 200) {
        printf("Logged in!\n");
        cookie = extractCookie(response);
    } else if (code == 204) {
        printf("Already logged in.\n");
    } else if (code == 400) {
        printf("Credentials are not good.\n");
    }

    // Free memory
    json_free_serialized_string(body_data);
    json_value_free(root_value);
    free(message);
    free(response);
}

void client_enter_library() {
    int code;
    char *message;
    char *response;

    // Create message
    message = compute_get_request(host, route_enter, NULL, cookie, token);

    // Server communication
    response = HTTP_send_recv(message);

    // Code verification
    code = extractCode(response);
    if (code == 200) {
        printf("Entered library!\n");
        token_free();
        token = extractToken(response);
    } else if (code == 401) {
        printf("Please log in.\n");
    }

    // Free memory
    free(message);
    free(response);
}

void client_get_books() {
    int code;
    char *message;
    char *response;
    
    // Create message
    message = compute_get_request(host, route_books, NULL, cookie, token);

    // Server communication
    response = HTTP_send_recv(message);

    // Code verification
    code = extractCode(response);
    if (code == 200) {
        printf("Books:\n");
        printBooks(response);
    } else if (code == 403) {
        printf("Please enter the library.\n");
    }

    // Free memory
    free(message);
    free(response);
}

void client_get_book() {
    int code;
    char *message;
    char *response;
    char id[20];
    char route_book[50];

    printf("id=");
    fgets(id, 20, stdin);
    id[strlen(id) - 1] = '\0';

    if (!isUInt(id)) {
        printf("Id is a positive integer.\n");
        
        return;
    }

    // Generate book route
    strcpy(route_book, route_books);
    strcat(route_book, id);

    // Create message
    message = compute_get_request(host, route_book, NULL, cookie, token);

    // Server communication
    response = HTTP_send_recv(message);

    // Code verification
    code = extractCode(response);    
    if (code == 200) {
        printBook(response);
    } else if (code == 403) {
        printf("Please enter the library.\n");
    } else  if (code == 404) {
        printf("Book not found.\n");
    }

    // Free memory
    free(message);
    free(response);
}

void client_add_book() {
    int code;
    char *message;
    char *response;
    char *body_data;
    char title[100];
    char author[100];
    char genre[50];
    char publisher[50];
    char page_count[10];
    int page_count_int;
    int all_valid;

    printf("title=");
    fgets(title, 100, stdin);
    title[strlen(title) - 1] = '\0';
    printf("author=");
    fgets(author, 100, stdin);
    author[strlen(author) - 1] = '\0';
    printf("genre=");
    fgets(genre, 50, stdin);
    genre[strlen(genre) - 1] = '\0';
    printf("publisher=");
    fgets(publisher, 50, stdin);
    publisher[strlen(publisher) - 1] = '\0';
    printf("page_count=");
    fgets(page_count, 10, stdin);
    page_count[strlen(page_count) - 1] = '\0';

    all_valid = isValidString(title);
    all_valid &= isValidString(author);
    all_valid &= isValidString(genre);
    all_valid &= isValidString(publisher);
    all_valid &= isUInt(page_count);

    if (!all_valid) {
        printf("Invalid input.\n");
        printf("Only {[a-z][A-Z][0-9]'\" .,!?;:} characters allowed\n");
        printf("page_count: positive integer\n");

        return;
    }
    
    // Create JSON
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    page_count_int = atoi(page_count);
    json_object_set_number(root_object, "page_count", (double)page_count_int);
    json_object_set_string(root_object, "publisher", publisher);

    body_data = json_serialize_to_string(root_value);
    // Create message
    message = compute_post_request(host, route_books, type, body_data, cookie, token);

    // Server communication
    response = HTTP_send_recv(message);

    // Code verification
    code = extractCode(response);    
    if (code == 200) {
        printf("Added book!\n");
    } else if (code == 403) {
        printf("Please enter the library.\n");
    } else if (code == 400) {
        printf("Wrong format.\n");
    }

    // Free memory
    json_free_serialized_string(body_data);
    json_value_free(root_value);
    free(message);
    free(response);
}

void client_delete_book() {
    int code;
    char *message;
    char *response;
    char id[20];
    char route_book[50];

    printf("id=");
    fgets(id, 20, stdin);
    id[strlen(id) - 1] = '\0';

    if (!isUInt(id)) {
        printf("Id is a positive integer.\n");
        
        return;
    }

    // Generate book route
    strcpy(route_book, route_books);
    strcat(route_book, id);

    // Create message
    message = compute_delete_request(host, route_book, cookie, token);

    // Server communication
    response = HTTP_send_recv(message);

    // Code verification
    code = extractCode(response);    
    if (code == 200) {
        printf("Book deleted!\n");
    } else if (code == 403) {
        printf("Please enter the library.\n");
    } else  if (code == 404) {
        printf("Book not found.\n");
    }

    // Free memory
    free(message);
    free(response);
}

void client_logout() {
    int code;
    char *message;
    char *response;

    // Create message
    message = compute_get_request(host, route_logout, NULL, cookie, token);

    // Server communication
    response = HTTP_send_recv(message);

    // Code verification
    code = extractCode(response);    
    if (code == 200) {
        printf("Logged out!\n");
        cookie_free();
        token_free();
    } else  if (code == 400) {
        printf("You are not logged in.\n");
    }

    // Free memory
    free(message);
    free(response);
}