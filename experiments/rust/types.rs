fn main() {
    println!("CASTING");
    let decimal = 93.243_295_f32;
    let cast_u32 = decimal as u32;
    let cast_u8 = decimal as u8;
    // Only u8 can be cast to char (even though char is 32 bits)
    let cast_char = cast_u8 as char;
    println!("u8: '{}', char: '{}'", cast_u8, cast_char);

    println!("LITERALS");
    println!("size(u8): {}", std::mem::size_of_val(&cast_u8));
    println!("size(u32): {}", std::mem::size_of_val(&cast_u32));
    println!("size(char): {}", std::mem::size_of_val(&cast_char));
    println!("size(f32): {}", std::mem::size_of_val(&decimal));

    println!("ALIASING");
    type UnsignedInt128 = u128;
    let long = 8 as UnsignedInt128;
    println!("size(UnsignedInt128): {}", std::mem::size_of_val(&long));
}
