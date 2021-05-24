@[TOC](ARP 協議)

# 為甚麼要有 ARP ? 
ARP 協議的全名是 Address Resolution Protocol。
而為什麼會有 ARP 協議的出現呢?是因為，之前在介紹數據鏈路層時，我們就知道了，數據報的傳輸在數據鏈路層會形成一個幀 (frame) 的傳輸單元，而這個幀會包含源主機和目標主機的 MAC 地址。 那對於源主機，缺少的信息就是目標主機的 MAC 地址。ARP 協議的用途就在於可以透過一個已知的 IP 地址 (目標主機的 IP 地址)，來獲取相應的 MAC 地址，以達到通訊的目的。

# ARP 表
首先有個概念，每個主機或是路由器都會有一個自己的 ARP 緩存，存在 RAM 中。存取的內容就像一個個 IP 地址對應的 MAC 地址的條目的表。
> 大家可以到命令行下輸入 arp -a，就可以看到自己主機的 ARP 信息

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-13.png)

如下圖，197.15.22.33 想要發送數據報給 197.15.22.126，但是 ARP 表中並沒有 197.15.22.126 的對應條目。於是我們需要 ARP 協議。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-14.png)

# ARP 協議機制
我們以下面這個情境說明 ARP 的原理:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-15.png)

此時 10.0.2.1 想和 10.0.2.9 通訊，但因為 ARP table 沒有相應條目所以無法形成幀。

Step1 : 主機 A 會發送一個 ARP request。這個 ARP request 必須廣播出去，以確保所有網段內的主機都會收到。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-16.png)

==注意，目的主機地址全 1，為 ff.ff.ff.ff.ff.ff，因此這個 ARP request 是廣播的==

Step2 : 接著，所有本網段的主機都會收到這個 ARP request，並開始核對自己的 IP 地址是否和 ARP request 中的目的 IP 地址相同。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-17.png)

Step3 : 10.0.2.9 發現自己的 IP 地址和 ARP request 中的目的 IP 地址相同。這時，10.0.2.9 會發送一個 ARP reply 給 10.0.2.1。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-18.png)

可以看到，這個 ARP reply 就會包涵目標主機的 MAC 地址。

Step4 : 當 10.0.2.1 接收到這個 ARP reply 後，就會在自己的 ARP table 中增添一個條目。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-19.png)

這樣一來，10.0.2.1 就可以成功的形成要發送給 10.0.2.9 的幀。原本未知的 10.0.2.9 MAC 地址現在存在於自己的 ARP table 了。

>值得一提的是，當 10.0.2.1 發送 ARP request，且 10.0.2.9 響應時，10.0.2.9 也會更新自己的 ARP table，將 10.0.2.1 和對應的 MAC 地址寫盡自己的 ARP table。

總結一下，APR 機制大致上遵照下圖的流程:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-21.png)

都理解了上面的情況和 ARP 的工作機制後，我們發現，上述的主機都是在同個網段。那今天如果我想要通信的目標主機和源主機不在同個網段呢?接下來，就要介紹兩種解決方法。

# Default Gateway 默認網關
首先我們得知道，一個路由器包含很多個端口，且每個端口都是一個網關，而每個網關都有網卡，有自己的 MAC 地址和 IP 地址。

所謂的默認網關，就是說路由器上屬於源主機網段的端口的 IP 地址。當源主機和目標主機不在同網段時，只能把數據報發送給默認網關，由默認網關幫我轉發給目標主機。

# Proxy ARP
對於一些設備可能不支持配置默認網關，這時就使用 Proxy ARP。Proxy ARP 的意思是代理的 ARP。作法首先會在路由器上啟動這個 Proxy ARP 的進程。

源主機一樣要廣播一個 ARP request，而當路由器收到 ARP request 之後，因為啟動了 Proxy ARP，所以會給一個 ARP reply。但是這個 ARP reply 所包含的 MAC 地址並不是目標主機的，而是源主機所屬網段的網關的 MAC 地址。所以源主機收到 ARP reply 後，其實是將數據報發送到默認網關上，再由默認網關轉發到目標主機網段。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-20.png)

代理的想法隱含在源主機其實感覺就像直接跟目標主機進行了通訊，但不知道的是，其實中間其實是透過默認網關代理幫忙轉發數據報的。
