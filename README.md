
# Quiz Master – C Programming Quiz Application

## 1. Project Overview

Quiz Master is a menu-driven, console-based quiz application developed using C11.

It allows users to test their knowledge through multiple-choice questions organized into different categories. Questions are loaded from an external text file.

## 2. Features

- Multiple-choice quiz with four options per question.
- External question bank using a text file.
- Category selection:
  - C Programming
  - Python
  - Computer Basics
- Immediate feedback after every answer.
- Automatic score and percentage calculation.
- Correct and incorrect answer tracking.
- Final performance summary.
- Complete answer review after the quiz.
- Restart quiz without restarting the executable.
- Input validation for non-numeric and out-of-range choices.

## 3. Technologies Used

- Programming Language: C
- Standard: C11
- Compiler: GCC
- IDE: Visual Studio Code
- Data Storage: Text file

## 4. Project Structure

```text
Task2_Quiz_Application/
|
|-- main.c
|-- questions.txt
|-- README.md
|-- quiz.exe
```

## 5. Compilation and Execution

### Compile

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o quiz.exe
```

### Run on Windows PowerShell

```powershell
.\quiz.exe
```

### Run on Linux

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o quiz
./quiz
```

Keep `questions.txt` in the same working directory as the executable.

## 6. Question Bank Format

Questions are stored in `questions.txt`.

Each question follows this format:

```text
category|question|option1|option2|option3|option4|correct_answer|-
```

Example:

```text
C|Which function displays output in C?|scanf()|printf()|main()|gets()|2|-
```

The correct answer is represented by its option number (1–4).

## 7. Input Validation

The application validates user input.

Examples:
- Alphabetic input is rejected.
- Choices outside the permitted range are rejected.
- Users are prompted again until a valid choice is entered.

## 8. Testing

The following tests were performed:

| Test | Result |
|---|---|
| Load questions from file | Passed |
| Display categories | Passed |
| Correct answer feedback | Passed |
| Incorrect answer feedback | Passed |
| Score calculation | Passed |
| Percentage calculation | Passed |
| Answer review | Passed |
| Restart quiz | Passed |
| Switch categories | Passed |
| Reject alphabetic input | Passed |
| Reject out-of-range input | Passed |

Sample test results:
- C Programming: 4/5 (80%)
- Python: 5/5 (100%)

## 9. Limitations

- The question bank is limited to the entries supplied in the text file.
- The application supports up to 100 questions and 20 categories.
- Each question has exactly four options.

## 10. Future Enhancements

- Add more question categories.
- Add difficulty levels.
- Add a timer for each question.
- Save quiz history and high scores.
- Allow users to add questions.

## 11. Author

Developed as part of the CodeNova Internship – Task 2.

---

**Quiz Master – Learn, Play and Improve!**
