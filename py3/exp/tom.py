import pytest

class Dog:
    def __init__(self, 
            name=None, age=None, color=None):
        self.name = name 
        self.age = age
        self.color = color

    def change_color(self, color):
        self.color = color
    def get_name_and_age(self):
        return self.name, self.age

@pytest.fixture
def create_tom() -> Dog:
    tom = Dog("Tom", 23, "Blue")
    return tom

def test_tom(create_tom):
    name, age = create_tom.get_name_and_age()
    assert age == 23
    

    
