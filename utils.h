#ifndef UTILS_H
#define UTILS_H
    
    #include <string.h>
    #include <ctype.h>
    #include <stdlib.h>
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
        JSON_Object* root_object = json_value_get_object(root_value);
        const char* const_token = json_object_dotget_string(root_object, "token");
        char *token = malloc((strlen(const_token) + 1) * sizeof(char));
        strcpy(token, const_token);
        json_value_free(root_value);
        
        return token;
    }




    
#endif