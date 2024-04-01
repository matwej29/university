use std::io;

use undetermined_finite_automaton::{Dfa, Nfa};


fn main() {
    let mut input_regex = String::new();
    io::stdin().read_line(&mut input_regex).expect("Error while read input");
    let input_regex = input_regex.trim();

    let mut input_string = String::new();
    io::stdin().read_line(&mut input_string).expect("Error while read input");
    let input_string = input_string.trim();

    // let nfa = Nfa::build_from_regex(input_regex);
    let nfa = Nfa::build_like_syntax_analysis(input_regex);

    if let Ok(nfa) = nfa {
        let dfa = Dfa::build_from_nfa(&nfa);
        println!("{}", Dfa::check_string(&dfa, &input_string));
    } else {
        println!("Error");
    }

    // for (state, neighbors) in &nfa.nfa{
    //     for (other_state, symbol) in neighbors {
    //         println!("{state}(({state})) -->|{symbol}| {other_state}(({other_state}))");
    //     }
    // }
    //
    // println!("DFA:::");
    //
    // let dfa = Dfa::build_from_nfa(&nfa);
    //
    // println!("Finishes {:?}", dfa.ends);
    //
    // for (state, neighbors) in &dfa.dfa{
    //     for (other_state, symbol) in neighbors {
    //         println!("{state}(({state})) -->|{symbol}| {other_state}(({other_state}))");
    //     }
    // }
}
