#ifndef UTILS_H
#define UTILS_H
    
    #include <string.h>
    #include <ctype.h>
    #include <stdlib.h>

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




    
#endif