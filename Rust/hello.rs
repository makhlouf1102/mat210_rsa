use std::time::{SystemTime, UNIX_EPOCH};

fn generate_random_number(min: u128, max: u128) -> u128 {
    assert!(max > min, "Max must be greater than min");

    let now = SystemTime::now();
    let since_the_epoch = now.duration_since(UNIX_EPOCH)
        .expect("Time went backwards");
    let range = max - min;
    let random_number = min + (since_the_epoch.as_nanos() % range) as u128;

    random_number
}


fn is_pair(n: u128) -> bool {
    return n % 2 == 0;
}

fn pow_mod(mut a: u128, mut e: u128, m: u128) -> u128 {
    let mut result: u128 = 1;
    a %= m;
    while e > 0 {
        if e % 2 == 1 {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        e /= 2;
    }
    return result;
}


fn miller_test(n: u128, confidence: u128) -> bool {
    let mut k : u128 = 0;
    let mut m : u128 = n - 1;
    let mut a : u128; 
    let mut x : u128;
    let mut broken : bool;

    while m % 2 == 0 {
        k += 1;
        m /= 2;
    }

    for _ in 0..confidence {
        a = generate_random_number(2, n-2);

        x = pow_mod(a, m, n);

        if x == 1 || x == n - 1 {
            continue;
        }     
        broken = false;
        for _ in 0..(k - 1) {
            x = pow_mod(x, 2, n);
            if x == n - 1 {
                broken = true;
                break;
            }
        }

        if !broken {
            return false;
        }
    }

    return true;
}

fn is_probably_prime(n : u128) -> bool {
    if n == 3 || n == 2 { return true; }

    let prime_numbers : [u128; 10] = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29];

    for p in prime_numbers.iter() {
        if n % p == 0 {
            return false;
        }
    }

    return miller_test(n, 10);
}

fn extended_gcd(a: u128, b: u128) -> (i128, i128, u128) {
    if b == 0 {
        (1, 0, a)
    } else {
        let (x, y, gcd) : (i128, i128, u128) = extended_gcd(b, a % b);
        (y, x - (a / b) as i128 * y, gcd)
    }
}

fn mod_inverse(phi: u128, e: u128) -> u128 {
    let (x, _y, _gcd) = extended_gcd(e, phi);
    let x = if x < 0 { x + phi as i128 } else { x }; // Ensure x is non-negative
    x as u128 % phi
}

fn generate_prime(bits: u32) -> u128 {
    assert!(bits >= 10, "Number of bits must be at least 10");

    let min = 2u128.pow(bits - 10);
    let max = 2u128.pow(bits) - 1;
    let mut n_test = generate_random_number(min, max);

    if is_pair(n_test) {
        n_test += 1;
    }

    while !is_probably_prime(n_test) {
        n_test += 2;
    }

    n_test
}

fn main() {
    // message (integers only)
    let m: u128 = generate_random_number(2u128.pow(10), 2u128.pow(20) - 1);

    // generating big prime numbers
    let p: u128 = generate_prime(32); // Assuming 100 bits for prime generation
    let q: u128 = generate_prime(32);

    // calculate the modulus for private and public keys
    let n: u128 = p * q;

    // calculate Euler's totient
    let phi: u128 = (p - 1) * (q - 1);

    // pick random public exponent 1 < e < phi
    let e: u128 = 65537; // Best for security and performance

    // compute private exponent
    let d: u128 = mod_inverse(phi, e);

    // form public and private keys
    let public_key: (u128, u128) = (n, e);
    let private_key: (u128, u128) = (n, d);

    // encryption of m
    let c: u128 = pow_mod(m, public_key.1, public_key.0);

    // decryption of c
    let new_m: u128 = pow_mod(c, private_key.1, private_key.0);

    // Check if the decryption is correct
    println!("Decryption is correct: {}", new_m == m);

        // Print the values
        println!("m: {}", m);
        println!("p: {}", p);
        println!("q: {}", q);
        println!("n: {}", n);
        println!("phi: {}", phi);
        println!("e: {}", e);
        println!("d: {}", d);
        println!("new_m: {}", new_m);

}