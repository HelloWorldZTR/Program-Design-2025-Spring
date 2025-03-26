def accfunc(f):
    f1 = f
    def wrapper(*nf):
        if len(nf) == 0:
            return f
        else:
            return accfunc(lambda x: nf[0](f1(x)))
    return wrapper

def f1(x):
    return x + 1
def f2(x):
    return x * x
def f3(x):
    return x + x
def f4(x):
    return x*3
def f5(x):
    return x-4

while True:
    s = input()
    n = int(input())
    s = s.split()
    k = accfunc
    for x in s:
        k = k(eval(x))
    print(k()(n))