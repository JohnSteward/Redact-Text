#include <stdio.h>
#include <stdlib.h>

//Finds the size of the text file so I can allocate memory accordingly
int sizeOfFile(const char *filename){
    FILE *inp = fopen(filename, "r");
    char c;
    int i = 0;
    if (inp) {
        c = fgetc(inp);
        while (!feof(inp)){
            //putchar(c);
            i++;
            c = fgetc(inp);
        }
        fclose(inp);
    }

    return i;
}

//Reads all of the characters in the text file and puts them into a list.
const char* readFromFile(const char *filename, char *list){
    FILE *inp = fopen(filename, "r");
    char c;
    int i = 0;
    if (inp) {
        c = fgetc(inp);
        while (!feof(inp)){
            //putchar(c);
            list[i] = c;
            i++;
            c = fgetc(inp);
        }
        fclose(inp);
    }
    return list;
}
//Writes the final text with all of the redactions to a new text file called 'result.txt'
void printToTextFile(char *textList, int size){
    FILE *result;
    int c;
    result = fopen("result.txt", "w");
    for (int i=0; i<size; i++){
        c = textList[i];
        fputc(c, result);
    }
    fclose(result);
}


//Redacts all of the words from the list, replacing them with '*'
void redact_words(const char *text_filename, const char *redact_words_filename){
    int redactSize = sizeOfFile(redact_words_filename);
    int textSize = sizeOfFile(text_filename);

    char *textList = malloc(textSize);
    char *redactList = malloc(redactSize);

    readFromFile(redact_words_filename, redactList);
    readFromFile(text_filename, textList);
    int isWord = 0;
    int buffer = 0;
    for (int i=0; i<redactSize; i++){
        if (i!=0){
            if (redactList[i-1] == '\n'){
                for (int j=0; j<textSize; j++){
                    if (j != 0){
                        //If it finds the first char of one of the redacted words in the text, iterates
                        //through to find out if it is a redacted word, stores the indexes if it is
                        if (textList[j-1] == ' ' || textList[j-1] == '\n'){
                            if (textList[j] == redactList[i]){
                                int redactIndex = i;
                                int textIndex = j;
                                //Since all of the redacted words end in a new line, I can check for the new line,
                                //and a space in the text and, if they're both there, I can redact the word.
                                while ((redactList[redactIndex] != '\n')){
                                    if (textList[textIndex] == redactList[redactIndex]){
                                        if (redactList[redactIndex+1] == '\n'){
                                            isWord = 1;
                                            break;
                                        }
                                        buffer++;
                                    }else{
                                        break;
                                    }
                                    textIndex++;
                                    redactIndex++;
                                }
                            }
                        }
                    }
                    if (isWord) {
                        for (int k = j; k <= j + buffer; k++) {
                            if (textList[k] == ' '){
                                //Keeps the space in Manchester United
                            }
                            else{
                                textList[k] = '*';
                            }
                        }
                    }
                    isWord = 0;
                    buffer = 0;
                }

            }
        }else{
            //Checks the first word in the redacted list, without there being an index error.
            for (int j=0; j<textSize; j++){
                if (j != 0){
                    if (textList[j-1] == ' ' || textList[j-1] == '\n'){
                        if (textList[j] == redactList[i]){
                            int redactIndex = i;
                            int textIndex = j;
                            while ((redactList[redactIndex] != '\n')){
                                textIndex++;
                                redactIndex++;
                                if (textList[textIndex] == redactList[redactIndex]){
                                    buffer++;
                                    if (redactList[redactIndex+1] == '\n'){
                                        isWord = 1;
                                        break;
                                    }
                                }else{
                                    break;
                                }

                            }
                        }
                    }
                }
                if (isWord) {
                    for (int k = j; k <= j + buffer; k++) {
                        textList[k] = '*';
                    }
                }
                isWord = 0;
                buffer = 0;
            }
        }
    }
    printToTextFile(textList, textSize);
}

int main(int argc, char *argv[]) {
    const char *input_file = "./debate.txt";
    const char *redact_file = "./redact.txt";
    redact_words(input_file, redact_file);
    return EXIT_SUCCESS;
}
