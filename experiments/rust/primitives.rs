use std::fmt;

struct Matrix(f32, f32, f32, f32);

fn transpose(m: Matrix) -> Matrix {
    // `let` can be used to bind the members of a tuple to variables
    Matrix(m.0, m.2, m.1, m.3)
}

impl fmt::Display for Matrix {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "( {} {} )\n( {} {} )", self.0, self.1, self.2, self.3)
    }
}

fn main() {
    let matrix = Matrix(1.1, 1.2, 2.1, 2.2);
    println!("Matrix:\n{}", matrix);
    println!("Transpose::\n{}", transpose(matrix));

    //i = 5;
    println!("ARRAYS AND SLICES");
    // Arrays: Fixed length, known at compile time
    let arr_five_nums: [i32; 5] = [1, 5, 3, 4, 2];
    let arr_ten_zeros: [i32; 10] = [0; 10];
    println!("{:?}", arr_five_nums);
    println!("{:?}", arr_ten_zeros);
    // These line will break the build or, if `let i = 5` is moved up,
    // lead to panic
    //// let i = 5;
    //// println!("{}", arr_five_nums[i]);
    //
    // Slices: Similar to arrays, but length unknown at compile time.
    // Essentially consists of (pointer to first element, length)
    // Arrays can be automatically borrowed as slices
    let slice: &[i32] = &arr_five_nums[1..4];
    println!("{:?}", slice);
}
