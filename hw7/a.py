n, m  = map(int, input().split())
data = []
for i in range(n):
    data.append(list(map(int, input().split())))

for i in range(m-1, -1, -1):
    for j in range(n):
        print(data[j][m-i-1], end=" ")
    print()
