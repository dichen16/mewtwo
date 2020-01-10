import pytest
import unittest

def f():
    return 3

# @pytest.mark.org
class Base(unittest.TestCase):
    def __init__(self, 
            name=None, age=None, color=None):
        self.name = name 
        self.age = age
        self.color = color

    def change_color(self, color):
        self.color = color
    def get_name_and_age(self):
        return self.name, self.age
    @pytest.mark.nestedtwo
    def test_case1():
        assert f() == 4

    @pytest.mark.nested
    def test_case2():
        assert f() == 4

    

    
