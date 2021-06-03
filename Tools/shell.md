@[TOC](Shell 編程攻略)

# 參考
|來源|鏈接|
|:-----|:-----|
|runoob教程|https://www.runoob.com/linux/linux-shell.html|

# 什麼是 Shell?
大家都一直在說 shell，那到底什麼是 shell。Shell 其實是用 C 語言寫的， 相當於是用戶跟 linux 內核溝通的一個橋樑。Shell 既是一種命令語言，又是一種程序設計語言，通過命令行使用 shell 這個應用程序，用戶就可以和內核的一些服務等等打交道。可以將 shell 理解為作為一個核外程序，提供給用戶訪問操作系統的一個接口。
![截圖 2021-04-14 下午4.27.58.png](https://sc04.alicdn.com/kf/Ue166ef9a5d4145ed83cb6d8295353223T.jpg)

而所謂的 shell script，其實就是為 Shell 編寫的腳本程序。大家通常說的 shell 其實都是指的 shell 腳本，但其實 shell 和 shell script 是兩個東西，這邊要區分一下。

# Shell 種類
Shell 其實有很多種，下面列出一些常見的：

- Bourne Shell（/usr/bin/sh或/bin/sh）
- Bourne Again Shell（/bin/bash）
- C Shell（/usr/bin/csh）
- K Shell（/usr/bin/ksh）
- Shell for Root（/sbin/sh）
- ......

其中最常見的就是 bash，是大多數 linux 系統下默認的 shell，下面的介紹也都將圍繞 bash 開展。接下來我們就來看看要怎麼編寫 shell 腳本。

# 第一個 shell 腳本
這邊就先照著做，命令下面都還會一一介紹，只是先讓大家有個感覺而已。

首先我們隨便在任意目錄下創建一個 `test.sh`，可以使用 `touch` 指令，在 `test.sh` 裡面寫一行簡單的代碼。
```sh
#! /bin/bash
echo "Hello World!"
```
`#!` 是一個標記，告訴系統要用哪一種 shell 來解釋腳本。

接著，我們要使腳本使用執行權限，我們使用 `chmod` 指令，接著就執行這個腳本。具體如下：
```cmd
chmod +x test.sh
./test.sh
```
應該就會看到命力行輸出了 `Hello World!` 了。

# Shell 變量
Shell 中的變量通過 `[var]=[val]` 表達式聲明即可，規範這邊不多作介紹，具體可以自己百度就好。而要訪問定義過的變量，只需要在前面加上 `$` 就好。
```sh
name="cclin"
echo $name
```
我們也可以在變量周圍加上 `{}` 花括號，其實效果都一樣，但就只是有時候為了分清楚變量的邊界。
```sh
for skill in C, Java, JavaScript; do
  echo "I am good at ${skill} language"
done
```
Shell 中的字符串雖然雙引號或單引號都可以，但是建議都用雙引號比較好。

# Shell 數組
Shell 中也有數組，不過只提供一維的，初始化時不需要定義大小，且下標也是從 0 開始。語法格式如下：
```sh
array_name=(value1 value2 ... valuen)
```
數組用 `()` 表示，裡面的元素之間用空格分開就好。我們也可以用下標來定義數組：
```sh
array[0]=value0
array[1]=value1
```
訪問數組元素則要通過 `${}`，如下：
```sh
echo ${array[index]}
```
使用 `@` 或是 `*` 可以獲得數組所有元素。
```sh
array=(a b c d)
echo ${array[*]}
echo ${array[@]}

// a b c d
```
# read
`read` 命令用於讀取用戶輸入，下面給出整理的參數：
|參數|描述|
|:-----|:-----|
|-p|指定要顯示的提示|
|-s|靜默輸入，一般用於密碼|
|-n #|指定輸入的字符長度最大值|
|-d '字符'|輸入終止符，當用戶輸入的內容出現目標字符，立即結束輸入|
|-t N|超出 N 秒沒有輸入，則自動退出|

- 基本用法

```sh
#! /bin/bash
# read.sh

echo -n "Enter your name: " # -n 可以讓用戶直接在後面輸入
read name
echo "Hello $name, welcome to Shell!"
```
```sh
./read.sh

Enter your name: cclin
Hello cclin, welcome to Shell!
```
- read -p

```sh
#! /bin/bash
# read.sh

read -p "Enter your name: " name
echo "Hello $name, welcome to Shell!"
```
```sh
./read.sh

Enter your name: cclin
Hello cclin, welcome to Shell!
```
- 指定多個變量

```sh
#! /bin/bash
# read.sh

read -p "Enter two numbers: " a b
sum=$[$a+$b]
echo "The sum is $sum"                 
```
```sh
./read.sh

Enter two numbers: 1 2
The sum is 3
```
- REPLY

如果沒有在 read 命令後面指定變量，那麼 read 會將收到的所有數據都放到一個特殊環境變量 `REPLY` 中。
```sh
#! /bin/bash
# read.sh

read -p "Enter a number: "
factorial=1
for((count=1; count<=REPLY; count++))
do
  factorial=$[$factorial * $count]
done
echo "The factorial of $REPLY is $factorial"                    
```
```sh
./read.sh

Enter a number: 6
The factorial of 6 is 720
```
- read -t

```sh
#! /bin/bash
# read.sh

if read -t 3 -p "Enter your name: " name
then
  echo "Welcome, $name"
else
  echo ""
  echo "Such a slowpoke!"
fi
```
```sh
./read.sh

Enter your name: 
Such a slowpoke!
```
```sh
./read.sh

Enter your name: cclin
Welcome, cclin
```
- read -n
```sh
read -n1 -p "Do you want to continue?[Y/N]" res
case $res in
Y | y) echo
  echo "Fine, go on...";;
N | n) echo
  echo "OK, bye.";;
*)
  echo "error choice";;
esac
exit 0
```
```sh
./read.sh

Do you want to continue?[Y/N]y
Fine, go on...
```
- read -s
```sh
read -s -p "Enter password: " pwd
echo
echo "Password is $pwd"
```
```
./read.sh

Enter password: 
Password is cclin
```
# 字符串比較

|比較符|描述|
|:-----|:-----|
|str1=str2|两个字符串相同则结果为真|
|str1!=str2|两个字符串不相同则结果为真|
|-z str|字符串为空则结果为真|
|-n str|字符串不为空则结果为真|

# 算術比較

|比較符|描述|
|:-----|:-----|
|expr1 -eq expr2|两个表达式相等则结果为真|
|expr1 -ne expr2|两个表达式不相等则结果为真|
|expr1 –gt expr2|expr1 大于 expr2 则结果为真|
|expr1 –ge expr2|expr1 大于或等于 expr2 则结果为真|
|expr1 –lt expr2|expr1 小于 expr2 则结果为真|
|expr1 –le expr2|expr1 小于或等于 expr2 则结果为真|

# 邏輯操作
|比較符|描述|
|:-----|:-----|
|!expr|逻辑表达式求反|
|expr1 -a expr2|两个逻辑表达式“And”(“与”)|
|expr1 -o expr2|两个逻辑表达式“Or”(“或”)|

# if/elif/else
```sh
if [ expression ]
then 
  statements
elif [ expression ] 
then
  statements 
elif ...
else
  statements 
fi
```
同一行上如果有多個命令，通常在 shell 中我們用 `;` 分隔。

# case
```sh
case str in
str1 | str2) 
  statements;; 
str3 | str4) 
  statements;; 
*) 
  statements;;
esac
```
# for
```sh
for var in list 
do
  statements 
done
```
# while
```sh
while condition 
do
  statements 
done
```
# select
`select` 命令比較有趣，用於生成簡單的菜單，可以跟用戶做一些簡單的交互。直接看一個例子就好：
```sh
#! /bin/bash
# select.sh

echo "Choose a language"
select item in Java C JavaScript
do
  case "$item" in
    Java) echo "Backend";;
    C) echo "Game Develop";;
    JavaScript) echo "Frontend";;
    *) exit 0
  esac
done
```
```sh
./select.sh

Choose a language
1) Java
2) C
3) JavaScript
#? 1
Backend
#? 2
Game Develop
#? 3
Frontend
#? 4
```
# Shell 函數
Shell 中也提供了函數，大致形式如下：
```sh
[ function ] funname [()]

{
    action;

    [return int;]
}
```
話不多說，直接來看看函數的使用方式。
```sh
#! /bin/bash
# func.sh

add() {
  read -p "Enter first number: " num1
  read -p "Enter second number: " num2
  sum=$[${num1}+${num2}]
  echo "The sum is ${sum}"
}

add
```
```sh
./func.sh

Enter first number: 1
Enter second number: 2
The sum is 3
```
如果函數有返回值，那麼我們也可以通過 `?$` 來進行捕獲。關於函數傳參其實也很簡單，具體見上面參考鏈接，本篇文章只是開一個 shell 編程的頭。

