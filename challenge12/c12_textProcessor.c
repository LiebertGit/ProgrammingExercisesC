#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef struct TextNode {
    char *text;
    struct TextNode *prev;
    struct TextNode *next;
} TextNode;

TextNode *parseText(char *input);
void freeList(TextNode *head);

int main (){

    char *input = NULL;
    char line[2000];
    size_t inputLength = 0;

    printf("Enter your text (2000 char buffer).\n");
    printf("Linux/macOS: Ctrl+D when finished.\n");
    printf("Windows: Ctrl+Z, then Enter when finished.\n\n");

    while(fgets(line, sizeof(line), stdin) != NULL){
        
        size_t lineLength = strlen(line); 
        
        char *temp = realloc(input, inputLength + lineLength +1);
        
        if(temp == NULL){
            free(input);
            return 1;
        }

        input = temp;

        memcpy(input + inputLength, line, lineLength);

        inputLength += lineLength;

        input[inputLength] = '\0';
}
    printf("\n");

    if (input == NULL) {
        return 0;
    }

    TextNode *head = parseText(input);

    TextNode *current = head; 

    while(current != NULL){
        printf("\n");
        printf("%s\n", current->text);

        TextNode *next = current->next;

        free(current->text);
        free(current);

        current = next;
    }

    free(input);
   
    return 0;
}

TextNode *parseText(char *input){
    
    char *start = input;
    TextNode *head = NULL;
    TextNode *tail = NULL;

    for (char *c = input; *c != '\0'; c++){
        if (*c == '.' || *c == '!' ||
            *c == '?' || *c == ';'){
            
            size_t length = c - start +1;

            TextNode *node = malloc(sizeof(TextNode));
            
            if(node == NULL){
                return NULL;
            }

            node->text = malloc(length +1);

            if(node->text == NULL){
                free(node);
                return NULL;
            }

            memcpy(node->text, start, length);
            node->text[length] = '\0';

            node->prev = NULL;
            node->next = NULL;

            if(head == NULL){
                head = node;
            } else {
                tail->next = node;
                node->prev = tail;
            }
            tail = node;

            start = c + 1;

            while (isspace((unsigned char)*start)) {
                start++;
            }
        }
    }

    if (*start != '\0') {
        size_t length = strlen(start);

        TextNode *node = malloc(sizeof(TextNode));

        if(node == NULL) {
            return NULL;
        }

        node->text = malloc(length +1);

        if(node->text == NULL) {
            free(node);
            return NULL;
        }

        memcpy(node->text, start, length);
        node->text[length] = '\0';

        node->prev = NULL;
        node->next = NULL;

        if(head == NULL){
            head = node;
        } else {
            tail->next = node;
            node->prev = tail;
        }
        tail = node;
    }

    return head;
}

void freeList(TextNode *head) {
    TextNode *current = head;

    while (current != NULL){
        TextNode *next = current->next;

        free(current->text);
        free(current);

        current = next;
    }
}
