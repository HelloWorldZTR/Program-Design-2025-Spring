s = input().split()
a = int(s[0])
b = int(s[1])
op = s[2]

if op == '+':
    print(a+b)
elif op == '-':
    print(a-b)
elif op == '*':
    print(a*b)
elif op == '/':
    if b == 0:
        print("Divided by zero!")
    else:
        print(a//b)
else:
    print("Invalid operator!")