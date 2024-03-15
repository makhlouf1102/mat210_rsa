def pow_mod(a, e, m):
    result = 1
    a = a % m
    while e > 0:
        if e % 2 == 1:
            result = (result * a) % m
        a = (a * a) % m
        e //= 2
    return result