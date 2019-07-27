#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    //freopen("input.txt","r",stdin);
    char *input_line= NULL;
    size_t size = 0;

    while (getline(&input_line, &size, stdin) != -1) {
        //REMOVES LAST CHARACTER (NEWLINE) FROM INPUT_LINE
        unsigned long num = strlen(input_line);
        input_line[num-1] = '\0';

        char *delimiter;
        delimiter = " ";
        char *token;
        token = strtok(input_line, delimiter);
        if (token != NULL) {
            int input_line_word_length;
            if (strcmp(token, "addent") == 0 || strcmp(token, "delent") == 0) {
                input_line_word_length = 2;
            }
            else if (strcmp(token, "addrel") == 0 || strcmp(token, "delrel") == 0) {
                input_line_word_length = 4;
            }
            else {
                input_line_word_length = 1;
            }
            char *input_divider[input_line_word_length];
            input_divider[0] = token;

            for (int i = 1; i<input_line_word_length; i++) {
                token = strtok(NULL, delimiter);
                if(token != NULL) {
                    input_divider[i] = token;
                }
            }
            printf("%d ", input_line_word_length);
            printf("%s\n", input_divider[0]);
        }
    }
    free(input_line);

    return 0;
}