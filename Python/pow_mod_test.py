import random
from pow_mod import pow_mod

def test_pow_mod(num_tests=1000):
    for i in range(num_tests):
        a = random.randint(2**1999, 2**2000 - 1)
        e = random.randint(2**1999, 2**2000 - 1)
        m = random.randint(2**1999, 2**2000 - 1)

        expected = pow(a, e, m)
        result = pow_mod(a, e, m)

        print(f"Test #{i + 1}: a={a}, e={e}, m={m}")
        print(f"Expected: {expected}, Got: {result}")
        print(f"Got: {result}")

        if expected != result:
            print("Test failed!\n")
            return False
        else:
            print("Test passed!\n")
    print("All tests passed!")
    return True

test_pow_mod()