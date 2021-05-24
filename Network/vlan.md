@[TOC](搞懂 VLAN 是甚麼)

在進入本文的主題 VLAN 之前，我們先要來介紹一個 STP(Spanning Tree Protocol) 協議，對理解 VLAN 是不可或缺的一個知識點。

# 為什麼要有 STP
在之前的文章中介紹過，路由器之間的報文轉發是有可能會發生路由迴路的，若不清楚，歡迎先去看看 [詳解 Router 路由器](https://blog.csdn.net/weixin_46803507/article/details/106972595)。那麼，交換機之間的幀轉發也是有可能會形成迴路的。看看下面這個例子 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-1.png)
若正常情況下，CAT1 和 CAT2 都會知道和 A,B 各自轉發的接口。但是假設今天 B 崩潰了或是說拿掉了，那當 A 要發一個信息給 B 時，就會在 CAT1 和 CAT2 之間不停轉發，造成泛洪迴路。

# STP (Spanning Tree Protocol) 

## STP 概念
接下來直接來看看 STP 的主要目的 :
1. 在網絡中允許冗余路徑，但是可以避免迴路的產生
2. 通過計算出穩定的生成樹還避免迴路發生
3. 在 STP 中，有一個叫做 BPDUs(Bridge Protocol Data Units) 的幀，用於建立生成樹


## STP 機制
STP 的基本步驟如下 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-2.png)

## BPDU 報文
BPDU 報文的全稱是 Bridge Protocol Data Unit。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-4.png)
具體步驟看看下面這張圖 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-3.png)
幾個 BPDU 比較重要的信息如上圖。首先所有交換機會先選舉出一個 root bridge，然後各自以 root bridge 為根，形成一個生成樹。至此，每個交換機之間都只會有唯一的連接，而冗余的路徑都會被禁用掉。

## BID 怎麼來的 ?
每一個交換機都會有自己的 BID (Bridge ID)，也都有自己的 mac 地址。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-5.png)
上面提到 STP 要先選舉一個 root bridge，也就是選舉出 BID 最小的交換機。如果所有交換機 BID 的首 2 字節都使用的是默認的 32768，那麼 mac 地址就作為仲裁者。越低的 mac 地址就成為 root bridge。

## STP 工作釋例
上面說的確時抽象了點，本人自己一開始也難以理解，所以來看看這個 STP 具體工作的情境吧!

### Step1 : 選舉根交換機
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-6.png)
一開始當然大家都會將自己視為根交換機，因為誰也看不到誰。接下來就會開始交換 BPDU，對於 cat-B, cat-C 來說，當收到 cat-A 的 BPDU 後，就會知道 cat-A 的 BID 比自己小，於是將根交換機設為 cat-A，並且開始要計算各自到 cat-A 的 cost，也要記錄出發的端口。

### Step2 : 計算 cost
接著就要開始計算所有非根的交換機到根交換機最短的 cost，並且為每個非根交換機確定各自的 root port。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-7.png)
以 cat-B 來說，從端口 1/1 和 1/2 出去都可以到達 cat-A，但是從端口 1/1 出去的 cost 顯然小於 1/2 的路徑。因此，cat-B 就會將自己的 root port 設為 1/1。cat-C 同理。

### Step3 : 選舉指定端口
Step2 選舉出了 root port 後，接下來還要為每個鏈路，或者說區域 (segment) 選舉出一個指定端口，作為實際幀的轉發。而其餘既不是 root port，也不是指定端口的端口就會被禁用掉。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-8.png)

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-9.png)

以 segment3 來說，cat-B 的 1/2 就被選舉為指定端口，所以 cat-C 的 1/2 就被禁用了。

# VLAN

## VLAN 基本概念
VLAN 的意思是 Virtual LAN (Local Area Network)。我們藉由下面的圖來了解 VLAN 的基本概念 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-10.png)

左邊傳統的 LAN 就是由一些主機連到同個 hub 上構成一個 LAN，再透過 router 連接其他網段，基於物理地域劃分。右邊則是 VLAN 的做法。看看上面三台主機，雖然都連接到一個交換機上，但是他基於 VLAN 的劃分邏輯的構成了其他的 LAN，而因為不同 VLAN 之間是不能通訊的，因此可以達到一些網絡安全的考量。

簡單來說，VLAN 的概念就是基於交換機的端口設定 VLAN id，而相同 VLAN id 的主機才屬於同一個廣播域，才可以通訊，而不再受限於物理連接到的設備。

## VLAN 架構
上面說道 VLAN 之間應該是不能通信的，但如果真的要通信還是可以的，這時得靠路由器代為轉發，或者使用三層交換機也可以，看看下面的示意圖 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-11.png)

注意到上圖，譬如 PC2 和 S2 間的線路只能傳輸 VLAN20 的信息，但是像 S2 上的 F0/1 和 S1 的 F0/1 則需要同時可以傳輸 VLAN10, VLAN20 的信息。這樣的線路再 VLAN 中稱為 backbone。

### VLAN frame
那麼我們當設定好交換機端口對應的 VLAN 後，我們就要來看看幀在 VLAN 中是怎麼被辨別傳輸的了。

#### frame filtering
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-12.png)
這個方法就是說，每個交換機會有自己的一張 VLAN 表，對應各自端口的地址所屬的 VLAN id。

#### frame tagging
這個做法則是說，對於要被發送到 backbone 上的幀，會先加上一個 VLAN 標示符，然後會在被交換機識別後，再把這個標示符去掉，發送給相應的 VLAN id 的端口。

## VLAN 具體實施

### 靜態 VLAN
靜態 VLAN的做法就是說由用戶或是網管自己手動配置交換機端口上屬於的 VLAN，很明顯這樣做比較方便，更容易配置，也更好維護。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-13.png)

### 動態 VLAN
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-14.png)

注意到上圖，在動態 VLAN 的做法中會配置一個 VLAN 服務器，包含配置所屬 VLAN 的一些機制。動態 VLAN 的做法則配置上較為複雜，但是可以適應更多設備加入的情況。

### 面向端口的 VLAN
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-15.png)

面向端口的 VLAN 指的是路由器的端口，因為之前也提到不同 VLAN 間要通信只能通過路由器。可以看到，真正 VLAN 的劃分其實就是從第二層開始，然後針對不同 VLAN 連接到不同路由器的端口，以此來達到一些安全上的考量。

### AL / TL
在 VLAN 中的連接分為二種，AL 和 TL。AL 是 Access Link，而 TL 是 Trunk Link。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-16.png)
如上圖我們可以知道二者的區別；
> AL 就是指只能允許一個 VLAN 的信息傳輸的線路。

> TL 則是指那些可以允許多個 VLAN 信息在上面傳輸的線路。而通常，TL 所需要的帶寬當然就大些，作為 backbone 連接到路由器或是三層交換機上。好處就是可以節省端口。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-17.png)
我們還可以為一個 TL 配置一個 native link，是在如果 TL 崩潰掉時可以使用的傳輸線路。

### VLAN 操作

#### 基本配置
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-18.png)
Step1 就是在交換機的 VLAN database 中新增一個 VLAN id。而 Step2 開始就是具體配置端口所屬的 VLAN。還有可以使用 switchport mode access 或 switchport mode trunk 來指定要配的端口的線路類型。

#### 查看 VLAN
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-19.png)
也可使用 show vlan brief 查看交換機上完整 VLAN 信息。

#### 刪除 VLAN
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-20.png)
注意到，即使刪除 VLAN，那些原本屬於該 VLAN 的端口仍然會被關連在一起，直到被重新分配為止

#### 路由器配置
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-21.png)
一個路由器接口理論上只能被一個 VLAN 占用，但是這樣顯得有些浪費，所以就出現了劃分子接口的做法。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-22.png)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-23.png)
如上圖，就是說物理上還是只有一個 FA0/0 接口，但是邏輯上會劃分出三個子接口，有各自的 ip 地址，且被分配給不同的 VLAN。

具體看看下面一個實際配置子接口 VLAN 的過程 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/vlan-24.png)

|物理接口|子接口|
|:-------:|:-------:|
|每個 VLAN 占用一個端口|多個 VLAN 占用一個端口|
|無帶寬爭用|有帶寬爭用|
|連接到 Access Link|連接到 Trunk Link|
|成本高|成本低|
|配置複雜|配置容易|

