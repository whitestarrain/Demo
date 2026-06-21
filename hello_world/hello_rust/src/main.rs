mod utils;

mod variable_and_types;
mod demo;

fn main() {
    demo::main_demo();
    variable_and_types::variable::run();
    variable_and_types::base_type::run();
}
