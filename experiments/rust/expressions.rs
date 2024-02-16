fn main() {
    let res = {
        let first = 4;
        let squared = first * first;
        // Last expression is assigned to res, *unless it ends with a semicolon*
        squared - 6
    };
    println!("4*4-6={}", res);
}
