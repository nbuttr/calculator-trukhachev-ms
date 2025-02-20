#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

char *expr;
int pos = 0;

void skip_whitespace() {
    while (expr[pos] == ' ') pos++;
}

int parse_number() {
    int num = 0;
    while (isdigit(expr[pos])) {
        num = num * 10 + (expr[pos] - '0');
        pos++;
    }
    return num;
}

int parse_factor() {
    skip_whitespace();
    if (expr[pos] == '(') {  
        pos++; 
        int result = parse_expression();
        skip_whitespace();
        pos++; 
        return result;
    }
    return parse_number();
}


int parse_term() {
    int result = parse_factor();
    skip_whitespace();
    
    while (expr[pos] == '*' || expr[pos] == '/') {
        char op = expr[pos];
        pos++;
        int right = parse_factor();
        
        if (op == '*')
            result *= right;
        else if (op == '/')
            result /= right;
        
        skip_whitespace();
    }
    return result;
}

int parse_expression() {
    int result = parse_term();
    skip_whitespace();
    
    while (expr[pos] == '+' || expr[pos] == '-') {
        char op = expr[pos];
        pos++;
        int right = parse_term();
        
        if (op == '+')
            result += right;
        else if (op == '-')
            result -= right;
        
        skip_whitespace();
    }
    return result;
}

int main() {
    char buffer[BUFFER_SIZE];
    if (!fgets(buffer, BUFFER_SIZE, stdin)) {
        return 1;
    }

    expr = buffer;
    pos = 0;
    
    printf("%d\n", parse_expression());
    
    return 0;
}