@[TOC](一篇搞懂 TCP 和 UDP)

# 準備工作
在真正介紹 TCP 和 UDP 之前，我們先來了解一些關於 OSI 第四層，也就是傳輸層的一些概念。

## OSI Transport Layer 傳輸層
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-35.png)

傳輸層主要負責下面幾個功能:
- 將上層數據分段
- 建立點對點連接
- 將數據段從一個進程傳輸到另一個進程 (不同主機)
- 控制數據流相關變數

前三層的介紹多半都是在探討怎麼在主機間傳輸數據報，但有時候我們是要在不同主機間進行進程的通訊。所以傳輸層主要就是為了完成這樣的功能。第四層傳輸的數據單位是 segment (數據段)，而其中最重要的兩個協議就是本文的重點，UDP 和 TCP。

## 端口 (port)
端口是個很重要的概念，但不算是本文的重點，所以不多述。這邊找到一篇 [端口的概念](https://blog.csdn.net/weixin_42724467/article/details/88533582)，寫的挺好的，若不清楚可以前往了解。

## Socket
前面提到傳輸層的功能，也就是要完成兩個進程之間的通訊。所以，當然不可能只要 ip 地址啦，我們還需要兩個進程的端口信息。這邊就要引入 socket ，它是傳輸層通訊的起點和終點的一個單位。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-36.png)

最後一點強調下，因為傳輸層通訊是一個點對點的通訊，所以第 TCP 是不支持廣播或多播的。

# TCP 協議
TCP 的全稱是 Transmission Control Protocol。首先就先來看看 TCP 的數據段結構。

## TCP 數據段
TCP 數據段 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp.png)

- 源端口，目的端口: 端口是傳輸層和應用層的接口，配合 IP 報文的 源和目標 ip 地址組成 socket，做到進程之間的通訊。
- 序號: 在 tcp 傳輸中，會為每個字節編上一個序號。而這個序號字段表示的就是該 tcp 數據段所攜帶數據的第一個字節的序號。
- 確認號: 確認號字段表示期望收到對方應答的數據段的數據部分的第一個字節的序號。
- 數據偏移: 指出 tcp 數據段的數據部分起始處離數據段起始處的距離，單位為 4 字節。
- 保留: 目前無特別用途
- URG: urgent 縮寫。當 URG = 1 時，說明該 tcp 數據段包含緊急數據，相當於提升該數據段的優先級。
- ACK: acknowledge 縮寫。當 ACK = 1 時確認號有效，否則無效。
- PSH: push 縮寫。這邊不對 PSH 多做解釋。
- RST: reset 縮寫。若收到的數據段 RST = 1，說明 tcp 連接發生了錯誤，會斷開連接重新嘗試建立連接。
- SYN: synchronize 縮寫。若 SYN = 1，說明該 tcp 數據段是一個連接請求或連接接受類型的數據段。
- FIN: finish 縮寫。當 FIN = 1，說明發送端的數據都已經發送完畢，可以釋放連接了。
- 窗口: 用來讓對方設置發送的窗口。
- 檢驗和: 用來檢驗 tcp 數據段的首部和數據部分正確性。
- 緊急指針: 指出數據段包含多少字節的緊急數據。配合著 URG 字段使用，緊急數據會被放在數據段數據部分的最前面。
- 選項: 告訴對方自己最多能接受多少個字節的數據段數據部分。
- 填充: 只是為了讓 tcp 數據段首部是 4字節的整數倍。
- ==tcp 數據段等於首部加上整個數據部分==

## 有趣的兩軍問題
兩軍問題是網絡連接一個很經典的例子，先有個了解對接下來的握手機制是有幫助的。若有興趣，可以看看 [有趣的兩軍問題](https://blog.csdn.net/weixin_46803507/article/details/106880875)。

## TCP 三次握手 (建立連接)
這邊進入到重點，來看看 TCP 協議是怎麼透過三次握手在兩個進程間建立可靠連接。

### 第一次握手
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-2.png)

首先應該先由服務端啟動，否則客戶端根本不知道要跟誰建立連接。Passive open 是說服務端啟動後不會主動跟客戶端通訊，Active open 則是說客戶端啟動後會率先開始向服務端發起請求。

### 第二次握手
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-3.png)

第二次握手因為要確認第一次握手的數據段，所以多了 ACK 字段使確認號生效。ack 表示確認號字段。

### 第三次握手
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-4.png)

至此，可靠連接建立完成。

### 數據傳輸
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-5.png)

連接建立後，客戶端就可以開始向服務端傳送數據了。

## TCP 四次握手 (斷開連接)
上面我們了解了 TCP 建立可靠連接的過程，接下來來看看 TCP 是怎麼用四次握手斷開連接的。

### 第一次握手
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-6.png)

客戶端首先自己請求結束了，於是率先發送一個 FIN = 1 的數據段，告訴服務端我不請求了，要端開連接了。

### 第二次握手
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-7.png)

服務端確認第一次握手的數據段，ACK = 1，ack = u + 1。

但是這時候還不能完全斷開連接。因為這只是客戶端跟服務端說請求結束了，但是之前的請求可能還沒處裡完，所以還得先把之前請求的數據繼續傳給客戶端才行。

### 第三次握手
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-8.png)

當服務端把之前請求的數據全部傳完給客戶端後，進行第三次握手。由服務端發起，告訴客戶端該傳的數據都已經傳完，可以斷開連接了。

### 第四次握手
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-9.png)

最後由客戶端進行第四次握手。至此，TCP 建立的可靠連接斷開。

### TCP 斷開機制總攬
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-10.png)

## TCP 有限狀態機
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-11.png)

## TCP 數據流控制
TCP 還有額外的重傳機制和數據流控制的機制。如果在傳輸過程中丟失了數據段，那麼 TCP 是會要求要重傳數據的。具體來看，數據段丟失的情況如下:

1. 數據丟失
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-12.png)

2. 確認丟失
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-13.png)

3. 確認遲到 
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tcp-14.png)

### 停止等待協議 (Stop and Wait protocol)
在這種協議下，每次傳輸數據段，都會有相應的備份。還會設定一個時鐘，若在時鐘周期內，數據段發出但一直沒有收到確認，那就會要求重傳。
>通常，時鐘週期 > (平均來回時間 * 2)

### ARQ 協議
ARQ 全稱 Automatic Repeat Request。ARQ 協議是 TCP 控制數據傳輸流量的一種做法。具體做法就是將數據劃分為固定大小(可能很多組)，並且設定滑動窗口，還有一個窗口指針指向窗口最前面，即下一個等待發送的數據單位。在窗口內的數據可以進行傳輸，而只要一個單位獲得確認，窗口指針就前進一個單位，窗口也相應的向前滑動一個單位。值得一提的是，接收數據的一方隨時可以更改窗口大小(藉由確認數據段)，體現 TCP 的流量控制。具體看看下面這個例子，應該非常清楚:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/arq.png)

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/arq-1.png)

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/arq-2.png)


# UDP 協議
UDP 的全稱是 User Datagram Protocol。UDP 比起 TCP 來說就沒那麼複雜。因為 UDP 是無連接的，所以完全沒有什麼握手機制。因此 UDP 也可以說成是不可靠的傳輸。並且，不像 TCP，如果數據段丟失，TCP 會有相應的重傳機制。但是同樣情況，UDP 會直接丟棄數據段。也因此， UDP 速度更快，也比較不會有延遲。看看 UDP 數據段結構，也會發現簡單的多:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/udp.png)

# TCP/UDP 比較
||TCP|UDP|
|:---------:|:--------:|:---------------:|
|是否有連接|有連接|無連接|
|可靠性|高|低|
|速度|慢|快|
|確認機制|有 (握手機制)|無|
|回傳機制|有|無|
|數據流傳輸|有序|無序|
|使用場景|要求可靠數據傳輸|對速率敏感，可容忍丟失部分數據|

