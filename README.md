# Compiler-Principle-2023 Fall-Lab-Repo

This is repository contains a toy teaching compiler front-end and back-end combined with an interpreter.
The back-end will eventually generate a subset of RISC-V ISA assembly which could be executed on a simulator.
[*Note: Under heavily construction*]

sample tinyc program:
```Rust
struct Info {
    name: str,
    index: int,
    count: int
}

fn max(a: int, b: int) -> int {
    if(a >= b) {return a;}
    return b;
}

fn calculate(num: int) -> int {
    return num * num + max(num, 10);
}

fn total(info: &Info) -> void {
    if(info.index < 10) {return;}
    else {
        info.count = calculate(info.count - info.index);
        return;
    }
}

fn fib(x: int) -> int {
    if(x <= 2) {return 1;}
    return fib(x - 1) + fib(x - 2);
}

let hello_world: str = "hello world!";
fn main() -> int {
    let k: int = 6;
    let array[4]: int = {1, 2, fib(k + 1), 3};
    print(array[1]);
    let info: Info = Info{"Bob", 10, 17};
    print(info.name);
    print(info.index);
    print(info.count);
}
```
