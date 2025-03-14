
def decode(s):
    if len(s) <= 1:
        return s
    pivot = s[0]
    left = s[1:1+(len(s)-1)//2]
    right = s[1+(len(s)-1)//2:]
    # print(f"left_encoded: {left}")
    # print(f"right_encoded: {right}")
    return decode(left) + pivot+ decode(right) 

s = str(input())
print(decode(s))