mod nfa_builder;

use std::collections::{HashMap, BTreeSet, VecDeque, HashSet};
use crate::nfa_builder::IncorrectRegexStr;


pub struct Nfa {
    pub nfa: HashMap<u64, Vec<(u64, char)>>,
    pub start: u64,
    pub end: u64,
}

impl Nfa {
    pub fn build_from_regex(regex_str: &str) -> Nfa {
        nfa_builder::NfaBuilder::build_from_regex(regex_str)
    }

    pub fn build_like_syntax_analysis(regex_str: &str) -> Result<Nfa, IncorrectRegexStr> {
        nfa_builder::NfaBuilder::build_like_syntax_analysis(regex_str)
    }
}


type V = BTreeSet<u64>;

pub struct DfaBuilder(pub HashMap<V, Vec<(char, V)>>);

impl DfaBuilder {
    pub fn build_from_nfa(nfa: &Nfa) -> Dfa {
        let mut dfa: HashMap<V, Vec<(char, V)>> = HashMap::new();
        let mut processed: HashSet<V> = HashSet::new();
        let mut queue: VecDeque<V> = VecDeque::new();

        let mut new_start: V = V::new();
        DfaBuilder::e_closure(nfa, nfa.start, &mut new_start);
        queue.push_back(new_start.clone());
        while !queue.is_empty() {
            let v = queue.pop_back().unwrap();

            if processed.contains(&v) {
                continue;
            }
            dfa.entry(v.clone()).or_default();

            for transition in DfaBuilder::transitions(&v, nfa) {
                dfa.entry(v.clone())
                    .and_modify(|e| e.push(transition.clone()))
                    .or_insert(vec![transition.clone()]);
                queue.push_back(transition.1);
            }

            processed.insert(v);
        }

        let mut numbering: HashMap<V, u64> = HashMap::new();

        for (number, (v, _)) in dfa.iter().enumerate() {
            numbering.insert(v.clone(), number as u64);
        }

        let mut new_ends: Vec<u64> = vec![];
        for (v, number) in &numbering {
            if v.contains(&nfa.end) {
                new_ends.push(*number);
            }
        }

        let mut result_dfa: Dfa = Dfa {
            dfa: Default::default(),
            start: *numbering.get(&new_start).unwrap(),
            ends: new_ends,
        };

        for (v, transitions) in dfa {
            result_dfa.dfa.insert(*numbering.get(&v).unwrap(),
                                  transitions.iter()
                                      .map(|x| (*numbering.get(&x.1).unwrap(), x.0))
                                      .collect());
        }

        result_dfa
    }

    fn e_closure(nfa: &Nfa, v: u64, visited: &mut BTreeSet<u64>) {
        visited.insert(v);

        for (neighbor, transition) in &nfa.nfa[&v] {
            if *transition == ' ' && !visited.contains(neighbor) {
                DfaBuilder::e_closure(nfa, *neighbor, visited);
            }
        }
    }

    fn transitions(source: &V, nfa: &Nfa) -> HashMap<char, V> {
        let mut outside: HashMap<char, V> = HashMap::new();

        for v in source {
            for edge in &nfa.nfa[&v] {
                if edge.1 == ' ' {
                    continue;
                }
                outside.entry(edge.1)
                    .and_modify(|e| { e.insert(edge.0); })
                    .or_insert(V::from([edge.0]));
            }
        }

        for pair in &mut outside {
            for x in pair.1.clone() {
                DfaBuilder::e_closure(nfa, x, pair.1);
            }
        }

        outside
    }
}

pub struct Dfa {
    pub dfa: HashMap<u64, Vec<(u64, char)>>,
    pub start: u64,
    pub ends: Vec<u64>,
}

impl Dfa {
    pub fn build_from_nfa(nfa: &Nfa) -> Dfa {
        DfaBuilder::build_from_nfa(nfa)
    }

    pub fn check_string(dfa: &Dfa, input: &str) -> bool {
        let mut current_state = dfa.start;
        let mut input_iter = input.chars().peekable();
        'main: loop {
            if let Some(edges) = dfa.dfa.get(&current_state) {
                for edge in edges {
                    if let Some(current) = input_iter.peek() {
                        if *current == edge.1 {
                            current_state = edge.0;
                            input_iter.next();
                            if dfa.ends.contains(&current_state) && input_iter.peek().is_none() {
                                return true;
                            }
                            continue 'main;
                        }
                    }
                }
                return false;
            }
            return false;
        }
    }
}
