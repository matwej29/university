reg1 - 0
reg2 - 1
reg3 - 2
reg4 - 3
# inputs
a - 4
b - 5
c - 6
n - 7
m - 8
z - 9
x - 10
y - 11
s - 12
i - 13
j - 14
x1 - 15
x2 - 16
t - 17
t7 - 18
t9 - 19
t1 - 20
t2 - 21
t3 - 22
x = 9 * (c plus a):
Сумма c и a с результатом в t2:
zero reg1
<<<<<<<<<<<<<<<<<<<<<<[-]
zero reg2
>[-]
zero reg3
>[-]
copy from c to reg1
>>>>[-<<<<<<+>>+>>>>]
<<<<[->>>>+<<<<]
copy from a to reg2
>>[-<<<+>+>>]
<<[->>+<<]
copy from reg2 to reg1
<[-<+>>+<]
>[-<+>]
Передвинуть reg1 в t2
zero t2
>>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t2
<<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Произведение t9 и t2 с результатом в x:
zero reg4
>>>[-]
copy from t2 to reg4
>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<+<+>>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<]
>[
-zero reg2
<<[-]
copy from t9 to reg1
>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<]
>]
Передвинуть reg1 в x
zero x
>>>>>>>[-]
copy from reg1 to x
<<<<<<<<<<[->>>>>>>>>>+<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



y = (7 plus b) * (c minus a)
Сумма t7 и b с результатом в t1:
zero reg1
[-]
zero reg2
>[-]
zero reg3
>[-]
copy from t7 to reg1
>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<]
copy from b to reg2
>>>[-<<<<+>+>>>]
<<<[->>>+<<<]
copy from reg2 to reg1
<[-<+>>+<]
>[-<+>]
Передвинуть reg1 в t1
zero t1
>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t1
<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Разность c и a с результатом в t2:
zero reg1
[-]
zero reg2
>[-]
copy from c to reg1
>>>>>[-<<<<<<+>>+>>>>]
<<<<[->>>>+<<<<]
copy from a to reg2
>>[-<<<+>+>>]
<<[->>+<<]
<[
-<->]
Передвинуть reg1 в t2
zero t2
>>>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t2
<<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Произведение t1 и t2 с результатом в y:
zero reg4
>>>[-]
copy from t2 to reg4
>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<+<+>>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<]
>[
-zero reg2
<<[-]
copy from t1 to reg1
>>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<]
>]
Передвинуть reg1 в y
zero y
>>>>>>>>[-]
copy from reg1 to y
<<<<<<<<<<<[->>>>>>>>>>>+<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



for (i = 0; i less than n; inc(i))
copy from n to i
>>>>>>>[->>>>>>+<<<<<<<<<<<+>>>>>]
<<<<<[->>>>>+<<<<<]
>>>>>>>>>>>[
-int x = 7 * (a minus b)
Разность a и b с результатом в t1:
zero reg1
<<<<<<<<<<<<<[-]
zero reg2
>[-]
copy from a to reg1
>>>[-<<<<+>>+>>]
<<[->>+<<]
copy from b to reg2
>>>[-<<<<+>+>>>]
<<<[->>>+<<<]
<[
-<->]
Передвинуть reg1 в t1
zero t1
>>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t1
<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Произведение t7 и t1 с результатом в x1:
zero reg4
>>>[-]
copy from t1 to reg4
>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<+<+>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<]
>[
-zero reg2
<<[-]
copy from t7 to reg1
>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<]
>]
Передвинуть reg1 в x1
zero x1
>>>>>>>>>>>>[-]
copy from reg1 to x1
<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>+<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



y = (x plus b) * (c minus 9)
Сумма x1 и b с результатом в t1:
zero reg1
[-]
zero reg2
>[-]
zero reg3
>[-]
copy from x1 to reg1
>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>]
<<<<<<<<<<<<<[->>>>>>>>>>>>>+<<<<<<<<<<<<<]
copy from b to reg2
>>>[-<<<<+>+>>>]
<<<[->>>+<<<]
copy from reg2 to reg1
<[-<+>>+<]
>[-<+>]
Передвинуть reg1 в t1
zero t1
>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t1
<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Разность c и t9 с результатом в t2:
zero reg1
[-]
zero reg2
>[-]
copy from c to reg1
>>>>>[-<<<<<<+>>+>>>>]
<<<<[->>>>+<<<<]
copy from t9 to reg2
>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<+>+>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<]
<[
-<->]
Передвинуть reg1 в t2
zero t2
>>>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t2
<<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Произведение t1 и t2 с результатом в y:
zero reg4
>>>[-]
copy from t2 to reg4
>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<+<+>>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<]
>[
-zero reg2
<<[-]
copy from t1 to reg1
>>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<]
>]
Передвинуть reg1 в y
zero y
>>>>>>>>[-]
copy from reg1 to y
<<<<<<<<<<<[->>>>>>>>>>>+<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



for (j = 0; j less than m; inc(j))
copy from m to j
>>>>>>>>[->>>>>>+<<<<<<<<<<<<+>>>>>>]
<<<<<<[->>>>>>+<<<<<<]
>>>>>>>>>>>>[
-int t = (b plus x) * (z minus c)
int x = t
Сумма b и x1 с результатом в t1:
zero reg1
<<<<<<<<<<<<<<[-]
zero reg2
>[-]
zero reg3
>[-]
copy from b to reg1
>>>[-<<<<<+>>+>>>]
<<<[->>>+<<<]
copy from x1 to reg2
>>>>>>>>>>>>>[-<<<<<<<<<<<<<<+>+>>>>>>>>>>>>>]
<<<<<<<<<<<<<[->>>>>>>>>>>>>+<<<<<<<<<<<<<]
copy from reg2 to reg1
<[-<+>>+<]
>[-<+>]
Передвинуть reg1 в t1
zero t1
>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t1
<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Разность z и c с результатом в t2:
zero reg1
[-]
zero reg2
>[-]
copy from z to reg1
>>>>>>>>[-<<<<<<<<<+>>+>>>>>>>]
<<<<<<<[->>>>>>>+<<<<<<<]
copy from c to reg2
>>>>[-<<<<<+>+>>>>]
<<<<[->>>>+<<<<]
<[
-<->]
Передвинуть reg1 в t2
zero t2
>>>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t2
<<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Произведение t1 и t2 с результатом в x2:
zero reg4
>>>[-]
copy from t2 to reg4
>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<+<+>>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<]
>[
-zero reg2
<<[-]
copy from t1 to reg1
>>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<]
>]
Передвинуть reg1 в x2
zero x2
>>>>>>>>>>>>>[-]
copy from reg1 to x2
<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



s = s plus b minus (x minus c)
Сумма s и b с результатом в t1:
zero reg1
[-]
zero reg2
>[-]
zero reg3
>[-]
copy from s to reg1
>>>>>>>>>>[-<<<<<<<<<<<<+>>+>>>>>>>>>>]
<<<<<<<<<<[->>>>>>>>>>+<<<<<<<<<<]
copy from b to reg2
>>>[-<<<<+>+>>>]
<<<[->>>+<<<]
copy from reg2 to reg1
<[-<+>>+<]
>[-<+>]
Передвинуть reg1 в t1
zero t1
>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t1
<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Разность x2 и c с результатом в t2:
zero reg1
[-]
zero reg2
>[-]
copy from x2 to reg1
>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<[->>>>>>>>>>>>>>+<<<<<<<<<<<<<<]
copy from c to reg2
>>>>[-<<<<<+>+>>>>]
<<<<[->>>>+<<<<]
<[
-<->]
Передвинуть reg1 в t2
zero t2
>>>>>>>>>>>>>>>>>>>>[-]
copy from reg1 to t2
<<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



Разность t1 и t2 с результатом в s:
zero reg1
[-]
zero reg2
>[-]
copy from t1 to reg1
>>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<<<+>>+>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<]
copy from t2 to reg2
>>>>>>>>>>>>>>>>>>>[-<<<<<<<<<<<<<<<<<<<<+>+>>>>>>>>>>>>>>>>>>>]
<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<]
<[
-<->]
Передвинуть reg1 в s
zero s
>>>>>>>>>>>[-]
copy from reg1 to s
<<<<<<<<<<<<[->>>>>>>>>>>>+<<<<<<<<<<+<<]
>>[-<<+>>]
zero reg1
<<[-]



>>>>>>>>>>>>>>]

<]


<<<#
>#
>#
