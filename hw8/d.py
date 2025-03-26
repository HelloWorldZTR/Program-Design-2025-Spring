def cons(x, y):
    s = a = b = None  # 防止作弊用
    a = x
    b = y

    def _(n):
        if n == 0:
            return a
        elif n == 1:
            return b
        else:
            return "error"

    return _


s = input().split()
a, b = s[0], s[1]
pair = cons(a, b)
print(pair(int(input())))
