import pandas as pd
import numpy as np
from datetime import datetime

root_dir = "D:\\Develop\\algorithm\\Program-Design-2025-Spring\\hw17\\"
input_file = root_dir + "finance.xlsx"

df = pd.read_excel(input_file, sheet_name='Sheet1')
df['交易额']=df['交易额'].fillna(0)
weekdays = ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday']

# print(df.head)

# print(df['姓名'])
daily_sum = {}

for index, row in df.iterrows():
    date = row['日期']
    income = row['交易额']
    
    if date not in daily_sum:
        daily_sum[date] = 0
    daily_sum[date] += income

# print(daily_sum)
daily_sum = sorted(daily_sum.items(), key=lambda x: x[1])

for day, value in daily_sum[:3]:
    date = datetime.strptime(day, '%Y-%m-%d')
    print(
        f"{date.strftime('%Y-%m-%d')} {value:.0f} {weekdays[date.weekday()]}"
    )


