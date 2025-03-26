n, m = map(int, input().split())
dishes = {}
for i in range(m):
    name, price, count = input().split()
    price = int(price)
    count = int(count)
    dishes[name] = {
        "price": price,
        "count": count
    }
income = 0
for i in range(n):
    names = input().split()
    for name in names:
        if dishes[name]["count"] > 0:
            income += dishes[name]["price"]
            dishes[name]["count"] -= 1

print(income)