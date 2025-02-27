import subprocess
import sys
import os
   
def test_sum_int():
    res = subprocess.run(["./build/app.exe"], input = "2+2", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 4
       
def test_mult_int():
    res = subprocess.run(["./build/app.exe"], input = "12*5", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 60
 
def test_many_expressions_int():
    res = subprocess.run(["./build/app.exe"], input = "2+2*2+(6/2)", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 9

def test_many_expressions_float():
    res = subprocess.run(["./build/app.exe","--float"], input = "4*5/(8+13)", text=True, capture_output=True)
    assert res.returncode == 0
    assert float(res.stdout) == 0.9524
    
def test_wrong_symbol():
    res = subprocess.run(["./build/app.exe"], input = "4*x", text=True, capture_output=True)
    assert res.returncode != 0
    
def test_lot_space():
    res = subprocess.run(["./build/app.exe"], input = "4                                                                *2", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout) == 8
    
def test_lot_braces():
    res = subprocess.run(["./build/app.exe"], input = "(((2+3)", text=True, capture_output=True)
    assert res.returncode != 0
    
def test_division_by_zero():
    res = subprocess.run(["./build/app.exe"], input = "10/0", text=True, capture_output=True)
    assert res.returncode != 0
    
def test_lot_symbols_in_expression():
    res = subprocess.run(["./build/app.exe"], input = "3++++3", text=True, capture_output=True)
    assert res.returncode != 0
   
def test_max_number():
    res = subprocess.run(["./build/app.exe"], input = "2000000001+1", text=True, capture_output=True)
    assert res.returncode != 0

def test_min_number():
    res = subprocess.run(["./build/app.exe"], input = "-2000000001+1", text=True, capture_output=True)
    assert res.returncode != 0

