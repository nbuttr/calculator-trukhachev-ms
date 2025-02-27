#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE 1024
#define MIN_INT -2000000000
#define MAX_INT 2000000000
#define MIN_FLOAT 0.0001

typedef enum { INTEGER, FLOAT } Mode;

char *expr;
int pos = 0;
Mode mode = INTEGER;

void skip_whitespace();
long long parse_number();  
double parse_number_f();   
double parse_factor();
double parse_term();
double parse_expression();

double custom_floor(double value) {
    return (value >= 0) ? (long long)value : (long long)value - 1;
}

void skip_whitespace() {
    while (isspace(expr[pos])) pos++;
}

long long parse_number() {
    long long num = 0;
    while (isdigit(expr[pos])) {
        num = num * 10 + (expr[pos] - '0');
        if (num > MAX_INT) {
            fprintf(stderr, "Error: Integer out of bounds\n");
            exit(1);
        }
        pos++;
    }
    return num;
}

double parse_number_f() {
    double num = 0.0;
    while (isdigit(expr[pos])) {
        num = num * 10 + (expr[pos] - '0');
        pos++;
    }
    if (expr[pos] == '.') {
        pos++;
        double factor = 0.1;
        while (isdigit(expr[pos])) {
            num += (expr[pos] - '0') * factor;
            factor /= 10;
            pos++;
        }
    }
    return num;
}

double parse_factor() {
    skip_whitespace();
    if (expr[pos] == '(') {
        pos++;
        double result = parse_expression();
        skip_whitespace();
        if (expr[pos] != ')') {
            fprintf(stderr, "Error: Mismatched parentheses\n");
            exit(1);
        }
        pos++;
        return result;
    }
    if (mode == FLOAT)
        return parse_number_f();
    else
        return (double) parse_number();
}

double parse_term() {
    double result = parse_factor();
    skip_whitespace();
    
    while (expr[pos] == '*' || expr[pos] == '/') {
        char op = expr[pos];
        pos++;
        double right = parse_factor();
        
        if (op == '/') {
            if (mode == FLOAT) {
                if ((right > -MIN_FLOAT && right < MIN_FLOAT)) {
                    fprintf(stderr, "Error: Division by too small number\n");
                    exit(1);
                }
                result /= right;
            } else {
                if (right == 0) {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(1);
                }
                result = custom_floor(result / right);
            }
        } else {
            result *= right;
        }
        
        skip_whitespace();
    }
    return result;
}

double parse_expression() {
    double result = parse_term();
    skip_whitespace();
    
    while (expr[pos] == '+' || expr[pos] == '-') {
        char op = expr[pos];
        pos++;
        double right = parse_term();
        
        if (op == '+')
            result += right;
        else
            result -= right;
        
        skip_whitespace();
    }
    return result;
}

int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

#ifndef UNIT_TEST
int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];
    if (!fgets(buffer, BUFFER_SIZE, stdin)) {
        return 1;
    }
    
    
    char expr_nospace[BUFFER_SIZE];
    int j = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isspace(buffer[i])) {
            expr_nospace[j++] = buffer[i];
        }
    }
    expr_nospace[j] = '\0';
    
    
    for (int i = 0; i < j - 1; i++) {
        if (is_operator(expr_nospace[i]) && is_operator(expr_nospace[i+1])) {
            fprintf(stderr, "Error: consecutive operators found: '%c%c'\n", expr_nospace[i], expr_nospace[i+1]);
            return 1;
        }
    }
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        char c = buffer[i];
        
        if (!isdigit(c) && !isspace(c) &&
            c != '(' && c != ')' &&
            c != '*' && c != '+' &&
            c != '-' && c != '/') {
            fprintf(stderr, "Error: Invalid character '%c'\n", c);
            return 1; 
        }
    }
    
    expr = buffer;
    pos = 0;
    
    if (argc > 1 && strcmp(argv[1], "--float") == 0) {
        mode = FLOAT;
    }
    
    double result = parse_expression();
    
    if (mode == FLOAT) {
        printf("%.4f\n", result);
    } else {
        printf("%lld\n", (long long)result);
    }
    
    return 0;
}
#endif
