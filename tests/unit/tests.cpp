#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <gtest/gtest.h>

// Подключаем заголовок, где объявлены функции калькулятора (можно создать parser.h)
extern "C" {
typedef enum { INTEGER,
    FLOAT } Mode;
void skip_whitespace(void);
long long parse_number(void);
double parse_factor(void);
double parse_term(void);
double parse_expression(void);
double custom_floor(double value);

extern char* expr;
extern int pos;
extern Mode mode;
}

// Тест пропуска пробелов
TEST(CalculatorTest, SkipWhitespace)
{
    expr = (char*)"  42";
    pos = 0;
    skip_whitespace();
    ASSERT_EQ(pos, 2);
}

// Тест разбора числа
TEST(CalculatorTest, ParseNumber)
{
    expr = (char*)"12345";
    pos = 0;
    ASSERT_EQ(parse_number(), 12345);
}

// Тест разбора числа с превышением границ
TEST(CalculatorTest, ParseNumber_OutOfBounds)
{
    expr = (char*)"2000000001"; // больше MAX_INT
    pos = 0;
    ASSERT_EXIT(
        {
            parse_number();
            exit(0);
        },
        ::testing::ExitedWithCode(1), "Error: Integer out of bounds");
}

// Тест разбора фактора — число
TEST(CalculatorTest, ParseFactor_Number)
{
    expr = (char*)"78";
    pos = 0;
    ASSERT_EQ(parse_factor(), 78);
}

// Тест разбора фактора — скобки
TEST(CalculatorTest, ParseFactor_Parentheses)
{
    expr = (char*)"(3+2)";
    pos = 0;
    ASSERT_EQ(parse_factor(), 5);
}

// Тест ошибки: отсутствует закрывающая скобка
TEST(CalculatorTest, ParseFactor_MissingParentheses)
{
    expr = (char*)"(3+2";
    pos = 0;
    ASSERT_EXIT(
        {
            parse_factor();
            exit(0);
        },
        ::testing::ExitedWithCode(1), "Error: Mismatched parentheses");
}

// Тест деления на 0 в целочисленном режиме
TEST(CalculatorTest, ParseTerm_DivideByZero)
{
    expr = (char*)"10/0";
    pos = 0;
    ASSERT_EXIT(
        {
            parse_term();
            exit(0);
        },
        ::testing::ExitedWithCode(1), "Error: Division by zero");
}

// Тест сложения
TEST(CalculatorTest, ParseExpression_Addition)
{
    expr = (char*)"2+3";
    pos = 0;
    ASSERT_EQ(parse_expression(), 5);
}

// Тест custom_floor
TEST(CalculatorTest, CustomFloor)
{
    ASSERT_EQ(custom_floor(5.9), 5);
    ASSERT_EQ(custom_floor(-3.2), -4);
}
