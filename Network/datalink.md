@[TOC](OSI Datalink Layer 數據鏈路層)

# OSI 數據鏈路層
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-1.png)

# DataLink layer 跟 Physical layer 區別
數據鏈路層和物理層最大的區別在於，數據鏈路層是可以基於 mac 地址做一些驗證等等，而物理層只是單純的傳輸 01 bits。

1. layer1 無法與上層通信，layer2靠LLC(Logical Link Control)可以
2. layer1 無法決定哪個主機接收或發送信息，layer2靠MAC(Media Access Control)可以
3. layer1 無法識別計算機，layer2可以
4.  layer1 就是0-1串的形式，layer2 靠的是frame(幀)來組織數據

# mac 介質訪問原則
mac 介質訪問原則主要分為兩種:
1. deterministic : 有著 token 機制。網絡中拿到 token 的設備可以選擇發消息，或將 token 繼續往下傳遞。
2. non-deterministic : First come first served 的原則。不能保證線路同時只有一個數據報，所以使用 CSMA/CD 的衝突檢測機制。以太網就是採取這樣的機制。

# Datalink 分層
IEEE 將數據鏈路層分成了兩層，分別是 LLC 和 MAC。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-2.png)

LLC 負責邏輯的識別來自網絡層的不同協議，並進行封裝等等的工作。

MAC 負責向下跟物理層介質溝通。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-5.png)

來自網絡層的數據報首先由 LLC 子層在前後加上兩個 mac 地址，分別是 DSAP(Destination Service Access Point) 和 SSAP(Source Service Access Point)。接著才會再到 mac 子層做更底層的傳輸。



# 幀結構 (frame structure 802.3)
這邊所介紹的幀結構是傳統的有線以太局域網所使用的數據幀。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-3.png)

1. preamble: 告訴接收站有frame要來了，以 10101011 開頭
==我們在講一個frame有多少Bytes的時候，不會把preamble算進去==
2. dest addr: 可以是單播，多播，廣播地址
3. source addr: 總是一個單播地址
4. length: 標明 data 有多少個 Byte
5. data: 實際數據
> 1. 規定一幀至少要 64B，所以 data 至少要 64-18=46(B)
> 2. 規定一幀 data 最多 1500B，也就是說一幀最多是 1518B
6. FCS: 包和CRC校驗碼。接收方會依據前面的信息重新計算CRC並與FCS比較，以確認幀在傳輸過程中是否損耗

# mac 地址
幀結構的 mac 地址如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-4.png)

MAC地址最高字节的低第一位，表示这个MAC地址是单播还是多播。0表示单播，1 表示多播。

當 frame 中的 dest addr 全為 1 時，就表示這是一個廣播數據報。FFFF FFFF FFFF (h)。而因為廣播數據報會極大的降低網絡性能，所以應該只用於確實要發給網絡中所有設備，或者目標的 mac 地址未知。

# CSMA/CD
上面提到了傳統以太網所使用的衝突檢測機制，CSMA/CD，那麼就來了解下 CSMA/CD 的機制吧!

CSMA/CD 的全稱是 Carrier Sense Multiple Access with Collision Detection。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-6.png)

來看看百度百科對於 CSMA/CD 的解釋，其實還滿清楚的:
> CSMA协议要求站点在发送数据之前先监听信道。如果信道空闲，站点就可以发送数据；如果信道忙，则站点不能发送数据。但是，如果两个站点都检测到信道是空闲的，并且同时开始传送数据，那么这几乎会立即导致冲突。另外，站点在监听信道时，听到信道是空闲的，但这并不意味着信道真的空闲，因为其他站点的数据此时可能正在信道上传送，但由于传播时延，信号还没有到达正在监听的站点，从而引起对信道状态的错误判断。在早期的CSMA传输方式中，由于信道传播时延的存在，即使通信双方的站点，都没有侦听到载波信号，在发送数据时仍可能会发生冲突。因为它们可能会在检测到介质空闲时，同时发送数据，致使冲突发生。尽管CSMA可以发现冲突，但它并没有先知的冲突检测和阻止功能，致使冲突发生频繁。
可以对CSMA协议作进一步的改进，使发送站点在传输过程中仍继续侦听介质，以检测是否存在冲突。如果两个站点都在某一时间检测到信道是空闲的，并且同时开始传送数据，则它们几乎立刻就会检测到有冲突发生。如果发生冲突，信道上可以检测到超过发送站点本身发送的载波信号幅度的电磁波，由此判断出冲突的存在。一旦检测到冲突，发送站点就立即停止发送，并向总线上发一串阻塞信号，用以通知总线上通信的对方站点，快速地终止被破坏的帧，可以节省时间和带宽要求站点在发送数据过程中进行冲突检测，而一旦检测到冲突立即停止发送数据。这样的协议被称为带冲突检测的载波监听多路访问协议，即 CSMA/CD（Carrier Sense Multiple Access with Collision Detection）协议。

Carrier Sense 就是說，網絡上所有設備在發送數據報前都要先監聽總線上有無正在傳輸的信息。若有(總線忙)，則不發數據，若無(總線空閒)，則立即進入準備發送狀態。

Multiple Access 就是說，網絡中的所有設備收發數據報都通過一條總線。

Collision Detection 就是說，設備在發出報文同時，還要監聽介質上有無衝突。


# WLAN
WLAN 的全稱是 Wireless Local Area Network。

## WLAN 是甚麼?
Wireless 顧名思義，就是說沒有實體媒體，是通過無線通信技術所構成的局域網。無線局域網主要分為兩類。一類是有基礎設施的 infrastructure wlan，而另一種是沒有基礎設施的 ad-hoc wlan。所謂的基礎設施就是，先建立好的一個基站，而所有 wlan 的設備都跟這個基站進行通信。先來看看一般 wlan 的拓樸結構:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-7.png)

上圖是 infrastructure mode 的 wlan 示意圖。先看粉色圈圈的範圍。可以看到，有一個基站，而另外四台設備都和這個基站相連，構成星型拓樸。而這一個基站可以說是該 wlan 的接入點(AP)，wlan 內的設備其實不會直連，而是都是通過這個 AP 進行通信。而這個 AP 以及 wlan 的所有設備則構成了一個 BSS(Base Service Set)。而粉色 wlan 和黃色 wlan 則共同構成了一個 ESS(Extended Service Set)。最後，這些 wlan 都還連到了一條總線，在這邊稱為 DS(Distribution System)，並通過網絡層設備才得以上網。 這邊再提一個概念，當一個 AP 被建立，就會被分派一個 SSID(Service Set Identifier)。

## Active Scanning vs Passive Scanning
那麼 wlan 內是怎麼建立各設備跟 AP 的連通關係呢?這邊介紹兩種方式。第一種是 Active Scanning，第二種 Passive scanning。

Active Scanning 就是說，由想要加入 AP 的設備，自己主動的發送一個探測消息，而這個探測消息將會包含該設備希望能夠加入的 AP 的 SSID。當找到目標 AP，目標 AP 就會返回一個應答，至此，連通關係就建立了。

Passive Scanning 的作法是，由 AP 在一定機制下，在其所覆蓋的 wlan 範圍內發送自己的 SSID。當設備接收到這個消息，且也確實想要加入該 AP，就可以直接建立連接。很明顯，Active Scanning 比起 Passive Scanning 來說，相對更安全點。

## WLAN 幀結構
因為 wlan 相對於有線的 lan 以外，較為不穩定，所以 wlan 所使用的幀結構跟上述介紹的完全不一樣。那麼，就來看看 wlan 的幀結構:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-8.png)

這邊特別提 wlan frame 的四個地址字段。地址 1,2,3 用於 infrastructure mode 的 wlan，而地址 4 用於 ad-hoc mode 的 wlan。來看看下面這個例子:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-9.png)

如上圖，BSS1 內的主機A和主機B之間的通訊要透過AP1才行。

## CSMA/CA
在 wlan 中，各站點只能夠偵測到附近的信號，因此 CSMA/CD 不合事。所以在 wlan 中，應用的機制是 CSMA/CA (Carrier Sense Multiple Access with Collision Avoidance)。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/dl-10.png)


