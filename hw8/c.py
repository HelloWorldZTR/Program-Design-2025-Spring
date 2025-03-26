exit = None
class TaggedList:  #元素带标签的列表
    def __init__(self, data, tags):
        self.data:list = data
        self.tags:list = tags
    def __str__(self):
        output = ""
        for i in range(len(self.data)):
            output += f"{self.tags[i]}:{self.data[i]},"
        return output
    def __len__(self):
        return len(self.data)
    def __getitem__(self, a):
        if isinstance(a, str):
            index = self.tags.index(a)
            return self.data[index]
        else:
            return self.data[a]
    def __setitem__(self, a, v):
        if isinstance(a, str):
            index = self.tags.index(a)
            self.data[index] = v
            return self
        else:
            self.data[a] = v
            return self
    def __contains__(self, a):
        return a in self.data
a = TaggedList([70,80,90,100],["语文","数学","英语","物理"])
print(len(a),78 in a, 80 in a) #>>4 False True
print(str(a)) #>>语文:70,数学:80,英语:90,物理:100,
print(a[0],a['数学']) #>>70 80   标签也可以作为下标访问元素
a[1] = a['物理'] = 85
print(a) #>>语文:70,数学:85,英语:90,物理:85,