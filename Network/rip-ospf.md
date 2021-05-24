@[TOC](搞懂 RIP 和 OSPF)

# 前言
再上一篇關於路由器的文章中，已經介紹過動態路由的分類，分為了 Distance Vector 和 Link State 兩種。這篇就分別介紹各自最經典的 RIP 協議和 OSPF 協議。也歡迎先去看看 [詳解 Router 路由器](https://blog.csdn.net/weixin_46803507/article/details/106972595)，對本篇理解會容易點。

# 正文

## RIP
RIP 的全稱是 Routing Information Protocol。是一個內部網關協議，是動態路由 Distance Vector 的經典協議。發展至今，有兩個版本，RIP1 和 RIP2。

### RIP1
首先就來看看 RIP1 的一些特色 :

- 以 30sec 為單位和鄰居交換路由表
- 以 15跳作為最大的跳數
- 有附載均衡的功能

不過，既然有 RIP2 就表示 RIP1 是存在一些缺陷的。那麼來了解下 RIP1 的缺陷 :

- RIP1 報文不能攜帶子網掩碼，也就是說，不支持 VLSM
- 使用 255.255.255.255 作為廣播地址交換路由表

### 配置 RIP1
在一個路由器上啟動 RIP1 的配置如下: 
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R1.png)

來看看一個例子:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R2.png)

以 A 為例，我們想要在 A 上啟動 RIP 協議，並讓 A 跟 B,D 交換路由表信息。配置過程如圖所示，首先指名啟動了 RIP 進程，接著公告網段。注意到，這邊公告的網段是 1.0.0.0，不是 1.1.0.0，是 2.0.0.0，不是 2.2.0.0。為甚麼呢 ? 因為在配置 RIP協議時，只要公告要交換的網段就好了，公告 1.0.0.0 就可以和 S0 和 Token Ring 交換路由表了。

### RIP2
前面也有提到 Distance Vector 可能會造成路由迴路的問題，RIP2 就對此做了改善，整提比 RIP1 更加完備。首先來看看 RIP2 的一些特色 :

- RIP2 一樣是利用跳數作為計算
- 為避免路由迴路，採取了 3 種方式 :
> 利用計時器方法，默認時鐘設為 180sec
> 利用水平分割方法
> 將最大跳數設為 15 跳 (16 跳 = 不可達)

### 配置 RIP2
這邊直接來看一個啟動 RIP2 的例子 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R3.png)

對於 A 來說，公告了 172.16.0.和 10.0.0.0 網段，因此能跟左右鄰居都交換信息。C 也是。而看看 B，因為左右兩邊的網段都屬於 10.0.0.0，所以只要公告 10.0.0.0 就可以和  A,C 都做到交換信息了。

==注意到，必須顯示的指明要啟動 RIP2 (version 2)，不然會是 RIP1==

我們也可以檢查看看是否成功的啟動了 RIP2，並順利完成路由表交換 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R4.png)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R5.png)

我們以 A 為例，可以看到，跟 RIP1 一樣，RIP2 也是每 30sec 做一次交換，圖中顯示距離下一次交換還要 12sec。再往下看，可以看到 A 也顯示是跟 10.0.0.0 和 172.16.0.0 交換信息。

最後我們可以 `show ip route`，來看看 A 實際學習的情況。R 開頭的表示是經由 RIP 協議學習到的。可以看到，A 學習到說經由 10.1.1.2 這個地址，可以 1 跳到達 10.2.2.0/24 網段，而經由 10.1.1.2 可以 2 跳到達 192.168.1.0/24 網段。

## OSPF
OSPF 的全稱是 Open Shortest Path First。是目前最為廣泛應用的 Link State 動態路由協議。OSPF 不再基於跳數判別最佳路徑，而是通過交換路由器各自的拓數據集，以各自為根，建立自己的路徑樹，以此來判別最佳路徑。可以先來看看一個簡單的例子 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R6.png)

可以看到，RIP 基於跳樹會選擇下面的路徑。但是 OSPF 會基於帶寬，而選擇上面的路徑。其實上面路徑也確實較快。

接下來必須要來了解一下 OSPF 的一些用語和概念。

### OSPF terminology

#### Link
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R8.png)

#### Link State
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R9.png)

#### Cost
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R10.png)
如圖，一般的串口線帶寬會比小，而以太網線帶寬會比較大。cost 和帶寬呈現反比的關係。

#### Area
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R11.png)
在 OSPF 中，可以將大的拓樸劃分為一些區域。而每一個區域內的路由器都會有相同的鏈路狀態。而多個區域就構成一個自治系統。

#### Neighborship database
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R12.png)

#### Topology database
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R13.png)

#### Routing table
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R14.png)

最後生成的路由表就可以判別最佳路徑了。

#### DR / BDR
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R15.png)

### OSPF Area 概念
在 OSPF 中，首先我們必須了解到，OSPF 是有層次的，且是 2 層的。在多 area 的 OSPF 拓樸中，所有 area 都和 area0 相連。來看看示意圖 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R16.png)

### 單域 OSPF 操作
接下來我們只聚焦在單域的 OSPF 拓樸，也就是只有一個 area 的。

### SPF 算法
在 OSPF 中，使用了 SPF (Shortest Path First) 算法來生整個拓樸的路徑樹。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R18.png)

生成出來的樹顯然就不可能會有迴路，這就避免了 RIP 中路由迴路的問題。

圖中以 B 為根節點生成了 B 的 SPF tree，可以驗證確實到達其他路由器的路徑選擇都是最佳的 (代價最小)。

### 選取 DR / BDR
考慮下面的拓樸，在 OSPF 下，如果沒有選取 DR / BDR，那麼兩兩互相交換信息，需要 C5/2 = 10 條連接。而選取了 DR / BDR 後，目的就在於所有設備不在直接和其他設備交換信息，而是都只先和 DR 交換，再由 DR 發送給其他路由器。這樣可以只要 4 條連接，提升了 OSPF 的性能。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R19.png)

==不過需要記得，選取 DR / BDR 只有在多路復用的情形下才要進行==

那麼，來看看選取 DR / BDR 的規則是什麼吧!
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R25.png)
==路由器優先級是可以自行配置的。==


### OSPF 報文
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R20.png)

- Hello : 發送 Hello 報文相當於維持自己的心跳，讓別的路由器知道我還活著，會利用專用 ip 地址 224.0.0.5 把 Hello 發送給所有啟動了 OSPF 的路由器
==這邊注意下，Hello 報文的 TTL 置為 1，也就是說，只會發給相鄰的路由器==
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R24.png)

- DBD : 包含該路由器的鏈路狀態

- LSR : 若一個路由器收到另一個路由器的 DBD 後，發現了一個自己 DBD 沒有的鏈路狀態，就會發一個 LSR 給對方

- LSU : 當一個路由器收到 LSR 後，就會將對方缺少的鏈路狀態通過 LSU 發回去

- LSA : 作為建立可靠性用途

### OSPF 工作機制
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R21.png)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R22.png)
這邊 A 和 B 是一個只從關係，來決定誰先開始發送鏈路狀態信息。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R23.png)

> 總結 OSPF 主要有五個步驟 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R17.png)
==第二部選取 DR/ BDR 只有在多路復用的鏈路才需要。==

當路由表都建立好後，不像 RIP 會定時繼續交換信息，OSPF 就穩定下來了。不過，之前提到過 OSPF 是事件驅動的，也就是說，如果今天拓樸發生了變化，就必須進行上圖的 Step5。

來看看這個例子 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R26.png)

假設今天連接 A 的網絡斷掉了，那麼 A 會通過專用地址 224.0.0.6 作為目的地發一個 LSU 給 DR，再由 DR 藉由專用地址 224.0.0.5 發一個 LSU 給所有開啟了 OSPF 的路由器。而其他路由器也必須再去更新自己 area 的路由器的信息。接著重複所有的步驟，計算 SPF tree，生成路由表。

### 配置 OSPF
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R27.png)
首先要先啟動 OSPF，然後指名端口屬於的 area。注意到，這邊指名端口的 ip 地址時用的不是子網掩碼，而是一個叫做 wildcard mask 的東西。先介紹下，wildcard mask 其實就是 subnet mask 取反。來看看一個啟動 OSPF 的例子 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/R28.png)



