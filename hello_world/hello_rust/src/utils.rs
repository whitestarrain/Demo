macro_rules! trace {
    // 情况1：格式字符串 + 参数
    ($fmt:literal $(, $args:expr)* $(,)?) => {
        print!("[{}:{}] ", file!(), line!());
        println!($fmt $(, $args)*);
    };
    // 情况2：一个或多个表达式（非字符串字面量），打印 表达式: Debug值
    ($($expr:expr),+ $(,)?) => {
        print!("[{}:{}] ", file!(), line!());
        $(
            print!("{}: {:?}  ", stringify!($expr), $expr);
        )+
        println!();
    };
}

pub(crate) use trace;
