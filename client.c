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
#include "parson.h"
#include "utils.h"

char *host = "34.254.242.81";
int port = 8080;

char *route_register = "/api/v1/tema/auth/register";
char *route_login = "/api/v1/tema/auth/login";
char *route_enter = "/api/v1/tema/library/access";

char *type = "application/json";
char *cookie = NULL;
char *token = NULL;

// Send message to the server and return the response
char *HTTP_send_recv(char *message);
void cookie_free();
void client_register();
void client_login();
void client_enter_library();

int main(int argc, char *argv[])
{
    char cmd[50];

    while (1) {
        fgets(cmd, 50, stdin);
        cmd[strlen(cmd) - 1] = '\0';
        // Exit
        if (!strcmp(cmd, "exit")) {
            break;
        }
        // Register
        if (!strcmp(cmd, "register")) {
            client_register();
        }
        // Login
        if (!strcmp(cmd, "login")) {
            client_login();
        }
        // Enter library
        if (!strcmp(cmd, "enter_library")) {
            client_enter_library();
        }
    }

    cookie_free();

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

void client_register() {
    int code;
    char *message;
    char *response;
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

    char *body_data = json_serialize_to_string(root_value);

    // Create message
    message = compute_post_request(host, route_register, type, body_data, cookie);

    // Server communication
    response = HTTP_send_recv(message);

    code = extractCode(response);
    if (code == 201) {
        printf("Registered Successfully!\n");
    }
    if (code == 400) {
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

    char *body_data = json_serialize_to_string(root_value);

    // Create message
    message = compute_post_request(host, route_login, type, body_data, cookie);

    // Server communication
    response = HTTP_send_recv(message);

    code = extractCode(response);
    if (code == 200) {
        printf("Logged in!\n");
        cookie = extractCookie(response);
        printf("Cookie: %s\n", cookie);
    }
    if (code == 204) {
        printf("Already logged in.\n");
    }
    if (code == 400) {
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
    message = compute_get_request(host, route_enter, NULL, cookie);

    // Server communication
    response = HTTP_send_recv(message);

    printf("%s\n", response);

    code = extractCode(response);
    if (code == 200) {
        printf("Entered library!\n");

        JSON_Value* root_value = json_parse_string(basic_extract_json_response(response));
        JSON_Object* root_object = json_value_get_object(root_value);
        const char* token = json_object_dotget_string(root_object, "token");

        printf("token: %s\n", token);
        json_value_free(root_value);
    }
    if (code == 401) {
        printf("You are not logged in.\n");
    }

    // Free memory
    free(message);
    free(response);
}