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
    let prime_numbers : [u128; 10] = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29];

    for p in prime_numbers.iter() {
        if n % p == 0 {
            return false;
        }
    }

    return miller_test(n, 10);
}

fn main() {
    println!("The number {} is pair : {}",5, is_pair(5));
    println!("{}**{} mod {} is {}",6, 678, 3, pow_mod(567, 67869876, 345));
    println!("{} is prime : {}",3190, is_probably_prime(3190));

}