@[TOC](詳解路由器)

# 前言
路由器之前也稍微介紹過，知道一些基本功能，也知道路由器是工作於網絡層的設備。這篇文章我們就更進一步，來深入了解路由器的知識。若想先了解網絡層的知識，歡迎先去看看 [OSI Network Layer 網絡層](https://blog.csdn.net/weixin_46803507/article/details/106865151)，也可以去看看 [搞清楚網絡中的設備](https://blog.csdn.net/weixin_46803507/article/details/106759417) 先對一些設備有初步概念。

# 正文

## 路由器構造
路由器詳細的內部構造如下圖 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r1.png)

- RAM : 存放一些路由器配置的相關信息。包話路由表，ARP cach路由器配置的相關信息。包話路由表，ARP cache 等等。

- NVRAM : Non Volatile RAM，備份的 RAM。

- Flash : 存放 Cisco OS 系統。

- ROM : 存放 POST (Power On Self Test)，以及 boostrap 程序。主要是在路由器要被啟動時做為一個驗證作用。

- Interfaces : 路由器上有多個端口

## 路由器基本指令
當路由器被啟動時，首先是普通模式，是無法進行一些配置的，必須進入特權模式。
|指令|作用|
|------------|------------|
|enable|進入特權模式|
|config terminal|進入全局配置模式|
|Router(config)#hostname [name]|為路由器命名|
|Router(config)#enable secret [password]|為路由器設置密碼|
|Router(config)#banner motd #[message]#|設定登陸提示文字|
|Router(config)#interface [type][number]|進入某類型某端口配置模式|
|Router(config-if)#ip address [address][subnet mask]|配置端口 ip 地址和子網掩碼|
|Router(config-if)#description[description]|設定端口激活消息|
|Router(config-if)#no shutdown|將端口激活|
|Router#copy running-config startup-config|保存路由器更改|
|Router#show startup-config|顯示NVRAM中配置文件|
|Router#show ip route|顯示路由表|
|Router#show interfaces|顯示路由器端口完整信息|
|Router#show ip interface brief|顯示簡單路由器端口信息|

## 靜態路由
如果說一條路徑較為固定，也變化不大時，使用靜態路由會是更好的一個選擇。

### 配置靜態路由
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r2.png)
可以去看看 [静态路由命令配置](https://www.cnblogs.com/pwc1996/p/5425236.html)，有一個清楚的例子。

## 動態路由
前面的文章中也稍微介紹過了動態路由。動態路由的時限就是要靠一些 routing protocol 來完成，來在路由器之間分享信息。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r3.png)

所以最重要的，當然是來探討這些 routing protocol。以下介紹的 routing protocol 都是 IGP(Interior Gateway Protocol) 的協議。這邊先介紹兩大類 routing protocol。

### Distance Vector (距離史量)
DV 算法使用了 Bellman-Ford 算法，在路由器之間傳遞路由表信息，從而達到路由器之間知道彼此信息的目的。但是 DV 不允許任一路由器學習到整體路由的拓樸，而是只能學習到鄰居路由器的信息。

### DV 的缺陷
DV 的缺陷如下圖，就是有可能產生路由迴路，導致網絡性能大幅降低。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r4.png)

為什麼上圖會造成路由迴路呢 ? 首先，假設今天網段 1 和 E 之間的線路斷掉了，那基於 DV，E 就會告訴 A 說，我到不了網段 1。但是這時，B 或許會基於之前學習到的信息，告訴 A 說，我到的了網段 1，可是其實這個路徑，本身就是通過 A 四跳到達的。於是 A 此時就以為 B 真的有一條路徑可以到達網段 1，導致數據包不斷在 A,B,C,D 之間傳輸，造成路由迴路。

那麼來看看對於這樣的缺陷，有哪些解決辦法!

#### 設定最大跳數
設定最大跳數的做法就是在每個路由器上設定一個最大跳數，如果收到一個數據報跳數等於這個最大跳數 (通常設為 16)，就丟棄該報文，視為不可達。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r5.png)

#### 路由毒化
路由毒化的做法以這個拓樸來看。當 E 到達網段 5 的路徑斷掉後，E 並不會刪除到達網段 5 的這個條目，而是將之置為 16 跳到達 (不可達)。然後基於 DV 算法，就會將這個路由表依序傳遞下去，最終，A,B,C,D 都會知道到達網段 5 的代價是 16 跳，也就是不可達。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r6.png)

#### 水平分割 
水平分割的思路就是說，從一個端口得到的信息，不能再由相同端口發送回去，以此來避免迴路的形成。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r7.png)

#### 計時器
計時器的做法是說，在網絡不可達時，不會立即刪除條目，而會先起一個計時器。在這個計時器的指定時間內，其餘路由器仍然持續交換信息，但是，只有當所收到的路由優化，才會接收下信息，否則就不變。等到計時器到點，才把條目刪除。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r8.png)

### Link State (鏈路狀態)
Link State 的概念會比 DV 複雜得多，因為是要基於全局拓樸視野去決定最佳路徑等等，也因此，不會產生路由迴路的情形，也更高級些。

Link State 主要是通過每一個路由器有一個拓樸數據庫，並各自以自己為根，使用 SPF(Shortest Path First)算法生成各自的路徑樹。

之後會再介紹一些典型的 Link State Protocol。

### DV / LS 比較
||Distance Vector|Link State|
|:-----:|:----------:|:-----------:|
|拓樸視角|鄰居視角|全局視角|
|計算單位|跳數|最短路徑|
|觸發機制|定時定期更新|事件觸發(拓樸改變)|
|收斂性|慢|快|
|交換信息|路由表|鏈路狀態|
|效率|低|高|

==上表所指的收斂性指的是路由器從開始學習信息到穩定下來的過程的一個性質==

### 配置動態路由
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r9.png)

## 默認路由
默認路由的概念就是說，對於一個路由器，如果來了一個報文，其目的地址和路由表中的條目都沒有配對時，就會請默認路由代為轉發。

### 配置默認路由
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/r10.png)

