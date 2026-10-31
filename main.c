
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_QUESTIONS 100
#define MAX_CATEGORIES 20
#define OPTIONS 4
#define LINE_SIZE 1024
#define CATEGORY_SIZE 40
#define QUESTION_SIZE 250
#define OPTION_SIZE 150
#define FILE_NAME "questions.txt"

typedef struct {
    char category[CATEGORY_SIZE];
    char question[QUESTION_SIZE];
    char options[OPTIONS][OPTION_SIZE];
    int correctAnswer;
} Question;

typedef struct {
    int questionIndex;
    int selectedAnswer;
    int isCorrect;
} AnswerRecord;

Question questions[MAX_QUESTIONS];
char categories[MAX_CATEGORIES][CATEGORY_SIZE];

int questionCount = 0;
int categoryCount = 0;

/* Remove leading and trailing whitespace */
void trim(char *str) {
    char *start = str;
    char *end;

    while (isspace((unsigned char)*start)) {
        start++;
    }

    memmove(str, start, strlen(start) + 1);

    end = str + strlen(str);

    while (end > str && isspace((unsigned char)*(end - 1))) {
        end--;
    }

    *end = '\0';
}

/* Read and validate an integer from the user */
int readInteger(const char *message, int min, int max) {
    char input[100];
    char *end;
    long value;

    while (1) {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }

        input[strcspn(input, "\n")] = '\0';
        trim(input);

        errno = 0;
        value = strtol(input, &end, 10);

        if (input[0] == '\0' || end == input ||
            *end != '\0' || errno == ERANGE) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        if (value < min || value > max) {
            printf("Please enter a number between %d and %d.\n",
                   min, max);
            continue;
        }

        return (int)value;
    }
}

/* Check whether a category already exists */
int categoryExists(const char *name) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(categories[i], name) == 0) {
            return 1;
        }
    }

    return 0;
}

/* Add a new category if it does not already exist */
void addCategory(const char *name) {
    if (!categoryExists(name) &&
        categoryCount < MAX_CATEGORIES) {
        strcpy(categories[categoryCount], name);
        categoryCount++;
    }
}

/* Load questions from the external text file */
int loadQuestions(void) {
    FILE *file = fopen(FILE_NAME, "r");
    char line[LINE_SIZE];

    if (file == NULL) {
        printf("\nError: Could not open %s.\n", FILE_NAME);
        printf("Make sure the file is in the project folder.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL &&
           questionCount < MAX_QUESTIONS) {

        char *fields[8];
        int fieldCount = 0;

        line[strcspn(line, "\r\n")] = '\0';
        trim(line);

        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        char *token = strtok(line, "|");

        while (token != NULL && fieldCount < 8) {
            fields[fieldCount++] = token;
            token = strtok(NULL, "|");
        }

        if (fieldCount != 8 || token != NULL) {
            printf("Skipping an invalid question entry.\n");
            continue;
        }

        for (int i = 0; i < 8; i++) {
            trim(fields[i]);
        }

        if (strlen(fields[0]) >= CATEGORY_SIZE ||
            strlen(fields[1]) >= QUESTION_SIZE ||
            strlen(fields[2]) >= OPTION_SIZE ||
            strlen(fields[3]) >= OPTION_SIZE ||
            strlen(fields[4]) >= OPTION_SIZE ||
            strlen(fields[5]) >= OPTION_SIZE ||
            fields[0][0] == '\0' ||
            fields[1][0] == '\0' ||
            fields[2][0] == '\0' ||
            fields[3][0] == '\0' ||
            fields[4][0] == '\0' ||
            fields[5][0] == '\0') {
            printf("Skipping an entry with invalid field length.\n");
            continue;
        }

        char *end;
        errno = 0;
        long correct = strtol(fields[6], &end, 10);

        if (errno == ERANGE || end == fields[6] ||
            *end != '\0' || correct < 1 || correct > 4) {
            printf("Skipping an entry with an invalid answer.\n");
            continue;
        }

        /* The eighth field is reserved for future use.
           Use '-' in the question bank. */
        if (strcmp(fields[7], "-") != 0) {
            printf("Skipping an entry: final field must be '-'.\n");
            continue;
        }

        Question *q = &questions[questionCount];

        strcpy(q->category, fields[0]);
        strcpy(q->question, fields[1]);

        for (int i = 0; i < OPTIONS; i++) {
            strcpy(q->options[i], fields[i + 2]);
        }

        q->correctAnswer = (int)correct;

        addCategory(q->category);
        questionCount++;
    }

    fclose(file);
    return questionCount;
}

/* Display category selection menu */
int selectCategory(void) {
    printf("\n========== SELECT CATEGORY ==========\n");
    printf("0. All Categories\n");

    for (int i = 0; i < categoryCount; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    printf("=====================================\n");

    return readInteger("Choose a category: ", 0, categoryCount);
}

/* Display one question and its options */
int askQuestion(const Question *q, int number, int total) {
    printf("\n-------------------------------------\n");
    printf("Question %d of %d\n", number, total);
    printf("Category: %s\n\n", q->category);
    printf("%s\n\n", q->question);

    for (int i = 0; i < OPTIONS; i++) {
        printf("%d. %s\n", i + 1, q->options[i]);
    }

    printf("\n");

    return readInteger("Your answer (1-4): ", 1, 4);
}

/* Show final score and answer review */
void showResults(AnswerRecord records[], int total) {
    int score = 0;

    printf("\n\n=====================================\n");
    printf("             QUIZ RESULTS            \n");
    printf("=====================================\n");

    for (int i = 0; i < total; i++) {
        if (records[i].isCorrect) {
            score++;
        }
    }

    double percentage = (double)score / total * 100.0;

    printf("Total Questions : %d\n", total);
    printf("Correct Answers : %d\n", score);
    printf("Wrong Answers   : %d\n", total - score);
    printf("Final Score     : %d/%d\n", score, total);
    printf("Percentage      : %.2f%%\n", percentage);

    printf("\nPerformance: ");

    if (percentage >= 90) {
        printf("Excellent! Outstanding performance!\n");
    } else if (percentage >= 75) {
        printf("Very Good! Keep it up!\n");
    } else if (percentage >= 50) {
        printf("Good effort! Keep practising!\n");
    } else {
        printf("Keep learning. You can improve!\n");
    }

    printf("\n========== ANSWER REVIEW ==========\n");

    for (int i = 0; i < total; i++) {
        Question *q = &questions[records[i].questionIndex];

        printf("\nQ%d. %s\n", i + 1, q->question);

        printf("Your answer: %s\n",
               q->options[records[i].selectedAnswer - 1]);

        printf("Correct answer: %s\n",
               q->options[q->correctAnswer - 1]);

        if (records[i].isCorrect) {
            printf("Result: CORRECT\n");
        } else {
            printf("Result: INCORRECT\n");
        }
    }

    printf("\n=====================================\n");
}

/* Run one quiz session */
void runQuiz(int selectedCategory) {
    AnswerRecord records[MAX_QUESTIONS];
    int total = 0;
    int number = 0;

    for (int i = 0; i < questionCount; i++) {
        if (selectedCategory == 0 ||
            strcmp(questions[i].category,
                   categories[selectedCategory - 1]) == 0) {
            total++;
        }
    }

    if (total == 0) {
        printf("\nNo questions available in this category.\n");
        return;
    }

    printf("\n=====================================\n");
    printf("             QUIZ STARTED            \n");
    printf("=====================================\n");

    for (int i = 0; i < questionCount; i++) {
        if (selectedCategory != 0 &&
            strcmp(questions[i].category,
                   categories[selectedCategory - 1]) != 0) {
            continue;
        }

        int answer = askQuestion(&questions[i], number + 1, total);

        if (answer == -1) {
            return;
        }

        records[number].questionIndex = i;
        records[number].selectedAnswer = answer;
        records[number].isCorrect =
            (answer == questions[i].correctAnswer);

        if (records[number].isCorrect) {
            printf("\nCorrect! Well done!\n");
        } else {
            printf("\nIncorrect!\n");
            printf("Correct answer: %s\n",
                   questions[i].options[
                       questions[i].correctAnswer - 1]);
        }

        number++;
    }

    showResults(records, number);
}

/* Main program */
int main(void) {
    int choice;

    printf("\n");
    printf("========================================\n");
    printf("             WELCOME TO                 \n");
    printf("              QUIZ MASTER               \n");
    printf("========================================\n");
    printf("       Test your knowledge and learn!    \n");

    if (loadQuestions() == 0) {
        printf("\nNo valid questions were loaded.\n");
        printf("Please check your questions.txt file.\n");
        return 1;
    }

    printf("\nSuccessfully loaded %d questions!\n",
           questionCount);

    do {
        int category = selectCategory();

        if (category == -1) {
            break;
        }

        runQuiz(category);

        printf("\n========== MAIN MENU ==========\n");
        printf("1. Play Again\n");
        printf("2. Exit\n");

        choice = readInteger("Enter your choice: ", 1, 2);

        if (choice == -1) {
            break;
        }

    } while (choice == 1);

    printf("\nThank you for playing Quiz Master!\n");
    printf("Keep learning and keep growing!\n\n");

    return 0;
}
