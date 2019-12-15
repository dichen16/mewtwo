use std::env;

fn main() {
    println!("Hello, world!");
    //variable binding
    let x = 62;
    let y = 13;
    //funciton  
    print_sum(x, y);
    println!("{}", fib(10));

    let args: Vec<String> = env::args().collect();
    println!("{:?}", args);
}

// Rust  is an expression-based language, 
// and semicolons are different from semicolons 
// in other ‘curly brace and semicolon’-based languages. 
fn print_sum(x: i32, y: i32)  -> i32 {
    let sum = x + y;
    sum
}

fn fib(x: i32) -> i32
{
    if x <= 2
    {
        return 1;
    } 
    else 
    {
        return fib(x-1) + fib(x-2);
    }
}
