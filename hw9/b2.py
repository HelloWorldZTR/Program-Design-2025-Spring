def accfunc(f):
    pass # Stupid workaround to make the code compilable
def accfunc2(f=lambda x: x):
    def wrapper(nf = None):
        if nf is None:
            return f
        else:
            return accfunc(lambda x: nf(f(x)))
    return wrapper
accfunc = accfunc2

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