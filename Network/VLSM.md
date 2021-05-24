@[TOC](一篇搞懂 VLSM)

# 什麼是 VLSM ?
VLSM 的全稱是 Variable Length Subnet Mask。顧名思義，就是允許不同子網使用不同的子網掩碼。根據不同子網所需要的 ip 地址數，各自劃分子網和分配子網掩碼。

# 為什麼要有 VLSM ?
之前在 [OSI Network Layer 網絡層](https://blog.csdn.net/weixin_46803507/article/details/106865151) 介紹了如何對一個網段劃分所需的子網的方法，如果不清楚或忘了可以先去看看這篇。但值得一提的是，不見得所有情況下，每一個子網都需要一樣多的地址分配給主機。來看看下面這個例子:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-23.png)

可以看到這個網絡擁有 7 個子網。但是除了 Perth, Brisbane, Sydney, Melbourne 需要 30 個 ip 地址以外，其他三個路由器互連的子網其實只需要 2 個 ip 地址，那如果一樣都為每個子網分配一樣多的 ip 地址，就會造成很大一部份的浪費。於是，VLSM 就是為了解決這樣的需求。

# VLSM 工作機制
接下來就直接通過一個例子來瞭解完整的 VLSM 怎麼做到的!
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-24.png)

如上圖，我們現在要為 192.168.10.0/24 劃分子網。總共有 7 個子網。這邊補充一個常用寫法。
> 192.168.10.0/24 表示對於該 ip 地址有 24 位是用來表示網絡號的。

## Step0
整理下需求和 VLSM 大致工作的機制如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-25.png)

在開始之前再提一個點:
> 之前說劃分的子網的 host 位全 0 全 1 都是不可以分配給主機的。但是在 VLSM 中，全 0 是可用的。

那現在就按著上面的原則，來用 VLSM 劃分子網。

## Step1
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-26.png)

Perth 劃分後情況:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-27.png)

Perth 子網的子網掩碼是 255.255.255.192。且下一個開始分配的 ip 地址是 192.168.10.64/26。

## Step2
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-28.png)

KL 劃分後情況:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-29.png)

==注意，KL 的地址分配只到 192.168.10.95，192.168.10.95 才是廣播地址==

KL 子網的子網掩碼是 255.255.255.224。且下一個開始分配的 ip 地址是 192.168.10.96/27。

## Step3
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-30.png)

Sydney 和 Singapore 劃分後情況:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-31.png)

==注意，Singapore 的地址分配只到 192.168.10.127，192.168.10.127 才是廣播地址==

Sydney 和 Singapore 子網的子網掩碼是 255.255.255.240。且下一個開始分配的 ip 地址是 192.168.10.128/28。

## Step4
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-32.png)

劃份完 WAN links 後情況:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-33.png)

## Overview
這麼一來，就都劃分好了所有子網了。來看看完成後的拓樸:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/net-34.png)

可以看到，使用 VLSM 可以允許我們對於不同大小的子網分配不同的子網掩碼和不同數量的 ip 地址，可以避免 ip 地址大量浪費。

