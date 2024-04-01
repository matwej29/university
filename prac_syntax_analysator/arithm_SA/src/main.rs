struct SA<'a> {
    input_str: &'a str,
    i: usize,
}

impl SA {
    // pub fn new(input_str: &str, i: usize) -> Self {
    //     Self { input_str, i }
    // }
    fn out_of_bounds(&self) -> bool {
        self.i > self.input_str.len()
    }

    fn Expr(&self) -> bool {
        if self.out_of_bounds() { return false }

        if !self.Term() { return false }
        if !self.Expr_() { return false }

        true
    }

    fn Term(&self) -> bool {

        true
    }

    fn Expr_(&self) -> bool{
        if self.out_of_bounds() { return true }


        true
    }
}


fn main() {
    println!("Hello, world!");
}
