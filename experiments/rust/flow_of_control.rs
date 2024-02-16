fn main() {
    let mut opt = 2;
    let text = if opt == 1 {
        "option one"
    } else if opt == 2 {
        "option two"
    } else {
        "other"
    };
    println!("{}", text);
    println!("NESTING AND LABELS");
    'outer: loop {
        loop {
            println!("inner, count: {}", opt);
            opt += 1;
            if opt > 5 {
                break 'outer;
            }
            if opt > 3 {
                break;
            }
        }
        println!("outer")
    }
    println!("RETURNING FROM LOOP");
    let result = loop {
        opt += 1;
        if opt == 10 {
            break opt * 2;
        }
    };
    assert_eq!(result, 20);
}
