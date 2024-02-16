// Will look for nested.rs (or nested/mod.rs) and make it a public nested module
pub mod nested;

pub fn external() {
    println!("Nice, this too is a module");
}