#ifndef UTILS_H
#define UTILS_H
    
    #include <string.h>
    #include <ctype.h>
    #include <stdlib.h>
    #include <math.h>
    #include "requests.h"
    #include "parson.h"

    void eerror(char *message) {
        printf("%s\n", message);
        exit(EXIT_FAILURE);
    }

    int isUsername(char *str) {
        int len = strlen(str);
        for (int i = 0; i < len; i++) {
            if (!isalnum(str[i]))
                return 0;
        }
        return 1;
    }

    int isPassword(char *str) {
        int len = strlen(str);
        for (int i = 0; i < len; i++) {
            if (str[i] == ' ')
                return 0;
        }
        return 1;
    }

    int isUInt(char *str) {
        int len = strlen(str);
        for (int i = 0; i < len; i++) {
            if (!isdigit(str[i]))
                return 0;
        }
        return 1;
    }

    int isValidChar(char c) {
        static char *allowedChr = " .,!?'\";:";
        if (isalnum(c) || strchr(allowedChr, c) != NULL ) {
            return 1;
        } else {
            return 0;
        }
    }

    int isValidString(char *str) {
        int len = strlen(str);
        for (int i = 0; i < len; i++) {
            if (!isValidChar(str[i]))
                return 0;
        }
        return 1;
    }

    int extractCode(char *response) {
        return atoi(strchr(response, ' ') + 1);
    }

    char *extractCookie(char *response) {
        char *p;
        char *cookie;
        
        response = strstr(response, "Set-Cookie: ");
        response += strlen("Set-Cookie: ");
        p = strchr(response, ';');
        *p = '\0';
        cookie = malloc((strlen(response) + 1) * sizeof(char));
        if (cookie == NULL)
            eerror("malloc error.");
        strcpy(cookie, response);
        *p = ';';

        return cookie;
    }

    char *extractToken(char *response) {
        JSON_Value* root_value = json_parse_string(basic_extract_json_response(response));
        if (root_value == NULL) {
            eerror("JSON parse error\n");
        }
        JSON_Object* root_object = json_value_get_object(root_value);
        const char* const_token = json_object_get_string(root_object, "token");
        char *token = malloc((strlen(const_token) + 1) * sizeof(char));
        strcpy(token, const_token);
        json_value_free(root_value);
        
        return token;
    }

    void printBooks(char *response) {
        JSON_Value* root_value = json_parse_string(basic_extract_json_response(response));
        if (root_value == NULL) {
            eerror("JSON parse error\n");
        }

        JSON_Array* books = json_value_get_array(root_value);
        size_t count = json_array_get_count(books);

        for (size_t i = 0; i < count; i++) {
            JSON_Object* book = json_array_get_object(books, i);

            int id = (int)(round(json_object_get_number(book, "id")));
            printf("id: %d, ", id);
            const char *title = json_object_get_string(book, "title");
            printf("title: %s\n", title);
        }

        // Free memory
        json_value_free(root_value);
    }

    void printBook(char *response) {
        int number;
        const char *str;
        JSON_Value* root_value = json_parse_string(basic_extract_json_response(response));
        if (root_value == NULL) {
            eerror("JSON parse error\n");
        }
        
        JSON_Object* book = json_value_get_object(root_value);
        number = (int)(round(json_object_get_number(book, "id")));
        printf("id: %d\n", number);
        str = json_object_get_string(book, "title");
        printf("title: %s\n", str);
        str = json_object_get_string(book, "author");
        printf("author: %s\n", str);
        str = json_object_get_string(book, "publisher");
        printf("publisher: %s\n", str);
        str = json_object_get_string(book, "genre");
        printf("genre: %s\n", str);
        str = json_object_get_string(book, "author");
        printf("author: %s\n", str);
        number = (int)(round(json_object_get_number(book, "page_count")));
        printf("page_count: %d\n", number);

        // Free memory
        json_value_free(root_value);
    }

#endif