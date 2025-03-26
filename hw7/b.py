class Poker:
    conversion_color = {
        "c":0, 
        "d":1,
        "s":2, 
        "h":3
    }
    conversion_number = {
        "2": 12,
        "A": 11,
        "K": 10,
        "Q": 9,
        "J": 8,
        "10": 7,
        "9": 6,
        "8": 5,
        "7": 4,
        "6": 3,
        "5": 2,
        "4": 1,
        "3": 0,
    }
    conversion_color_reverse = {v: k for k, v in conversion_color.items()}
    conversion_number_reverse = {v: k for k, v in conversion_number.items()}

    def __init__(self, value:str):
        self.color = Poker.conversion_color[value[:1]]
        self.number = Poker.conversion_number[value[1:]]
    
    def __lt__(self, other):
        if self.number == other.number:
            return self.color < other.color
        else:
            return self.number < other.number
    def __str__(self):
        return Poker.conversion_color_reverse[self.color] + Poker.conversion_number_reverse[self.number]

while(True):
    try:
        data = list(map(Poker, input().split()))
        data.sort(reverse=True)
        for i in data:
            print(i, end=" ")
        print()
    except:
        break
