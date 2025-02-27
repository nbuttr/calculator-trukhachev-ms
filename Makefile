# Makefile для проекта Calculator

# Компиляторы и флаги
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c11
CXXFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c++17
LDFLAGS = -lstdc++ -lm
# Линковка для тестов: локальная сборка GoogleTest через -L, без явного включения библиотеки в списке зависимостей
TEST_LDFLAGS = -L$(GTEST_BUILD) -lgtest -lgtest_main -lpthread

# Каталоги
SRC_DIR = src
BUILD_DIR = build
UNIT_TESTS_DIR = tests/unit
INTEGRATION_TEST_DIR = tests/integration

# Исходники и артефакты приложения
APP_SRC = $(SRC_DIR)/main.c 
APP_OBJ = $(BUILD_DIR)/main.o 
APP_EXE = $(BUILD_DIR)/app.exe

# Исходники и артефакты unit-тестов
# При сборке тестов main.c компилируется с флагом -DUNIT_TEST, чтобы исключить production main
TEST_SRC = $(UNIT_TESTS_DIR)/tests.cpp 
TEST_OBJ = $(BUILD_DIR)/tests.o $(BUILD_DIR)/main_test.o
TEST_EXE = $(BUILD_DIR)/unit-tests.exe

# GoogleTest (если не установлен системно, будет клонирован в папку googletest)
GTEST_DIR = googletest
GTEST_BUILD = $(GTEST_DIR)/build

# Параметры форматирования кода
FORMAT_DIRS = $(SRC_DIR) $(UNIT_TESTS_DIR)
CLANG_FORMAT = clang-format

# Параметры для интеграционных тестов (Python+pytest)
VENV = venv
PYTHON = $(VENV)/bin/python
PIP = $(VENV)/bin/pip
INT_TESTS = $(INTEGRATION_TEST_DIR)/tests.py

.PHONY: all clean run-int run-float run-unit-test format venv run-integration-tests gtest

# Цель "all" собирает приложение и unit-тесты
all: $(APP_EXE) $(TEST_EXE)

# --- Сборка приложения ---
$(APP_EXE): $(APP_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# --- Сборка GoogleTest ---
$(GTEST_DIR)/CMakeLists.txt:
	git clone https://github.com/google/googletest.git $(GTEST_DIR)

$(GTEST_BUILD):
	mkdir -p $(GTEST_BUILD)
	cd $(GTEST_BUILD) && cmake .. && $(MAKE)

# --- Сборка unit-тестов ---
$(TEST_EXE): $(TEST_OBJ) | $(GTEST_BUILD)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TEST_LDFLAGS) $(LDFLAGS)

$(BUILD_DIR)/tests.o: $(UNIT_TESTS_DIR)/tests.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c -o $@ $<

# Компилируем src/main.c для тестов с флагом -DUNIT_TEST, чтобы production main не включался
$(BUILD_DIR)/main_test.o: $(SRC_DIR)/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -DUNIT_TEST -c -o $@ $<

# --- Очистка ---
clean:
	rm -rf $(BUILD_DIR) $(VENV)

# --- Запуск приложения ---
run-int: $(APP_EXE)
	./$(APP_EXE)

run-float: $(APP_EXE)
	./$(APP_EXE) --float

# --- Запуск unit-тестов ---
run-unit-test: $(TEST_EXE)
	./$(TEST_EXE)

# --- Форматирование исходников ---
format:
	@find $(FORMAT_DIRS) -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec $(CLANG_FORMAT) -i -style=file {} +

# --- Виртуальное окружение Python и установка pytest ---
venv:
	python3 -m venv $(VENV)
	$(PIP) install --upgrade pip
	$(PIP) install pytest

# --- Запуск интеграционных тестов ---
run-integration-tests: venv $(APP_EXE)
	@APP_PATH=$(abspath $(APP_EXE)) $(PYTHON) -m pytest -v $(INTEGRATION_TEST_DIR)

