// Custom types
//  struct   : structure
//  enum     : enumeration

// Structs
//  Tuple structs   : Essentially named tuples, i.e. fields
struct Tuple(i32, i32, i32, i32);
//  Classic structs : Named fields, like C
#[derive(Debug)]
struct Classic {
    a_field: i32,
    b_field: String,
}
//  Unit structs    : Field-less, useful for generics
struct Unit;

enum Status {
    InSync,
    Syncing,
    Error,
}

enum StatusExplicit {
    InSync = 1,
    Syncing = 2,
    Error = 3,
}

impl Status {
    // Instance function
    fn print(&self, prefix: String) {
        let str_status = match self {
            Self::InSync => "all good",
            Self::Syncing => "working!",
            Self::Error => "oh no",
        };
        println!("{} {}", prefix, str_status);
    }
    // Like Java static function
    fn get_error() -> Status {
        return Status::Error;
    }
}

const STATUS_PREFIX: &str = "It's";

fn main() {
    let c = Classic {
        a_field: 234,
        b_field: String::from("Some string"),
    };
    println!("{:?}", c);
    let t = Tuple(1, 3, 5, 7);
    println!("{:?}", t.1);
    let s = Status::Syncing;
    // Call .print on the instance
    s.print(String::from(STATUS_PREFIX));
    // `use`, like used here, makes the `Status::` prefix unnecessary.
    // Notice the wildcard to include all instead of individual fields
    use crate::Status::*;
    InSync.print(String::from("Now it's"));
    println!("Implicit discriminator: {}", InSync as i32);
    println!("Explicit: {}", StatusExplicit::InSync as i32);
    println!("Static function: {}", Status::get_error() as i32)
}
