#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(void) {
    char secretWord[100];
    int totalLines = 0;
    int selectedLine;
    char fileChar;
    bool isGameActive = true;
    int lives = 9;
    FILE *fp;
    char inputChar;
    bool isCorrectGuess = false;
    char guessedLetters[30] = {0};
    bool isFirstTurn = true;
    int guessedCount = 1;
    int missingCount = 0;
    bool isRepeated = false;
    int i;

    fp = fopen("text.txt", "r");

    if (fp == NULL) {
        perror("File error");
        return 1;
    }
    else {
        srand(time(NULL));

        for (fileChar = getc(fp); fileChar != EOF; fileChar = getc(fp)) {
            if (fileChar == '\n') {
                totalLines++;
            }
        }

        if (totalLines == 0) totalLines = 1;

        selectedLine = rand() % totalLines;

        rewind(fp);

        for (i = 0; i < selectedLine; i++) {
            fscanf(fp, "%*[^\n]\n");
        }

        fscanf(fp, " %[^\n]", secretWord);

        do {
            isCorrectGuess = false;
            printf("\033[H\033[J");

            printf("  _    _          _   _  _____ __   __         _   _  \n");
            printf(" | |  | |   /\\   | \\ | |/ ____|  \\/  |   /\\   | \\ | |\n");
            printf(" | |__| |  /  \\  |  \\| | |  __| \\  / |  /  \\  |  \\| |\n");
            printf(" |  __  | / /\\ \\ | . ` | | |_ | |\\/| | / /\\ \\ | . ` |\n");
            printf(" | |  | |/ ____ \\| |\\  | |__| | |  | |/ ____ \\| |\\  |\n");
            printf(" |_|  |_/_/    \\_\\_| \\_|\\_____|_|  |_/_/    \\_\\_| \\_|\n");
            printf("\n\n\n");

            switch (lives) {
                case 1:
                    printf("  _______\n");
                    printf(" |/      |\n");
                    printf(" |      (_)\n");
                    printf(" |      \\|/\n");
                    printf(" |       |\n");
                    printf(" |      / \\\n");
                    printf("_|___\n");
                    printf("LAST CHANCE, CHOOSE WISELY!!!\n");
                    break;
                case 2:
                    printf("  _______\n");
                    printf(" |/      |\n");
                    printf(" |      (_)\n");
                    printf(" |      \\|/\n");
                    printf(" |       |\n");
                    printf(" |      / \n");
                    printf("_|___\n");
                    break;
                case 3:
                    printf("  _______\n");
                    printf(" |/      |\n");
                    printf(" |      (_)\n");
                    printf(" |      \\|/\n");
                    printf(" |       |\n");
                    printf(" |      \n");
                    printf("_|___\n");
                    break;
                case 4:
                    printf("  _______\n");
                    printf(" |/      |\n");
                    printf(" |      (_)\n");
                    printf(" |      \\|\n");
                    printf(" |       |\n");
                    printf(" |      \n");
                    printf("_|___\n");
                    break;
                case 5:
                    printf("  _______\n");
                    printf(" |/      |\n");
                    printf(" |      (_)\n");
                    printf(" |       |\n");
                    printf(" |       |\n");
                    printf(" |      \n");
                    printf("_|___\n");
                    break;
                case 6:
                    printf("  _______\n");
                    printf(" |/      |\n");
                    printf(" |      (_)\n");
                    printf(" |      \n");
                    printf(" |       \n");
                    printf(" |      \n");
                    printf("_|___\n");
                    break;
                case 7:
                    printf("  _______\n");
                    printf(" |/      |\n");
                    printf(" |      \n");
                    printf(" |      \n");
                    printf(" |       \n");
                    printf(" |      \n");
                    printf("_|___\n");
                    break;
                case 8:
                    printf(" \n");
                    printf(" |/      \n");
                    printf(" |      \n");
                    printf(" |      \n");
                    printf(" |       \n");
                    printf(" |      \n");
                    printf("_|___\n");
                break;
                case 9:
                    printf(" \n");
                    printf("       \n");
                    printf("       \n");
                    printf("       \n");
                    printf("        \n");
                    printf("       \n");
                    printf("_____\n");
                break;
                default:
                    printf("CRITICAL ERROR!!");
                    return 1;
                    break;
            }

            printf("Lives: %d \n\n", lives);
            missingCount = 0;

            if (isFirstTurn) {
                printf("%c ", secretWord[0]);
                for (i = 1; i < strlen(secretWord)-1; i++) {
                    printf("_ ");
                    missingCount++;
                }
                printf("%c \n\n", secretWord[strlen(secretWord)-1]);

                printf("Choose a letter: ");
                scanf(" %c", &inputChar);
                guessedLetters[0] = inputChar;
                for (i = 1; i < strlen(secretWord); i++) {
                    if (secretWord[i] == inputChar) {
                        isCorrectGuess = true;
                    }
                }
                if (isCorrectGuess) {
                    printf("The letter '%c' is in the word!", inputChar);
                }
                else {
                    printf("The letter '%c' is NOT in the word!", inputChar);
                    lives--;
                }
                isFirstTurn = false;
            }
            else {
                printf("%c ", secretWord[0]);
                for (i = 1; i < strlen(secretWord)-1; i++) {
                    if (strchr(guessedLetters, secretWord[i]) != NULL) {
                        printf("%c ", secretWord[i]);
                    }
                    else {
                        printf("_ ");
                        missingCount++;
                    }
                }
                printf("%c \n\n", secretWord[strlen(secretWord)-1]);

                if (missingCount == 0) {
                    printf("\nYOU WON!!!");
                    return 0;
                }

                isRepeated = false;
                do {
                    printf("Choose a letter: ");
                    scanf(" %c", &inputChar);
                    if (strchr(guessedLetters, inputChar) != NULL) {
                        isRepeated = true;
                        printf("You already chose the letter: %c \n", inputChar);
                    }
                    else {
                        isRepeated = false;
                    }
                } while (isRepeated);

                guessedLetters[guessedCount] = inputChar;
                guessedCount++;

                for (i = 1; i < strlen(secretWord); i++) {
                    if (secretWord[i] == inputChar) {
                        isCorrectGuess = true;
                    }
                }
                if (isCorrectGuess) {
                    printf("The letter '%c' is in the word!", inputChar);
                }
                else {
                    printf("The letter '%c' is NOT in the word!", inputChar);
                    lives--;
                }
            }

            if (lives <= 0) {
                isGameActive = false;
            }

        } while (isGameActive);

        printf("\nYou lost! The word was: %s", secretWord);

        fclose(fp);
    }
    return 0;
}