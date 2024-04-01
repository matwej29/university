s = "(0+1)*011(0+1)*"  # Регулярное выражение задается через глобальную переменную
# s = "11"
i = 0  # Текущий символ выражения для функции разбора (глобальная переменная)
l = len(s)


# Класс для состояния НКА
class State:
    currentNumber = 0
    states = []  # глобальный список всех вершин

    def __init__(self):
        self.name = State.currentNumber  # в качестве имени присваиваем порядковый номер
        State.states.append(self)  # сохраняем новую вершину в глобальном списке вершин (для отладки)
        State.currentNumber += 1
        self.edges = {}  # ребра храним в хеш-таблице:

    # ключ - символ, значение - список вершин, в которые ведут ребра, помеченные этим символом
    def addEdge(self, symbol, state):  # метод добавления ребра с меткой symbol, ведущего в вершину state
        if not (symbol in self.edges):
            self.edges[symbol] = []
        self.edges[symbol].append(state)

    def __repr__(self):
        return str(self.name)

    def __str__(self):
        return str(self.name)

    def printEdges(self):  # печатаем содержимое вершины
        for symbol in self.edges:
            print("\t'" + symbol + "': " + str(self.edges[symbol]))

    # функция печати НКА: выводит все вершины,
    # созданные на текущий момент, и их ребра
def printStates():
    for state in State.states:
        print("State " + str(state))
        state.printEdges()
    print()


# мини-класс для хранения НКА - хранит только начальное и конечное состояния НКА
class NSM:
    def __init__(self, start, stop):
        self.startState = start
        self.stopState = stop

    def __repr__(self):
        return "Start state is " + str(self.startState) + ", end state is " + str(self.stopState)

    # БАЗИС: создаем НКА для символа
    def makeSymbolNSM(symbol: str):
        start = State()
        stop = State()
        start.addEdge(symbol, stop)
        return NSM(start, stop)

    # ИНДУКЦИЯ: создаем автомат для регулярного выражения И
    def makeAndNSM(leftNSM, rightNSM):
        for symbol in rightNSM.startState.edges:
            for state in rightNSM.startState.edges[symbol]:
                leftNSM.stopState.addEdge(symbol, state)
        tmp = NSM(leftNSM.startState, rightNSM.stopState)
        State.states.remove(rightNSM.startState)
        return tmp

    # ИНДУКЦИЯ: создаем автомат для регулярного выражения ИЛИ
    def makeOrNSM(leftNSM, rightNSM):
        start = State()
        stop = State()
        start.addEdge('epsilon', leftNSM.startState)
        start.addEdge('epsilon', rightNSM.startState)
        leftNSM.stopState.addEdge('epsilon', stop)
        rightNSM.stopState.addEdge('epsilon', stop)
        return NSM(start, stop)

    # ИНДУКЦИЯ: создаем автомат для регулярного выражения *
    def makeClosureNSM(cNSM):
        start = State()
        stop = State()
        cNSM.stopState.addEdge('epsilon', cNSM.startState)
        start.addEdge('epsilon', cNSM.startState)
        start.addEdge('epsilon', stop)
        cNSM.stopState.addEdge('epsilon', stop)
        return NSM(start, stop)


def out_of_bounds():
    return i >= l


def S() -> bool | NSM:
    if out_of_bounds(): return False
    q = T()
    if not q:
        return False
    x = SQ(q)
    if not x:
        return False
    return x


def SQ(param: NSM) -> bool | NSM:
    global i
    if out_of_bounds(): return param
    if s[i] == '+':
        i += 1
        x = T()
        if not x: return False
        res = SQ(NSM.makeOrNSM(x, param))
        if not res: return False
        return res
    return param


def T() -> bool | NSM:
    global i
    if out_of_bounds(): return False
    x = F()
    if x:
        res = TQ(x)
        if not res: return False
        return res
    return False


def TQ(param: NSM) -> bool | NSM:
    global i
    if out_of_bounds(): return param
    current_i = i
    x = F()
    if x:
        res = TQ(NSM.makeAndNSM(x, param))
        if not res: return False
        return res
    i = current_i
    return param


def F() -> bool | NSM:
    global i
    if out_of_bounds(): return False
    if x := X():
        if i < l and s[i] == "*":
            i += 1
            return NSM.makeClosureNSM(x)
        return x

    if s[i] == "(":
        i += 1
        res = S()
        if not res: return False
        if out_of_bounds(): return False
        if s[i] == ")":
            i += 1
        else:
            return False
        if out_of_bounds(): return res
        if s[i] == "*":
            i += 1
            return NSM.makeClosureNSM(res)
        return res

    return False


def X() -> bool | NSM:
    global i
    if s[i] in "01":
        sym = s[i]
        i += 1
        return NSM.makeSymbolNSM(sym)
    return False


if __name__ == "__main__":
    nsm = S()
    print(nsm)
    print(nsm and i >= l)
    print(i)
    printStates()
