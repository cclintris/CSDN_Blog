@[TOC](C++ 理解與實現哈希表)

# 哈希表概念
哈希技術是在記錄的存儲位置和它的關鍵字之間的建立一個確定的映射關係 f，也就是說，為每個關鍵字 key 對應一個存儲位置 f(key)。查找時，根據這個對應的關係找到給定值 key 的映射 f(key)，若查找集合中存在這個紀錄，則必定在 f(key) 位置上。我們就把這種映射關係 f 稱為哈希函數(Hash function)。採用哈希技術將數據紀錄存儲在一塊連續的存儲空間上，這塊連續空間也就稱為哈希表。

# 哈希表構造方法
這邊介紹幾種常見的哈希表構造方法，當然不止這幾種，且也有很多優化方式，但是作為了解可以從以下幾種開始。

## 直接定址法
直接定址法使用下面的公式:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/hash-1.png)
舉例來說統計出生年分，就可以用 f(key) = key - 1990 來計算哈希地址:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/hash-2.png)

## 除留取余法
這種方法是最常用的哈希函數構造方法，對於表長為 m 的散列公式如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/hash-4.png)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/hash-5.png)

## 數字分析法
分析數字型關鍵字在各位上的變化情況，比如今天的關鍵字是電話號碼，一般來說是 11 位的字符串，後 4 位是真正的用戶號。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/hash-6.png)

## 摺疊法
把關鍵字分割成幾個相同(m)位數的部分，做累加，然後再對結果取 m 位做為哈希值。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/hash-7.png)

## 平方取中法
對關鍵字做平方，並取中間幾位作為哈希值。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/hash-8.png)

# 衝突解決

