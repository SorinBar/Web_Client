# Web_Client

**Server Ip:** 34.254.242.81
**Server Port:** 8080

**Setup:**
-   ```
    make
    ```
    
**Commands:**
- register
- login
- enter_library
- get_books
- get_book
- add_book
- delete_book
- logout

**Implementation:**
- Sockets:
    - The client opens a socket for every message exchange with the server.
    - I used the "Connection: close" header, so the server won't keep its socket open, consuming resources.
    - The reasoning behind this is simplicity.
    - In this way, we don't need to monitor if the socket is still connected.
    - Also,  the rate at which packets are sent is low (one in a few seconds), so opening a socket once in a while is a better solution for the server as it doesn't require constant monitoring over a socket.

- JSON:
    - I chose the parson library because the skeleton for the client was written in C.
    - Create a JSON ex:
        ```
        JSON_Value *root_value = json_value_init_object();
        JSON_Object *root_object = json_value_get_object(root_value);

        json_object_set_string(root_object, "title", "Book1");
        json_object_set_number(root_object, "price", 9.99);

        serialized_json = json_serialize_to_string(root_value);
        ```
    - Parse a JSON ex:
        ```
        JSON_Value* root_value = json_parse_string(serialized_json);
        JSON_Object* root_object = json_value_get_object(root_value);

        const char* title = json_object_get_string(root_object, "title");
        double price = json_object_get_number(root_object, "price");
        ```

- Code:
    - Main functions:
        -   ```
            char *HTTP_send_recv(char *message);
            
            /*  Info:
                -> Opens socket 
                -> Connects with the server
                -> Sends message
                -> Returns the server response
            */
            ```
    
        -   ```
            void cookie_free();
            
            /*  Info:
                -> Checks if the cookie is NULL (no cookie)
                -> Free if there is a cookie
            */
            ```
        -   ```
            void token_free();
            
            /*  Info:
                -> Checks if the token is NULL (no token)
                -> Free if there is a token
            */
            ```
        -   ```
            void client_register();
            void client_login();
            void client_enter_library();
            void client_get_books();
            void client_get_book();
            void client_add_book();
            void client_delete_book();
            void client_logout();
            
            /*  Info:
                -> Functions for every user command
            */
            ```
        -   ```
            char *compute_get_request(char *host, char *url, char *query_params, char *cookie, char *token);
            
            /*  Info:
                -> Computes and returns a GET request string
            */
            ```
        -   ```
            char *compute_post_request(char *host, char *url, char* content_type,char *body_data, char *cookie, char *token);
            
            /*  Info:
                -> Computes and returns a POST request string
            */
            ```
        -   ```
            char *compute_delete_request(char *host, char *url, char *cookie, char *token);
            
            /*  Info:
                -> Computes and returns a DELETE request string
            */
            ```
    - Flow
        - While the command is not exit
            - Reads user input (command).
            - Calls the function for the given command, or notifies the user in the case of an invalid command.
            - Generates messages based on the command and input.
            - Sends the message to the server.
            - Receives response.
            - Parse response.
            - In case of an error alerts the user.
    
    - **Note:** If a user introduces an invalid input, the commands need to be recalled. The client app won't wait for valid input to be introduced.
 