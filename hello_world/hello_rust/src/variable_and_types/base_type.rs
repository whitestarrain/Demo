use crate::utils::trace;

pub fn type_number() {
    let i: i8 = 120;
    // overflow
    // let i = i + 20;
    trace!(i);

    assert_eq!(u8::MAX.saturating_add(127), u8::MAX);
}
pub fn type_float() {
    let x: f32 = 2.0;
    trace!(x);
    trace!(0.1 + 0.2 == 0.3);
    trace!((0.1_f64 + 0.2 - 0.3).abs() < 0.00001); // _f64 是类型后缀

    let abc: (f32, f32, f32) = (0.1, 0.2, 0.3);
    let xyz: (f64, f64, f64) = (0.1, 0.2, 0.3);

    trace!("abc (f32)");
    trace!("   0.1 + 0.2: {:x}", (abc.0 + abc.1).to_bits());
    trace!("         0.3: {:x}", (abc.2).to_bits());
    trace!(abc.0 + abc.1 == abc.2);
    trace!("xyz (f64)");
    trace!("   0.1 + 0.2: {:x}", (xyz.0 + xyz.1).to_bits());
    trace!("         0.3: {:x}", (xyz.2).to_bits());
    trace!(xyz.0 + xyz.1 == xyz.2);
}
pub fn bit_cal() {
    let a: u8 = 2; // 00000010
    let b: u8 = 3; // 00000011
    trace!("a value is {:08b}", a);
    trace!("b value is {:08b}", b);
    trace!("a&b: {:08b}", a & b);
    trace!("a|b: {:08b}", a | b);
    trace!("a^b: {:08b}", a ^ b);
    trace!("!a: {:08b}", !a);
    trace!("a>>b: {:08b}", a >> b);
    trace!("a<<b: {:08b}", a << b);
    let mut ma = 2;
    ma <<= 2;
    trace!(ma);

    // overflow
    // let oa:u8 = 255;
    // let oa = oa << 8;
}

pub fn syntax_range(){
    for i in 1..=5 {
        trace!(i);
    }
    for c in 'a'..='b' {
        trace!(c);
    }
}

pub fn type_char() {
    let c1 = 'a';
    let c2 = '我';
    let c3 = '😄';
    trace!(c1);
    trace!(c2);
    trace!(c3);
    trace!(size_of_val(&c1));
    trace!(size_of_val(&c2));
}

pub fn type_str() {}
pub fn type_slice() {}
pub fn type_bool() {}
pub fn type_unit() {}

pub fn run() {
    type_number();
    type_float();
    bit_cal();
    syntax_range();
    type_char();
    type_str();
    type_slice();
    type_bool();
    type_unit();
}
