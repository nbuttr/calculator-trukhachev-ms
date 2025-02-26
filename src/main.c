#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
double parse_factor();
double parse_term();
double parse_expression();

double custom_floor(double value)
{
    return (value >= 0) ? (long long)value : (long long)value - 1;
}

void skip_whitespace()
{
    while (isspace(expr[pos]))
        pos++;
}

long long parse_number()
{
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

double parse_factor()
{
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
    return parse_number();
}

double parse_term()
{
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

double parse_expression()
{
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
#ifndef UNIT_TEST
int main(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];
    if (!fgets(buffer, BUFFER_SIZE, stdin)) {
        return 1;
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
