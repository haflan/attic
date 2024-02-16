// To create this modtest module, make a file modtest.rs or a modtest/mod.rs
mod modtest;

// Define module in block
mod here_mod {
    pub fn funcy(s: &str) {
        println!("{}", s)
    }
}

fn main() {
    here_mod::funcy("Will print");
    modtest::external();
    let s = modtest::nested::nestfunc();
    println!("{}", s)
}
