struct Struct {
    e: i32,
}

pub fn variability() {
    let x = 5;
    println!("the value of x is {}", x);
    // x = 6; // error

    let mut y = 5;
    println!("the value of y is {}", y);
    y = 6;
    println!("the value of y is {}", y);

    let _unused_variable = 10;
}

pub fn extract() {
    let (a, mut b): (bool, bool) = (true, false);
    println!("a = {:?}, b={:?}", a, b);
    b = true;
    assert_eq!(a, b);

    let (a, b, c, d, e);
    (a, b) = (1, 2);
    [c, .., d, _] = [1, 2, 3, 4, 5];
    Struct { e, .. } = Struct { e: 5 };
    assert_eq!([1, 2, 1, 4, 5], [a, b, c, d, e]);
}

pub fn variable_shadowing() {
    let x = 5;
    let x = x + 1;
    println!("x is {x}");

    let spaces = "     ";
    let spaces = spaces.len();
    println!("space num is {spaces}");
}

pub fn run() {
    variability();
    extract();
    variable_shadowing();
}
