// Converting between custom types uses the From trait.
use std::fmt;

struct Complex {
    real: f32,
    imag: f32,
}

impl fmt::Display for Complex {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        if self.imag == 0f32 {
            write!(f, "{}", self.real)?;
        } else {
            write!(f, "{} + {}i", self.real, self.imag)?;
        };
        Ok(())
    }
}

impl From<f32> for Complex {
    fn from(item: f32) -> Self {
        Complex {
            real: item,
            imag: 0f32,
        }
    }
}

fn main() {
    // Create Complex from f32
    let c = Complex::from(3.52);
    println!("{}", c);
    // Convert f32 `into` Complex
    let ci: Complex = 8.49.into();
    println!("{}", ci);

    // try_from and try_into are alternatives that allow errors,
    // i.e. they return Result<Self, some_error_type>.
    // (Will probably learn more about Result in error handling chapter)

    // Converting a type into a String is a special case - the `ToString` trait.
    // Usually you implement the fmt::Display to automatically get the ToString trait
    // and allow printing with the `print!` macros as shown in `hellow_world.rs`.

    // Parsing a string
    let string = "8";
    let int: i32 = string.parse().unwrap();
    //let parsed:
    println!(
        "size of string '{}': {}",
        string,
        std::mem::size_of_val(string)
    );
    println!("size of parsed int: {}", std::mem::size_of_val(&int));
}
