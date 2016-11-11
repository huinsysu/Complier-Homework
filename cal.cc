#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <cstring>

using namespace std;

typedef enum {

    TOKENIZER_ERROR,
    TOKENIZER_ENDOFINPUT,
    TOKENIZER_NUMBER,
    TOKENIZER_PLUS,
    TOKENIZER_MINUS,
    TOKENIZER_ASTR,
    TOKENIZER_SLASH,
    TOKENIZER_LPAREN,
    TOKENIZER_RPAREN

} token_t;

#define EXPR_LEN       512
#define MAX_NUM_LEN    5

#define NO_ERROR       0
#define SYNTAX_ERROR    1

static char *curr_char = NULL;
static char *next_char = NULL;
static token_t current_token = TOKENIZER_ERROR;
static int error_code = NO_ERROR;

int factor();
int term();
int expr();

token_t singlechar() {

    switch (*curr_char) {

        case '+':
            return TOKENIZER_PLUS;

        case '-':
            return TOKENIZER_MINUS;

        case '*':
            return TOKENIZER_ASTR;

        case '/':
            return TOKENIZER_SLASH;

        case '(':
            return TOKENIZER_LPAREN;

        case ')':
            return TOKENIZER_RPAREN;

        default:
            break;
    }
    return TOKENIZER_ERROR;
}

token_t get_token() {

    if (*curr_char == '\0')
        return TOKENIZER_ENDOFINPUT;

    if (isdigit(*curr_char)) {
        for (int i = 0; i < MAX_NUM_LEN; i++) {
            if (!isdigit(*(curr_char + i))) {
                next_char = curr_char + i;
                return TOKENIZER_NUMBER;
            }
        }
    } else if (singlechar()) {
        next_char++;
        return singlechar();
    }

    return TOKENIZER_ERROR;

}

bool tokenizer_finished() {

    return *curr_char == '\0' || current_token == TOKENIZER_ENDOFINPUT;

}

void error(int error) {

    error_code = error;

}

void next_tokenizer() {

    if (!tokenizer_finished()) {
        curr_char = next_char;
        current_token = get_token();
    }

}

token_t get_current_token() {

    return current_token;

}

int tokenizer_num() {

    return atoi(curr_char);

}

void accept(token_t token) {

    if (token != get_current_token())
        error(SYNTAX_ERROR);

     next_tokenizer();

}

int factor() {

    int r;

    switch (get_current_token()) {

        case TOKENIZER_NUMBER:
            r = tokenizer_num();
            accept(TOKENIZER_NUMBER);
            break;

        case TOKENIZER_LPAREN:
            accept(TOKENIZER_LPAREN);
            r = expr();
            accept(TOKENIZER_RPAREN);
            break;

        default:
            error(SYNTAX_ERROR);
    }
    return r;

}

int term() {

    int f1, f2;
    token_t op;

    f1 = factor();
    op = get_current_token();

    while (op == TOKENIZER_ASTR || op == TOKENIZER_SLASH) {

        next_tokenizer();
        f2 = factor();
        
        switch (op) {

            case TOKENIZER_ASTR:
                f1 = f1 * f2;
                break;

            case TOKENIZER_SLASH:
                f1 = f1 / f2;
                break;
        }
        op = get_current_token();

    }
    return f1;

}

int expr() {

    int t1, t2 = 0;
    token_t op;

    t1 = term();
    op = get_current_token();

    while (op == TOKENIZER_PLUS || op == TOKENIZER_MINUS) {

        next_tokenizer();

        t2 = term();
        switch (op) {
        
            case TOKENIZER_PLUS:
                t1 = t1 + t2;
                break;

            case TOKENIZER_MINUS:
                t1 = t1 - t2;
                break;
        }
        op = get_current_token();
    }
   
    return t1;

}

void init_token(char *expr) {

    curr_char = next_char = expr;
    current_token = get_token();

}

int main() {

    char e[EXPR_LEN];
    int r;

    cout << "输入'q'退出计算器。" << endl;
    cout << ">>";

    while (cin >> e) {

        if (e[0] == 'q')
            break;

        init_token(e);
        r = expr();

        if (error_code) {
            cout << "Syntax Error!" << endl;
            error_code = 0;
            continue;
        }
        cout << r << endl;
        cout << ">>";

    }

    return 0;

}
