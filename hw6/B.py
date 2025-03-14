a = input()
a = a.split()
a = [int(word[:-1]) for word in a]

print(sum(a))