use std::collections::{VecDeque, HashMap};
use crate::Nfa;

#[derive(Debug)]
#[derive(Copy, Clone)]
enum StackItem {
    Op(char),
    SubNfa {
        start: u64,
        end: u64,
    },
}

pub struct IncorrectRegexStr;


pub struct NfaBuilder {
    nfa: HashMap<u64, Vec<(u64, char)>>,
    current_state: u64,

    input_str: Vec<char>,
    current_i: usize,
}

impl NfaBuilder {
    /// ### Описание алгоритма:
    /// Бежим по регулярному выражению справа налево,
    /// превращая символы и замыкания символа в подграфы НКА
    /// (после посторения этого подграфа, делается связывание с предыдущим, чтобы получить один элемент).
    /// А также передвигая операторы выбора, |,
    /// выше по стеку так, чтобы под ним лежали два операнда (т.е. некое подобие обратной польской записи)
    /// После первого процессинга получим стек, заполненный подграфами НКА (SubNfa)
    /// и операторами: *|, а также скобками
    ///
    /// Затем работа со стеком (сверхну-вниз)
    /// (тоже некое подобие обратной польской записи и алгоритма сортировочной станции):
    /// Если на стеке лежат скобки вокруг одного SubNfa, то такие скобки убираем.
    /// Если это оператор, то делаем соответствующие операции (замыкание или выбор) и кладем обратно на стек.
    /// После любого изменения стека продолжается работа начиная опять с верхушки.
    ///
    /// Эту реализацию можно довольно просто обогатить операцией усеченной итерации.
    pub fn build_from_regex(regex_str: &str) -> Nfa {
        let mut builder: NfaBuilder = NfaBuilder { nfa: HashMap::new(), current_state: 0, input_str: vec![], current_i: 0 };

        let mut stack: VecDeque<StackItem> = VecDeque::new();

        for c in regex_str.chars().rev() {
            if c == ' ' { continue; }
            if c.is_alphabetic() {
                let (current_start, current_end) = builder.char_nka(c);

                if let Some(item) = stack.back() {
                    let item = *item; // copy what's on top of stack
                    match item {
                        StackItem::Op(op) => {
                            if op == '*' {
                                let (start, end) =
                                    builder.closure_nka(current_start, current_end);

                                stack.pop_back();
                                stack.push_back(StackItem::SubNfa { start, end }); // вместо N положили N*

                                continue;
                            } else if op == '|' {
                                // оператор выбора сдвигаем вверх стека, чтобы под ним лежало два нка
                                stack.push_back(StackItem::SubNfa { start: current_start, end: current_end });
                                let len = stack.len();
                                stack.swap(len - 1, len - 2);

                                continue;
                            }
                        }
                        StackItem::SubNfa { start, end } => {
                            builder.chain(current_end, start);

                            stack.pop_back();
                            stack.push_back(StackItem::SubNfa {
                                start: current_start,
                                end,
                            });

                            continue;
                        }
                    }
                };

                stack.push_back(StackItem::SubNfa { start: current_start, end: current_end });
            } else {
                stack.push_back(StackItem::Op(c));
            }
        }


        while stack.len() > 1 {
            let mut current = stack.len();
            for item in stack.iter().rev() {
                current -= 1;
                match item {
                    StackItem::Op(op) => {
                        if *op == '(' {
                            if current < 2 { panic!("error case, stack len = {current}") }
                            // текущий элемент - (
                            // потом - НКА
                            // и еще ниже - )
                            if let Some(item) = stack.get(current - 2) {
                                match item {
                                    StackItem::Op(op) => {
                                        if *op == ')' {
                                            // удаляем две скобки
                                            stack.swap(current, current - 1); // кладем нка выше
                                            // и тогда ниже нка будут лежать две скобки
                                            stack.remove(current - 2);
                                            // после удаления нижней, верхняя будет на ее месте
                                            stack.remove(current - 2);

                                            // повторный код
                                            if let Some(StackItem::SubNfa { start, end }) = stack.get(current - 2) {
                                                if let Some((new_start, new_end)) = builder.chain_with_prev(&stack,
                                                                                                            *start,
                                                                                                            *end,
                                                                                                            current - 3) {
                                                    stack.remove(current - 2);
                                                    stack.push_back(StackItem::SubNfa { start: new_start, end: new_end });
                                                    stack.swap_remove_back(current - 3);
                                                    break;
                                                }
                                            }
                                            break;
                                        }
                                    }
                                    StackItem::SubNfa { .. } => {}
                                }
                            }
                        } else if *op == '|' {
                            let take_nka_from_stack = |num| {
                                stack.get(num).and_then(|item| match item {
                                    StackItem::Op(_) => { None }
                                    StackItem::SubNfa { start, end } => { Some((*start, *end)) }
                                })
                            };

                            let first: Option<(u64, u64)> = take_nka_from_stack(current - 1);
                            let second: Option<(u64, u64)> = take_nka_from_stack(current - 2);

                            if first.is_some() && second.is_some() {
                                // убрать из стека один нка и плюс
                                // второй заменить на построенный
                                stack.remove(current - 1);
                                stack.remove(current - 1);
                                if let Some(item) = stack.get_mut(current - 2) {
                                    // unwrap может запаниковать, но не должен из-за условия выше
                                    let (new_start, new_end) = builder.choice_nka(first.unwrap(), second.unwrap());
                                    *item = StackItem::SubNfa { start: new_start, end: new_end };
                                }
                                break;
                            }
                        }
                    }
                    StackItem::SubNfa { start, end } => {
                        // повторный код
                        if let Some((new_start, new_end)) = builder.chain_with_prev(&stack,
                                                                                    *start,
                                                                                    *end,
                                                                                    current - 1) {
                            stack.remove(current - 1);
                            stack.push_back(StackItem::SubNfa { start: new_start, end: new_end });
                            stack.swap_remove_back(current - 1);
                            break;
                        } else if let Some(StackItem::Op(op)) = stack.get(current - 1) {
                            if *op == '|' {
                                stack.swap(current, current - 1);
                                break;
                            }
                        }
                    }
                }
            }

            if current == 0 {
                // имеем полностью построенный НКА
                break;
            }
        }

        if let Some(StackItem::SubNfa { start, end }) = stack.back() {
            return Nfa { nfa: builder.nfa, start: *start, end: *end };
        }

        eprintln!("wrong regex");
        Nfa { nfa: builder.nfa, start: 0, end: 0 }
    }

    fn chain_with_prev(&mut self,
                       stack: &VecDeque<StackItem>,
                       current_start: u64,
                       current_end: u64,
                       prev: usize) -> Option<(u64, u64)>
    {
        if let Some(item) = stack.get(prev) {
            return match item {
                StackItem::Op(op) => {
                    if *op == '*' {
                        Some(self.closure_nka(current_start, current_end))
                    } else {
                        None
                    }
                }
                StackItem::SubNfa { start, end } => {
                    self.chain(current_end, *start);

                    Some((current_start, *end))
                }
            };
        }

        None
    }

    /// Takes end state number of first undermined finite automaton
    /// and state number of start second undermined automaton and connects them.
    ///
    /// i -N(s)-> j; k -N(t)-> l;
    ///
    /// after chain() will be
    ///
    /// i -N(s)-> j -> k -N(t)-> l;
    ///
    /// Both of nka must be in self.
    fn chain(&mut self, first_end: u64, second_start: u64) {
        self.nfa.entry(first_end)
            .and_modify(|nei| nei.push((second_start, ' ')));
    }

    /// Builds (inside self) undetermined finite automaton from one char-transition
    ///
    /// # Example
    /// 0 --a--> 1
    ///
    /// transition from state 0 to state 1 by character 'a'
    ///
    /// Uses two new states
    fn char_nka(&mut self, tran: char) -> (u64, u64) {
        let char_start = self.current_state;
        let char_end = self.current_state + 1;

        self.nfa.insert(char_start, vec![(char_end, tran)]);
        self.nfa.insert(char_end, vec![]);

        self.current_state += 2;

        (char_start, char_end)
    }

    /// Builds closure of given subnka
    ///
    /// Uses two new states
    fn closure_nka(&mut self, subnka_start: u64, subnka_end: u64) -> (u64, u64) {
        let closure_start = self.current_state;
        self.nfa.entry(subnka_end).and_modify(|nei|
            nei.push((subnka_start, ' ')));

        self.current_state += 1;
        let closure_end = self.current_state;
        self.nfa.insert(closure_start, vec![(subnka_start, ' '), (closure_end, ' ')]);
        self.nfa.insert(closure_end, vec![]);

        self.nfa.entry(subnka_end).and_modify(|nei|
            nei.push((closure_end, ' ')));

        self.current_state += 1;

        (closure_start, closure_end)
    }

    fn choice_nka(&mut self, first: (u64, u64), second: (u64, u64)) -> (u64, u64) {
        let choice_start = self.current_state;
        // мы должны создать новое стартовое состояние
        // и из него сделать е-переход в начало первого нка, второго нка
        self.nfa.insert(choice_start, vec![(first.0, ' '), (second.0, ' ')]);

        self.current_state += 1;
        let choice_end = self.current_state;
        self.nfa.insert(choice_end, vec![]);

        // из концов первого и второго нка перейти в еще одно новое конечное состояние
        self.nfa.entry(first.1).and_modify(|nei|
            nei.push((choice_end, ' ')));
        self.nfa.entry(second.1).and_modify(|nei|
            nei.push((choice_end, ' ')));

        self.current_state += 1;

        (choice_start, choice_end)
    }

    fn out_of_bounds(&self) -> bool {
        self.current_i >= self.input_str.len()
    }

    pub fn build_like_syntax_analysis(regex_str: &str) -> Result<Nfa, IncorrectRegexStr> {
        let mut builder: NfaBuilder = NfaBuilder {
            nfa: HashMap::new(),
            current_state: 0,

            input_str: regex_str.chars().collect::<Vec<_>>(),
            current_i: 0,
        };

        match builder.S() {
            Ok((start, end)) => {
                if builder.out_of_bounds() {
                    Ok(Nfa {
                        nfa: builder.nfa,
                        start,
                        end,
                    })
                } else {
                    Err(IncorrectRegexStr)
                }
            }
            Err(err) => { Err(err) }
        }
    }

    fn S(&mut self) -> Result<(u64, u64), IncorrectRegexStr> {
        if self.out_of_bounds() { return Err(IncorrectRegexStr); };
        let q = self.T()?;
        let x = self.SQ(q)?;
        Ok(x)
    }

    fn SQ(&mut self, param: (u64, u64)) -> Result<(u64, u64), IncorrectRegexStr> {
        if self.out_of_bounds() { return Ok(param); };

        if self.input_str.get(self.current_i).is_some_and(|c| *c == '+') {
            self.current_i += 1;
            let x = self.T()?;

            let temp = self.choice_nka(x, param);
            let result = self.SQ(temp)?;

            return Ok(result);
        }
        Ok(param)
    }

    fn T(&mut self) -> Result<(u64, u64), IncorrectRegexStr> {
        if self.out_of_bounds() { return Err(IncorrectRegexStr); };
        let x = self.F()?;
        let result = self.TQ(x)?;

        Ok(result)
    }

    fn TQ(&mut self, param: (u64, u64)) -> Result<(u64, u64), IncorrectRegexStr> {
        if self.out_of_bounds() { return Ok(param); };
        let temp_i = self.current_i;
        let x = self.F();

        if let Ok(x) = x {
            let new_nfa = (param.0, x.1);
            self.chain(param.1, x.0);

            let result = self.TQ(new_nfa)?;
            return Ok(result);
        }
        self.current_i = temp_i;

        Ok(param)
    }

    fn F(&mut self) -> Result<(u64, u64), IncorrectRegexStr> {
        if self.out_of_bounds() { return Err(IncorrectRegexStr); };

        if let Ok(x) = self.X() {
            if self.input_str.get(self.current_i).is_some_and(|c| *c == '*') {
                self.current_i += 1;
                return Ok(self.closure_nka(x.0, x.1));
            }
            return Ok(x);
        }

        if self.input_str.get(self.current_i).is_some_and(|c| *c == '(') {
            self.current_i += 1;
            let result = self.S()?;

            if self.input_str.get(self.current_i).is_some_and(|c| *c == ')') {
                self.current_i += 1;
            } else {
                return Err(IncorrectRegexStr);
            }
            if self.input_str.get(self.current_i).is_some_and(|c| *c == '*') {
                self.current_i += 1;
                return Ok(self.closure_nka(result.0, result.1));
            }
            return Ok(result);
        }

        Err(IncorrectRegexStr)
    }

    fn X(&mut self) -> Result<(u64, u64), IncorrectRegexStr> {
        if let Some(symbol) = self.input_str.get(self.current_i) {
            if ['0', '1'].contains(symbol) {
                self.current_i += 1;
                return Ok(self.char_nka(*symbol));
            }
        }

        Err(IncorrectRegexStr)
    }
}
