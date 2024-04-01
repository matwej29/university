from ipn_magic import calculate_ops

s = input()
l = len(s)
i = 0

RPN = []


def out_of_bounds():
    return i >= l


def Expr():
    if out_of_bounds(): return False
    if not Term(): return False
    if not ExprQ(): return False
    return True


def ExprQ():
    global s, i
    if out_of_bounds(): return True
    if s[i] in "+-":
        op = s[i]
        i += 1
        if not Term(): return False
        RPN.append(op)
        if not ExprQ(): return False
    return True


def Term():
    if out_of_bounds(): return False
    if not Factor(): return False
    if not TermQ(): return False
    return True


def TermQ():
    global s, i
    if out_of_bounds(): return True
    if s[i] in "*/":
        op = s[i]
        i += 1
        if not Factor(): return False
        RPN.append(op)
        if not TermQ(): return False
    return True


def Factor():
    global s, i
    if out_of_bounds(): return False
    if "0" <= s[i] <= "9":
        RPN.append(int(s[i]))
        i += 1
        return True
    if out_of_bounds(): return False
    if s[i] != "(":
        return False
    i += 1
    if not Expr(): return False
    if out_of_bounds(): return False
    if s[i] != ")":
        return False
    i += 1
    return True


if __name__ == "__main__":
    if Expr() and i >= l:
        print(calculate_ops(RPN))
    else:
        print("ERROR: incorrect input")
