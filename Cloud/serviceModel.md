@[TOC](關於 IAAS PAAS SAAS)

# 前言
最近開始接觸雲計算相關的技術棧以及知識點，所以算是一個剛剛入們的菜鳥。對於很多概念總是覺得抽象，難以理解，而關於雲計算的三種服務模型，就是一直感到抽象的部分。這篇就來整理下關於雲計算的三種服務模型。

# 正文

## 3 大服務模型
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sm-1.png)
在談論到雲計算這個近幾年火熱的話題時，勢必會提到搭建雲計算服務的三種服務模型。分別是 IAAS, PAAS, SAAS。AAS 是 As-A-Service 的縮寫，而三個首字母則分別代表 Infrastructure, Platform, Software。接下來就來說說分別的涵義。

接下來會以比較生活的例子，pizza，來解釋三者的概念。相信大家對 pizza 都十分熟悉才對！

## IAAS 基礎設施服務
IAAS 就像是，由第三方提供廚房的設備，包括爐子，媒等等。而你就得基於這些基礎設施，做出你想要的披薩。

## PAAS 平台服務
PAAS 就像是，除了第三方提供那些基礎設施以外，還提供製作披薩所需要的基底麵皮。而你可以直接在麵皮上開始鋪上你所希望的配菜。PAAS 就像是你只要基於提供服務的平台設計，其餘工作都交由第三方處理就行。

## SAAS 軟件服務
SAAS 則像是，重頭到尾都不需要你動手，你會拿到一個做好的披薩，而你只負責賣或是吃，看你想怎麼用。

整理下上述的概念可以彙總成下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sm-2.png)

## IAAS vs PAAS vs SAAS
從更生活化的角度來看，可以這麼理解:
SAAS 就像是我們平常使用的軟件，我們根本不用關心甚麼技術，部屬，底層硬件邏輯，我們只管用就對了。像是微信，QQ等等都是 SAAS 的例子。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sm-4.png)![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sm-3.png)
PAAS 則像是提供給你一個讓你部屬你的軟件應用的平台，而不需要你去關注底層硬件細節。開發者只需要關注自己設計的邏輯就好。Google App Engine 就是 PAAS 的例子。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sm-5.png)
而 IAAS 則只提供給你最基礎，最底層的資源。用戶就需要自己控制底層，注意硬件細節，操作系統等等。Amazon EC2 就是 IAAS 的例子。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sm-6.png)
去年去了微軟實習時，稍微用到了 Microsoft Azure，也就是微軟自己開發的雲計算服務平台。其中也有這麼一張圖，描述了這 3 大服務平台的區別:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/sm-7.png)
可以看到，SAAS 就是幫你把所有事情都完成，你沒有任何自主權，你只需負責使用，PAAS 可以自主安裝應用，但從操作系統至底層都由平台完成，而 IAAS 則是只由雲服務商提供虛擬的硬件，而從操作系統到應用程序全部都可以由用戶自行配置。

讀到這邊應該對 IAAS, PAAS, SAAS 有一定程度的了解了。最後來看看專業的大牛們都怎麼說這三者的關係:
> IAAS:基礎設施即服務。IAAS 模式提供用戶所有計算基礎設施的服務，包含 CPU, memory, 網絡等等。用戶負責進行其餘一切配置，部屬，應用安裝等等的工作。

> PAAS:平台即服務。PAAS 模式相當於將用戶自行開發的應用程序，部屬到雲服務商提供的計算基礎資源上，提供的是一個基於底層計算資源的服務平台供用戶應用程序運行。

> SAAS:軟件即服務。SAAS 模式提供直接，完整的軟件給用戶使用。用戶通過自己的設備的客戶端介面進行使用和訪問，無須管理或控制任何計算基礎設施或計算資源。強調「拿來即用」的概念。

# 結語
雖然本人也還只是剛剛開始接觸雲計算的小白，但還是能知道，理解這三種不同的雲計算服務模型對這個領域的重要性。其實這三種模式也不僅僅限於雲計算，也可以應用到其他軟件開發的理解。在這邊感謝阮一峰大大關於 IAAS, PAAS, SAAS 的文章，本人也有許多部份是參考來整理的，若有錯誤，還請大家多多指教。
