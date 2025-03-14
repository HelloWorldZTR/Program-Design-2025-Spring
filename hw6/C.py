s = input()
s = s.split()
x = int(s[0])
y = int(s[1])

if x>=-1 and x<=1 and y>=-1 and y<=1:
    print("yes")
else:
    print("no")