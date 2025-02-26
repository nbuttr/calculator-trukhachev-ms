import subprocess
import sys
import os

APP_PATH = os.path.join(os.path.dirname(os.path.dirname(__file__)), "..", "app.exe")

def run_calculator(args, input_data):
    proc = subprocess.Popen([APP_PATH] + args, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = proc.communicate(input_data)
    return proc.returncode, stdout.strip(), stderr.strip()

def test_integer_mode():
    code, output, err = run_calculator([], "3+5\n")
    # Ожидается целочисленный вывод
    assert code == 0
    assert output == "8"

def test_float_mode():
    code, output, err = run_calculator(["--float"], "10/4\n")
    assert code == 0
    # Проверяем, что результат равен 2.5000 с точностью до 4 знаков после запятой
    assert abs(float(output) - 2.5000) < 0.0001
