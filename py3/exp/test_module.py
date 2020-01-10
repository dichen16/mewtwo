def test_tom(create_tom):
    name, age = create_tom.get_name_and_age()
    assert age == 23


