@[TOC](網絡基本知識儲備)

# 前言
這學期上了一門互聯網計算，主要講述一些網絡方面的知識。無奈因為疫情關係，這學期都是遠距教學，加上本人實在懶惰沒效率，導致現在啥都不會。開始以寫博客的方式，回憶這學期的課程。

# 正文
首先，先有個基本概念。所謂的 network，有可能是好多台 pc，也有可能只有 2 台(1 台不行廢話)。那麼，這些 pc 總要有互連的方式，總要有個溝通的媒介吧 ! 這些媒介基本就分為 2 種:
1. shared media(multiple access)
2. point to point

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/phy-1.png)

## LAN vs WAN
LAN(Local Area Network)顧名思義，就是比較小範圍的網絡，而 WAN(Wide Area Network)，就是比較大範圍的。LAN 小至家庭網絡，辦公室等等，而 WAN 規模則可以大至跨越洲跟洲之間。

## 較為著名的網絡協議組織
1. IEEE (Institute of Electrical and Electronic Engineers)
2. UL (Underwriters Lab)
3. EIA (Electronis Industries Alliance)
4. TIA (Telecommunications Industry Association)
5. ANSI (American National Standards Institute)

## Ethernet(以太網)
以太網是一種局域網技術，使用同軸電纜作為網絡媒體，且使用 CSMA/CD 衝突檢測機制。以太網只是一個概稱，泛指那些使用 CSMA/CD 衝突檢測機制的局域網。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/phy-2.png)

## 衝突域 vs 廣播域
衝突域 : 衝突域的概念就是說，最小就會產生衝突的範圍。

廣播域 : 廣播域的概念就是說，當一台設備發送了一個報文，所有也都能收到該報文的設備全部都屬於同一個廣播域。

這部分還是要去看看各種設備對於衝突域和廣播域的劃分，會更好的理解這兩個概念。若有興趣可以看看 [搞清楚網絡中的設備](https://blog.csdn.net/weixin_46803507/article/details/106759417)。

