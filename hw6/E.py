import math

n = int(input())

min_perimeter = float('inf')
for length in range(1, int(math.sqrt(n)) + 1):
    if n % length == 0:
        width = n // length
        perimeter = 2 * (length + width)
        if perimeter < min_perimeter:
            min_perimeter = perimeter

print(min_perimeter)

    