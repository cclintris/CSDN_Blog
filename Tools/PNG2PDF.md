@[TOC](將多張圖片合成一份 pdf)

# 前言
會有這樣的需求其實是因為本小白的 npy 無法下載某堂課的課件(也不知道老師幹嘛這樣刁難學生)，總之，本小白的一個好朋友(是個大老)寫了段 js 程序試圖下載課件，結果能是能，不過下載下來並不是一份完整的 pdf，而是很多張單張圖片。因此就有了將多張圖片合成一份 pdf，本篇就是紀錄下解決的方法。

# 正文

## 環境和工具
- python3
- IDE pycharm

## 代碼實現
主要功能使用到了 python PIL 和 fpdf 庫。
```py
from fpdf import FPDF
from PIL import Image
import os

def PNG2PDF(pdfFileName, listPages):
	cover = Image.open(listPages[0])
	width, height = cover.size

	pdf = FPDF(unit = "pt", format = [width, height])

	for page in listPages:
		pdf.add_page()
		pdf.image(page, 0, 0)

	pdf.output(pdfFileName, "F")

if __name__ == '__main__':
    PNG2PDF("result.pdf", [imgFileName for imgFileName in os.listdir('.') \
                           if imgFileName.endswith("png")])
```
main 函數中用到了 `os.listdir('.')` 是列當前目錄下的所有文件。對於 python os 庫還是挺好用的，建議可以去熟悉下 [Python OS 文件/目录方法](https://www.runoob.com/python/os-file-methods.html)。

所以只要將所有 png 放在與這段代碼同級的目錄下，就能按序生成一份完整的 pdf!

# 結語
雖然根本稱不上甚麼多了不起的東西，但能解決生活中的實際需求還是小有成就感的，好像其他第三方平台也能完成，只不過要花錢就是了(貌似省了 74 塊)。感謝讀到這篇的朋友們，相信一定有用的。
