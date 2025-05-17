import selenium
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.edge.service import Service as EdgeService
from selenium.webdriver.edge.options import Options as EdgeOptions
import requests
import re

cwd = "D:\\Develop\\algorithm\\Program-Design-2025-Spring\\hw16\\"

driver = webdriver.Edge(
    service=EdgeService(executable_path=cwd + "msedgedriver.exe")
)

target_words = """
myself
new
name
class
old
school
phone
body
head
"""

target_words = target_words.split()
target_url = "https://cn.bing.com/dict/search?q="
# print("target_words:", target_words)

filter_synoid = re.compile(r'<div id="synoid" style="display:block;">(.*?)<div class="df_div">')
filter_entry = re.compile(r'<a href=".*?" h=".*?"><span class="p1-4 b_alink">(.*?)</span></a>')
filter_img = re.compile(r'<img .*? class="rms_img" src="(.*?)">')

for word in target_words:
    print(f"{'Searching for:'+word :=^40}")
    driver.get(target_url + word)
    # print(driver.page_source)
    text = driver.page_source
    synoid_div = filter_synoid.findall(text)
    imgs = filter_img.findall(text)

    syns = []
    img = ""

    # attempt to find synnonyms
    if synoid_div:
        synoid_div = synoid_div[0]
        syns = filter_entry.findall(synoid_div)
    # attempt to find image
    if imgs:
        try:
            img = imgs[0]
            img = re.sub(r'&amp;', '&', img)
        except:
            img = ""

    print("syns:", syns)
    print("imgs:", img)

    # Save to file
    if syns:
        with open(cwd + "result.txt", "a", encoding="utf-8") as f:
            f.write(f"${word}\n")
            for syn in syns:
                f.write(f"{syn}\n")

    # Download image
    save_path = cwd + word + ".jpg"
    if img:
        try:
            response = requests.get(img)
            with open(save_path, "wb") as f:
                f.write(response.content)
            print(f"Image saved to {save_path}")
        except Exception as e:
            print(f"Failed to download image: {e}")
    

