@[TOC](網絡安全與加密系統概述)

# 網絡安全問題
在網絡安全的範疇中，主要分為四種網絡安全問題 :

|問題|描述|
|:-----:|:-----|
|截獲|從網絡上竊取他人通信內容|
|中斷|有意中斷他人的通信過程|
|篡改|有意修改網絡上通信的信息|
|偽造|偽造信息在網絡中傳輸|

對於這四類安全問題，可以分為主動攻擊和被動攻擊。被動攻擊是說，不改變整個網絡通信的過程，相反的，主動攻擊當然就改變了網絡通信的過程。看看下圖 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-1.png)

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-2.png)

大家常常聽到的計算機病毒，諸如特洛伊木馬病毒，或是邏輯炸彈都是網絡安全問題的隱患。

所以整理出幾個網絡安全的目標如下 :

- 防止信息被分析
- 防止通信量被分析
- 檢測更改報文流
- 檢測拒絕報文服務
- 檢測偽造信息行為

# 數據加密模型
下圖是一般的數據加密模型 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-3.png)

可以看到，會有一個加密和解密算法，和加密解密鑰史。

# 對稱密鑰系統
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-4.png)
對稱式加密就是傳送方與接收方的加解密皆使用同一把密鑰，所以只要雙方都擁有這把鑰匙，當傳送方傳資料時，使用這把鑰匙加密，接收方收到訊息後，再用同一把鑰匙解密，就能解開訊息了，運作原理如下圖所示：
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-5.png)

# 非對稱密鑰系統
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-6.png)
非對稱式加密就是每個使用者都擁有一對金鑰：公開金鑰(Public key)及私密金鑰(Private key)，公開金鑰能被廣泛的發佈與流傳，而私密金鑰則必須被妥善的保存；當訊息由其中一把金鑰加密後，就必須用另一把金鑰解密，加解密的鑰匙要是完整一對的，所以可以是公鑰加密私鑰解密，也可以是私鑰加密公鑰解密，沒有一定。

運作原理是傳送方與接收方在傳送之前，先把彼此的公鑰傳給對方，當傳送方要傳送時，就用接收方的公鑰將訊息加密，接收方收到加密訊息後，再用自己的密鑰解開，這樣即使有心人拿到公鑰，只要沒拿到接收方的私鑰，也還是無法解密訊息。

# 防火牆
大家對於防火牆這個名詞肯定不會太陌生，那麼就來看看防火牆到底是神麼東西!
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-7.png)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-8.png)
而防火牆提供的就兩個功能，允許和阻止。阻止就是說，防火牆阻止某些類型的信息通過，允許則相反。大部分來說，防火牆存在的目的還是做為阻止作用的。

# ACL
ACL 的全稱是 Access Control List。ACL 就是防火牆實現的技術。

其實 ACL 就是在路由器上配置一些指令，來告訴路由器那些報文是可以通過，那些要拒絕的。而如果路由上配置了 ACL，那麼報文就會進行順序的測試。測試不是通過就是拒絕，當都沒有指令匹配時，ACL 指令最後會有隱式的 `deny any` 指令，因此報文就會被丟棄。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-9.png)

## 標準 ACL
標準 ACL 的做法比較單純，只基於報文中的源地址作為判斷依據。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-10.png)

### 配置標準 ACL
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-11.png)
注意到，這邊在配置時，如上圖，192.5.5.10 0.0.0.0 的前面還是 ip 地址，但後面不是我子網掩碼了，是 wildcard mask。這邊必須說明什麼是 wildcard mask。

### wildcard mask
之所以使用 wildcard mask，是因為既然我們配置了 ACL，那對於報文，必須以一個機制檢驗其中的源地址是否符合我們所配置的條件的地址。檢測也不見得就是一個主基地址，也許 ACL 中配置的是一個網段等等。

簡單來說，wildcard mask 會告訴我們那些位是要檢查，那些位是不用檢查的。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-12.png)

那之前說過，如果報文都不符合所有 ACL 配置指令，那麼就會丟棄掉這個報文。但有時候，我們是想要做到，如果都不符合條件，就都允許。ACL 也提這樣的做法 :
> Router(config)#access-list 1 permit any

有時候，我們也想只針對一個主機配置 ACL，當然我們可以這麼做 :
- 假設今天想拒絕 192.5.5.10 主機
> Router(config)#access-list 1 deny 192.5.5.10 0.0.0.0

但是，ACL 也提供了另一種更直觀的寫法 :
>Router(config)#access-list 1 deny host 192.5.5.10

### 標準 ACL 缺陷
因為標準 ACL 的配置只能以源地址作為判斷，所以約定是要將 ACL 配置盡可能接近目標主機。但是這樣會造成在過程中的浪費，看看下面這個例子 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-13.png)
如圖，我們想要讓 192.5.5.0 網段的主機不能訪問 210.93.105.0 網段的主機。按照約定，我們要將 ACL 配置在 Lab D 的 E0 端口上。但是這麼一來，由 192.5.5.0 所發出前往 210.93.105.0 的報文仍然會在網絡中傳輸，造成一些網段資源的浪費。於是，就出現了擴展 ACL。

## 擴展 ACL
擴展 ACL 不再侷限於源地址作為判斷依據，而多了許多依據。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-14.png)

### 配置擴展 ACL
當然，配置擴展 ACL 也就會更複雜些。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-15.png)
可以看到，擴展 ACL 允許我們配置更多參數。如圖的配置就是說，在路由器上配置一個編號 101，拒絕使用 tcp 協議，從 192.5.5.0 網段到 210.93.105.0，且上層使用了 telnet 協議的報文。

因為擴展 ACL 除了源地址的判斷條件外，多了目的地址也能判斷，所以就可以避免掉標準 ACL 的缺陷，將 ACL 配置盡可能接近發送方。看看下面的圖 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-16.png)

可以看到，擴展 ACL 就可以避免掉多餘的浪費。

此外，除了為 ACL 配置編號外，我們還可以為 ACL 配置名字，方便其他人或是很久之後，可以理解配置該 ACL 的用意。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-17.png)

我們也能用以下指令查看 ACL 配置的相關信息 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-18.png)

## ACL 編號
最後，介紹下配置標準 ACL 和擴展 ACL 的編號規約，參考下面的表格 :
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sec-19.png)

