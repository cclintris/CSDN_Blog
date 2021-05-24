@[TOC](OSI Network Layer 網絡層)

# OSI 網絡層
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-1.png)


# 網絡層數據報
回憶一下，在數據鏈路層，數據是以幀(frame)的形式組織起來的。若還不清楚，可以去看看這篇 [OSI Datalink Layer 數據鏈路層](https://blog.csdn.net/weixin_46803507/article/details/106787953)。那這邊要介紹的，是網絡層的數據報文，首先來看看 packet 的結構。

# IP 數據報結構
這邊說的 packet 只是一個單位。而在網絡層傳輸的 packet 叫做 IP 數據報。

下圖就是 packet 結構 :

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-3.png)

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-2.png)

現在就一一來看看這樣一個 IP 數據報各個字段的用途跟涵意吧!

IP 數據報分為兩部分，首部和數據部分。以下是對於首部的字段介紹:
1. version : 表示IP報所使用的IP協議版本。現在有0100（IPv4），0110（IPv6）。
2. IHL(Internet Header Length) : 表示首部長度。4b 最多表示 15(1111)，而這邊的單位是以一行來算，也就是 4B 為一個字節。所以 15 的意思是說，首部最多可以有 15*4=60 個字節，而 60-20-40，是說扣掉固定部分 20 個字節，可變部分最多可以有 40B。
3. Service Type : 後期使用較少， 這邊不做介紹。
4. Total length : 指IP報文的總長度，最多是65535B(2^16-1)，但實際上，因為我們其實是把數據報放到數據鏈路層的幀結構中，所以一般不會超過 1500 個字節。
5. Identification : 因為IP是無可靠服務，報文不可以按序接收(TCP可以實現按序接收)，但當報文長度超過MTU而必須分片時，這欄位來標識多個分片是否屬於同一個報文，比如A報文被分成A1和A2，那麼A1和A2的Identification是相同的。Flags和Fragment Offest 結合實現在接受端組合分片報文
6. Flags : 这个地方的内容为『0DM』，其意义为：
> 1. 0： 不可用
> 2. D：若為 0 表示可以分片，若為 1 表示不可分片
> 3. M：若為 0 表示此 IP數據報為最後分片，若為 1 表示非最後分片
7. Fragment Offset : 表示該 IP 包在該組分片包中位置，接收端靠此來組裝還原 IP 包。這個字段是以 8B 為單位的。
8. TTL(Time To Live) : 表示數據報能夠在網絡中經過多少個路由器。不過其實要通過 255 個路由器也很難。
9. Protocol : 指定數據報該交由網絡層的哪一種協議處理。
10. Header checksum : 因為可靠性極低，後期使用較少， 這邊不做介紹。
11. Source IP addr : 源 ip 地址
12. Destination IP addr : 目標 ip 地址

==以上都是一個 IP 數據報所包含的固定首部==

# IP 地址
IP 地址是 32 位的，通常分 8 位寫成四部分。像是 192.168.1.7。而 IP 地址包含二部分，分別是網絡號和主機號。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-4.png)

## IP 地址分類
IP 地址主要分為 A,B,C 三類。每一類地址都會保留不同的位數給網號和主機號。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-5.png)

那有了這樣的分類以後，我們又要如何區分這是哪一類的地址，以解析該地址的網絡號和主機號呢?下圖的分類就滿清楚的:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-6.png)

可以看到，A,B,C 三類地址的開頭都不一樣，所以也造成了第一個字節取值的分別，因此，我們可以方便地得知任意 IP 地址屬於哪類。

這邊補充一個概念。IP 地址規定，有兩個地址不能分配給網絡中的設備。第一個就是 host 全取 0 時。這是因為，這樣的地址要保留起來表示該網絡的 network id。當網絡中欲通信的設備基於自己的 IP 地址計算出的 network id都一樣時，就可以直接通信，而不必透過第三層設備轉發。而當 host 全取 1 時，也不能分配給設備。因為 host 全為 1 的這個地址，表示的是該數據報是一個廣播數據報。

所以舉例來說，一個 C 類網段所能分配的設備數量就是 2^8 -2 個。全 0 全 1 都是不能作為分配用途的。

## 私有 IP 地址
對於 A,B,C 三類 ip 地址，還特別劃分了一些範圍屬於私有 ip 地址。私有的意思就是說，這些 ip 地址是不用於上網的，也就是說，這些 ip 地址可以用在不同的私有局域網，且不會產生衝突。那麼至於這些 ip 地址的設備要上 Internet，之後會介紹 NAT 技術。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-7.png)

私有 ip 地址主要可以解決 ip 地址耗盡的問題。若有興趣，可以參考 [全球IPv4地址耗尽意味着什么？](https://baijiahao.baidu.com/s?id=1651519779769763087&wfr=spider&for=pc)。


# 子網 (subnet)

## 子網是什麼 ? 
上述提到的 A,B,C 三類 ip 地址，我們可以看到，對於每個 ip 地址，是可以容納很多很多主機數的。但是其實要在同個網段中容納這麼多的主機數是不大可能的，所以會造成 ip 地址的浪費。這就是子網出現的原因。

有時候對於一個網段，我們還會需要劃分成更小的幾個網段，我們稱為子網。這樣可以增加靈活性，或是縮小廣播域等等。作法是，我們會跟 IP 地址的 host 借位，來當作 subnet 標示。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-8.png)

那問題來了，我們可以借幾位來做為 subnet id 呢?
最少我們必須要至少借 2 位。回顧下，剛剛我們說到，一個網段有兩個 IP 地址是不能作為分配設備用的，那就是 host 全 0 和 全 1。所以其實很好理解，因為如果今天只借 1 位，那對於這兩個子網，根本無法分配給任何設備。

那最多至少要給 host 留 2 位。原因也跟上面是一樣的，相信很好理解。

當然，借多少位取決於我們，但是我們當然是希望可以盡可能增加子網的使用效率，來看看下面這張表:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-9.png)

而劃分子網，因為提升了一些靈活性，所以難免會有些損失，但是我們還是希望可以盡量降低損失。

譬如說第一行的數據。首先這是對於 c 類地址的分析，如果只借了 2 位，那麼我們最多可以劃分為 2^2 -2 = 2 個子網。而每個子網又可以分配給  2^6 -2 = 62 個設備。所以總共可以分配 2*62 = 124 個主機。那如果不劃分子網，可以分配 2^8 -2 = 254 個主機，也就是說，利用率是 124/254 = 49%。所以可見，劃分子網也是有效率高低的。  

## 子網掩碼 (subnet mask)
子網掩碼的基本概念就是，要標明對於某個 IP 地址，那些位用作 network id，那些位用作 host id。舉例來說，對於一個普通的 A 類地址，他的子網掩碼就會是 255.0.0.0。把網絡號相應部分置為 1，其餘主機部分置為 0。那麼接下來直接來看看這個例子，綜合上面所介紹劃份子網的概念，還有子網掩碼的用途:

>給定 IP 地址 223.14.17.0。我們想對這個網段劃分子網，需要 13 個子網，每個子網要容納 10 台主機。

首先分析下，223.14.17.0 是一個 C 類地址，且因為 host 全為 0，所以表示的是一個網段號。

Step1 : 因為是個 C 類地址，所以默認子網掩碼是 255.255.255.0

Step2 : 要求要 13 個子網，所以說最少要跟 host 位借 4 位當作 subnet id。5 位可以嗎?不行，因為如果借 5 位，那 host 不夠每個子網容納 10 台主機。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-10.png)

Step3 : 因為發現要借 4 位當作 subnet id，所以把 subnet id 的部分置為 1。最後一個字節就變成 11110000，換算成十進制就是 240。所以在這樣的要求下，對於 223.14.17.0 這個網段的子網掩碼就是 255.255.255.240。

以上就完成了要求。那麼這個子網掩碼到底有甚麼用呢?有了子網掩碼和主機的 IP 地址，其實我們就可以回推出這個主機所在網段。怎麼做呢，看看下面的例子:

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-11.png)

子網掩碼的用途就在於，可以讓我們回推出該 IP 地址的主機所屬的網段，或者子網段，這樣對於我們的目標主機，如果計算出和源主機所在的網段是一樣的，就可以直接轉發，而不用透過路由器。

# 路由器 (Router)
路由器可以說是網絡層最代表的設備。我們可以把路由器想像成，一個路由器上面其實有很多端口(port)，而每一個端口都有配置一個網關(Gateway)，這個網關需要配對相應的網段的 network id。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-12.png)

每一個路由器還都會有自己的路由表，當一個報文到達某個端口時，路由器就會去查自己的路由表，來確定到底要交由哪個端口轉發該報文。

而構建路由表，主要分為兩種方式，動態和靜態的。

## static addressing
static addressing 的做法就是由網管去特別賦予設備一個特定，且不會改變的 IP 地址。

## dynamic addressing
現今主流的 dynamic addressing 方法是 DHCP，會再特別介紹。

還有關於路由器劃分衝突域和廣播域，以及和其他網絡設備比較的知識點，可以去看看 [搞清楚網絡中的設備](https://blog.csdn.net/weixin_46803507/article/details/106759417)。

# Routing Protocol 和 Routed Protocol
這邊要釐清這兩個概念，Routing Protocol 和 Routed Protocol。
> Routed Protocol 是說，這樣的協議是基於第三層做判斷，協助路由器之間報文的轉發，從而使得路由器之間可以成功連通。IP 協議就是一種 Routed Protocol。

> Routing Protocol 是說，這樣的協議仍然工作於第三層，只是它的目的不是要轉發報文，而是幫助路由器生成各自的路由表。

通過 Routing Protocol，路由器之間會動態的交換信息，成功的生成當下的路由表，再基於 Routed Protocol 成功轉發數據報。來看看一句總結的繞口令:
> Routing protocols determine how Routing protocols are routed.

理解二者的差別之後，其實還是很好理解的。下面我們再來看看具體的 Routing Protocol。

# 細說 Routing Protocol
Routing protocol 主要分為兩大類，IGP(Interior Gateway Protocol) 和 EGP(Exterior Gateway Protocol)。IGP 用在自治系統內，而 EGP 是跨自治系統的。而 IGP 又有更多的分類。

## IGP 協議
來看看關於 IGP 的分類；
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-22.png)


