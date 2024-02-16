fn main() {
    /*
     * Formatted print
     */
    println!("My name is {0}, {1} {0}", "Bond", "James");
    let pi = 3.141592;
    // Implicit position
    println!("Pi is roughly {:.2}", pi);
    // Explicit position
    println!("Pi is roughly {0:.2}", pi);
    // Named
    println!("Pi is roughly {num:.2}", num = pi);

    /*
     * Debug
     */
    println!("DEBUG");

    // v derive(DEBUG) trait lets you use {:?} in println!, but {} though
    #[derive(Debug)]
    struct Person<'a> {
        name: &'a str,
        age: u8,
    }
    let name = "Peter";
    let age = 27;
    let peter = Person { name, age };

    // Pretty print
    println!("{:?}", peter);

    /*
     * Display
     */
    use std::fmt;
    // Define a structure where the fields are nameable for comparison.
    #[derive(Debug)]
    struct Complex {
        real: f64,
        imag: f64,
    }

    // Implement `Display`, which defines how `{}` prints the struct in `println!`
    // v derive(DEBUG) trait lets you use {:?} in println!, but {} though
    impl fmt::Display for Complex {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            // Customize so only `x` and `y` are denoted.
            write!(f, "{} + {}i", self.real, self.imag)
        }
    }

    println!("DISPLAY");
    println!(
        "Display: {}",
        Complex {
            real: 3.3,
            imag: 7.2
        }
    );

    println!(
        "Debug: {:?}",
        Complex {
            real: 3.3,
            imag: 7.2
        }
    );

    println!("TESTCASE: LIST");
    // Define a structure named `List` containing a `Vec`.
    struct List(Vec<i32>);

    impl fmt::Display for List {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            // Extract the value using tuple indexing,
            // and create a reference to `vec`.
            let vec = &self.0;

            write!(f, "[")?;

            // Iterate over `v` in `vec` while enumerating the iteration
            // count in `count`.
            for (count, v) in vec.iter().enumerate() {
                // For every element except the first, add a comma.
                // Use the ? operator to return on errors.
                if count != 0 {
                    write!(f, ", ")?;
                }
                write!(f, "{}: {}", count, v)?;
            }

            // Close the opened bracket and return a fmt::Result value.
            write!(f, "]")
        }
    }
    let v = List(vec![1, 2, 3]);
    println!("{}", v);

    println!("FORMATTING");

    #[derive(Debug)]
    struct Color {
        red: u8,
        green: u8,
        blue: u8,
    }

    impl fmt::Display for Color {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            write!(
                f,
                "RGB ({0}, {1}, {2}) 0x{0:02X}{1:02X}{2:02X}",
                self.red, self.green, self.blue
            )
        }
    }
    for color in [
        Color {
            red: 128,
            green: 255,
            blue: 90,
        },
        Color {
            red: 0,
            green: 3,
            blue: 254,
        },
        Color {
            red: 0,
            green: 0,
            blue: 0,
        },
    ]
    .iter()
    {
        // Switch this to use {} once you've added an implementation
        // for fmt::Display.
        println!("{}", *color);
    }
}
