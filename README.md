# Калькулятор на C

## 1. Что сделано?
Этот проект представляет собой консольный калькулятор, который принимает арифметическое выражение из стандартного ввода, разбирает его и вычисляет результат. Поддерживаются следующие операции:
- Сложение (`+`)
- Вычитание (`-`)
- Умножение (`*`)
- Деление (`/`)
- Круглые скобки (`(` и `)`) для управления порядком вычислений

Программа считывает ввод без приглашения (ожидает ввод выражения и `EOF`) и выводит только итоговое число.

## 2. Как это запустить?
### **Компиляция:**
```sh
gcc main.c -o calculator
```

### **Запуск:**
```sh
echo "3 + 5 * (2 - 8) / 2" | ./calculator
```

ИЛИ:
```sh
./calculator
# Введите выражение, затем нажмите Ctrl+D (EOF) в Linux/macOS или Ctrl+Z в Windows
3 + 5 * (2 - 8) / 2
```

### **Ожидаемый вывод:**
```
1
```

## 3. Как это сделано?
Программа реализует **разбор арифметического выражения** с использованием метода **рекурсивного спуска**, учитывая приоритет операторов:

1. **`parse_expression()`** – обрабатывает `+` и `-`
2. **`parse_term()`** – обрабатывает `*` и `/`
3. **`parse_factor()`** – обрабатывает числа и скобки
4. **`skip_whitespace()`** – игнорирует пробелы
5. Ввод считывается через `fgets()`, выражение вычисляется и выводится единственное число

Программа использует только стандартную библиотеку C (`libc`) и не требует дополнительных зависимостей.

