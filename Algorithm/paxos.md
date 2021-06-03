@[TOC](深入淺出 Paxos 算法)

# 前言
對於 paxos 算法，真的必須承認，是一個比較複雜且難懂的算法，所以其實寫這篇也是希望能夠通過這樣的方式，強迫自己多看些資料，逼自己看看能不能夠稍微理解下 paxos 的全貌。
paxos 算法在分布式領域有著非常重要的地位，但是，眾所皆知非常難懂，且在現實工程中更加難以實現。這篇就來看看 paxos 工作的原理和工作機制。

# 正文

## 什麼是 Paxos ?
> Paxos 是一個基於消息傳遞，且具有高度容錯特性的一致性算法，應用於分布式領域的一致性問題。

## 問題背景
在分布式系統中，常見的問題像是機器當機或是網路延遲等問題都是有可能發生的。而 Paxos 算法就是希望能解決在問題發生的分布式系統中，能夠盡快在集群內部就一個值達成一致。

這裡說的值(value)，不限於某個數。像是一條日誌，一條命令等等，在不同分布式場景下，值可以是不同的含意。

## Paxos 三種角色
在 paxos 算法中，有三種角色:
1. Proposer
2. Acceptor
3. Learner

在一個集群中，任何機器上的進程都可以同時充當多個角色。

這邊引進一個新的概念，提案(propose)。最終要達成一致的 value 就在 propose 中。

==注==:
>1. 暫且認為 提案 = value，即提案只包含 value。
>2. 暫且認為 proposer 可以直接提出提案。

Proposer 可以 propose 一個提案，而 Accepter 可以選擇接受或者拒絕一個提案，一旦某個提案被接受(chosen)，該提案中的 value 就會被選中。

在前面提到，paxos 希望做到的就是對於某個 value 達成一致。所謂一致就是指 proposer, accepter, learner 都認為同一個 value 被 chosen。那麼就來看看，三種角色分別在怎樣的情況下可以認為一個 value 被 chosen。
- Proposer
只要 propser 提出的提案被 acceptor 接受，就可以認為該提案中的那個 value 被 chosen。(下一節開始會先假設只要一個 acceptor 接受就可以，可是稍後就會發現要一半以上的 acceptor 接受)

- Acceptor
只要 acceptor 接受了某個提案，就可以認為該提案中的那個 value 被 chosen。

- Learner
由 acceptor 告訴 learner 哪個 value 被 chosen，learner 就認為該 value 被 chosen。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/pax-1.png)

在進入 paxos 算法的推導前，再提醒下:
>Paxos 目標:保證最後有一個 value 被 chosen，且集群內的機器最終都能學習到這個 value

## 推導 Paxos 算法

### 如果只有一個 Acceptor ?
假設只有一個 acceptor(proposer 數量沒有限制)，只要 acceptor 接受他收到的第一個提案，按照上面的規則，該提案中的 value 就是被 chosen 的 value，且保證只有這一個 value 被 chosen。

但是，如果這唯一一個 acceptor 崩潰或當機了，那整個系統就會 crash。因此，必須有多個 acceptor。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/pax-2.png)

### 多個 Acceptor
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/pax-3.png)
現在有多個 acceptor，那要怎麼保證在多個 proposer 跟多個 acceptor 的情況下選定一個 value ?

### 尋找解決辦法
首先，必須保證在即使只有一個 proposer 提出了 value 的情況下還是會有 value 被 acceptor 接受，因此有了第一個約束:
> R1 : 一個 acceptor 必須接受第一個收到的提案。

![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/pax-4.png)
如上圖，R1 導致了一個問題，也就是多個 value 都被選中。為什麼會這樣呢 ? 原因就在於剛剛說只要一個提案被 acceptor 接受就視為 value 被接受。因此為了解決這樣的問題，我們要加上一個約束:
> 一個提案真正被選定必須要被半數以上的 acceptor 接受

再進一步想想，這暗示著一個 acceptor 必須要可以接受不只一個提案，不然根本不可能有一個提案被半數以上的 acceptor 接受。

也因為這樣，上述說的 提案 = value 已經不能滿足需求，因為必須有辦法區分每個提案，所以修改提案，令提案 = 提案id + value，其中 提案id 表示該提案被提出的順序。

而雖然允許多個提案被選定，但又必須保證所有被選定的提案都有同樣的 value，不然就失去 paxos 的意義了。於是多了一個約束:
> R2 : 如果一個 value = v 的提案被選中了，那麼每個 id 更高的提案若被選中，value 也必須為 v

而一個提案要被選定也必定是被 acceptor 接受，因此也可以把 R2 改寫成這樣:
> R2a : 如果一個 value = v 的提案被選中了，那麼每個 id 更高被 acceptor 接受的提案的 value 也必須是 v

那麼有了這些基本概念後，看看這個情況:
假設今天有 5 個 acceptor 和兩個 proposer，但是 acceptor1 一開始當機了，而 proposer2 提出了 [I1, V1] 的提案，根據 R1，acceptor2-5 就接受了該提案，又因為過半數 acceptor 都接受這個提案，所以就認為 V1 為選定的 value。這時，acceptor1 恢復正常工作，而 proposer1 提出了 [I2, V2] 提案，這邊 I2 > I1。同樣根據 R1，acceptor1 必須接受該提案，並且就認為 V2 為被選中的 value。這就出現了兩個問題:
1. V1 != V2 ，出現不一致問題
2. M2 > M1 , V1 != V2，與 R2a 矛盾
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/pax-5.png)

所以要對 R2a 更進一步的強化。R2a 是對 acceptor 接受提案的約束，但其實追溯源頭，提案其實也是由 proposer 提出的。因此其實應該調要對 proposer 進行約束才更合理一點。
> R2b : 如果一個 value = v 的提案被選中了，那麼之後任何 proposer 提出的 id 更高的提案的 value 也必須是 v

這就引出另一個問題，要怎麼確保在某個 value = v 的提案被選中後，proposer 提出更高 id 的提案的 value 也都是 v 呢?
於是再進行強化:
> R2c : 對於任意組合為 [N, V] 的提案，集群內半數以上的 acceptor 集合 S 滿足以下兩個條件的任意一個:
- S 中的 acceptor 都沒有接受過 id 小於 N 的提案
- S 中接受過最大 id 的提案的 value = V

### Proposer 生成提案 - prepare
為了滿足 R2b，要引進一個新的概念 : proposer 在提出提案之前要經過一個學習的過程。所謂學習的意思就是說，應該先去「學習」已經被選定的提案中的 value 或可能被選定的 value，並將該 value 作為自己的提案的 value 提出。如果學習之後都發現尚未有參考的 value，才可以自己去決定自己提案中的 value。這樣學習的過程其實是通過一個「Prepare 請求」實現的。

Prepare 請求使用的提案生成算法如下:
1. Proposer 選擇一個新的提案 id = N，然後向集合 S(半數以上的 Acceptor 組合)發送 prepare 請求，要求 S 中的每個 acceptor 都必須做出如下回應(response):
- 向 proposer 承諾不會再接受 id < N 的提案
- 如果 acceptor 已經有接受過了提案，那麼要向 proposer 回報已經接受過且 id < N 的最大 id 的提案

>我們將這稱為編號 N 的 prepare 請求。
2. Proposer 在收到所有 response 之後，就可以生成形如 [N, V] 的提案。N 就是自己決定的。V 則是根據所有 response 做學習，選擇 acceptor 回報提案中 id 最大的提案的 value。如果所有 response 中都沒有回報提案，才可以由 proposer 自己決定 value。

至此，proposer 生成提案，並將提案發送給半數以上的 acceptor，希望這些 acceptor 可以接受該提案，這步稱為「Accept 請求」。

### Acceptor 接受提案
Acceptor 可以忽略任何請求，包括 prepare 和 accept 請求，不用擔心破壞 Paxos 的完備性。而至於甚麼時候 Acceptor 要接受一個請求，就是這邊要討論的部分。

首先，對於 Acceptor 給出如下約束:
> R3a : 如果 acceptor 還沒響應或響應過任何編號大於 N 的 prepare 請求，那該 acceptor 就可以接受這個編號為 N 的 accept 請求，意即接受該提案

如果 acceptor 收到一個 編號 = N 的 prepare 請求，而在此之前已經響應過 編號 > N 的請求，根據 acceptor 給之前 proposer 的 response 規則，acceptor 就不會接受這個請求。

因此，總結一下，Acceptor 只需記住兩個東西:
1. 已響應的的 prepare 請求最大的編號
2. 已接受的提案最大的 id
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/pax-6.png)

### Paxos 算法整體流程
看到這邊，相信都已經有些眼花了。這部分稍微總結下 paxos 算法的整體工作機制。

Paxos 算法大致可以分為兩個階段:

#### Stage1

(a) proposer 生成一個編號 = N，並向半數以上的 acceptor 發出編號為 N 的 prepare 請求。

(b) 當 acceptor 收到這個 編號 = N 的請求，且 N > 該 acceptor 已經響應過的 prepare 請求的所有編號，acceptor 就可以接受這個 prepare 請求，並 response 承諾不會再接受 編號 < N 的 prepare 請求。

#### Stage2

(c) 如果 proposer 收到了半數以上的 acceptor 對於 prepare 請求的 response，那這時候就表示學習成功，proposer 就可以發送一個針對 [N, V] 提案的 accept 請求給半數以上的 acceptor。這邊的 V 是由 acceptor response 中最大 id 的提案的 value，如果 response 都沒有提案存在，proposer 可以自己決定 V 的取值。

(d) 這時 acceptor 收到一個 [N, V] 的提案的 accept 請求。只要 acceptor 沒有響應過 編號 > N 的 prepare 請求，就接受該 accept 請求，也就是接受 [N, V] 這個提案。
 

### Learner 學習被選定的 value
前面提到 Paxos 算法中有三個角色，Proposer Acceptor Learner，也說過一個提案最終認為被選中必須要三個角色都認可，卻一直沒提到 Learner。

經過上述的整個算法過程後，會有一個 [N, V] 的提案被選中。這時候，就是要讓 Learner 也學習到這個提案中的 value = V。有三種方法:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/pax-7.png)

至此，就說完了關於 Paxos 這個應用於分布式領域的一致性算法的整個工作機制了。

# 結語
Paxos 算法真的比較複雜晦澀，對於沒那麼聰明 (like me) 的人學習上確實會有些困難，本人也是在寫完了這麼一篇之後才稍微理解了 Paxos 那麼一丁點，也不敢說是融會貫通。對於 Paxos 算法，還是得由自己去多體會其中的意義和推導過程，也希望讀者們看完這篇，對 Paxos 算法有清晰一點的了解。
