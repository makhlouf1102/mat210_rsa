from random import randint

def is_pair(n):
    return n % 2 == 0

def miller_test(n=7, confidance=10):

    if is_pair(n) or n < 3:
        return False
    if n == 3 : 
        return True
    
    # solution to n - 1 = pow(2, k) x m
    k, m = 0,  n - 1
    while (pow(m, 1, 2)) == 0:
        k += 1
        m//=2
    for _ in range(0,confidance):
        # random value a such that 1 < a < k - 1
        a = randint(2,n - 2)

        # compute x = pow(a, m) % n
        x = pow(a, m, n)

        if x == 1 or x == n - 1:
            continue

        for _ in range(k - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
        
    return True


def is_probably_prime(n):
    # check if it is a prime number or not
    for p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]:
        if n % p == 0:
            return False
    return miller_test(n)

def generate_prime(bits=2048):
    n_test = randint(2**(bits - 1), 2**bits - 1)
    if is_pair(n_test):
        n_test -= 1
    while not is_probably_prime(n_test):
        n_test += 2
    return n_test

def extended_gcd(a, b):
    if b == 0:
        return (1, 0, a)
    else:
        x, y, gcd = extended_gcd(b, a % b)
        return (y, x - (a // b) * y, gcd)

def mod_inverse(phi, e=65537):
    x, y, gcd = extended_gcd(e, phi)
    if gcd != 1:
        raise Exception('Inverse modulaire n\'existe pas')
    else:
        return x % phi

# message
m = 123450249

# generating big prime numbers
p, q = generate_prime(), generate_prime()

# calculate the module for private and public keys
n = p*q

# calculate euler's totient
phi = (p-1)*(q-1)

# pick random public exponent 1<e<phi
e = 65537 # Best for security and performance

# computer private exponent
d = mod_inverse(phi, e)

# form public and private keys
public_key = {
    "mod": n,
    "exponent":e
}

private_key = {
    "mod": n,
    "exponent":d
}

# encryption m
c = pow(m, public_key["exponent"], public_key["mod"])

# decription of c
new_m = pow(c, private_key["exponent"], private_key["mod"])

print(new_m==m)

