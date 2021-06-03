@[TOC](Make 是甚麼以及它的用法)

# Make 是甚麼 ?
顧名思義，make 的意思就是做。譬如說，想做出一個文件 a.txt，那麼就可以執行下面的指令:
```makefile
$ make a.txt
```
但是如果你直接在命令行輸入這個指令是不會起作用的，因為 make 本身不知道要怎麼做出 a.txt，需要其他人告訴 make 怎麼做，也就是下面會介紹到的 Makefile。

比如說，假設 a.txt 要由 b.txt 和 c.txt 合併來得到，實際上指令應該要像這樣:
```makefile
a.txt: b.txt c.txt
    cat b.txt c.txt > a.txt
```
我們來解析下上面的指令。其實 make a.txt 本身可以分為兩步。第一步(第一行)，要先確認 b.txt 和 c.txt 存在。第二步(第二行)，使用 cat 連接指令合併 b.txt 和 c.txt 並且輸出到 a.txt。

其實這就是本文的主角，Makefile。像上面告訴 make 指令要怎麼做的規則都寫在一個叫做 Makefile 的文件中。而我們可以指定當前的 make 指令要依照哪一個 Makefile 文件的規定去 make 東西，像下面這樣:
```makefile
$ make -f rules.txt
// 或是
$ make --file=rules.txt
```
上面的代碼就是說，指定 make 依照 rules.txt 中的規則去做東西。

這邊先簡單總結下，其實 make 就是一個根據 Makefile 規則指示構建東西的工具，Makefile 中的規則也很簡單，說明要做的東西依賴甚麼東西，要怎麼做。

# Makefile 怎麼寫 ?

## 文件格式
Makefile 文件其實就是一些規則的集合，而每條規則型式如下:
```makefile
<target> : <prerequisites>
[tab] <command>
```
target 放的叫做"目標"，prerequisites 放的叫做前置條件，第二行必須由一個 `tab` 開頭，這是規定，然後後面就跟著 command，也就是要執行的指令。

其中，除了 target 為必需的，其他都是可選的，而 prerequisites 和 command 則必須至少存在一個。

## target
一個目標就構成一個規則。通常，目標可以是文件名，像是上面的 a.txt。除了文件名，還可以是某個操作的名字，這叫做偽目標 (phony target)。看看下面的例子:
```makefile
clean:
    rm *.o temp
```
像上面這樣，clean 就是一個偽目標，作用是刪除所有後綴為 .o 的文件。
```makefile
$ make clean
```

但是，萬一我們的目錄下面已經有了一個叫 clean 的文件，那 make 就會認為已經存在 clean 這個目標，所以並不會去執行真正的 clean 規則，當然這不是我們所希望的。解決辦法就是可以顯示的聲明 clean 是一個偽目標，相當於告訴 make，當我執行 `make clean` 時，不用去檢查是否已經存在 clean 文件。寫法如下:
```makefile
.PHONY: clean
clean:
    rm *.o temp
```
這邊補充下，如果我們的命令這麼寫:
```makefile
$ make
```
那 make 就會默認去執行 Makefile 文件中的第一個目標
。

## prerequisites
前置條件通常是一些文件名，中間用空格分開，指定了"目標"是否重新構建的標準。只要有一個前置文件不存在或是被更新過，就會需要重新構建該"目標。來看看這個例子:
```makefile
result.txt: source.txt
    cp source.txt result.txt
```
上面的 Makefile 中，構建目標 result.txt 的前置條件就是要先有個 source.txt，如果有，`make result.txt` 就會將 source.txt 的內容 copy 到 result.txt 當中。如果沒有，那就要再寫一個生成 source.txt 的規則，還生成目標 source.txt，如下:
```makefile
source.txt:
    echo "This is source.txt" > source.txt
```
可以看到，source.txt 目標不依賴任何前置條件，只要 source.txt 不存在，每次執行 `make source.txt` 都會如期生成 source.txt，文件內容為  "This is source.txt"。

有了上述的兩個目標 result.txt 和 source.txt 後，可以來看看下面這個例子，更清楚一點:
```makefile
make result.txt
make result.txt
```
連續執行了兩次 `make result.txt`，第一次因為還沒有 source.txt，所以會先新建一個 source.txt，然後再新建一個 result.txt。第二次的 `make result.txt` 因為發現 source.txt 沒有變動，所以不會執行不會執行任何操作，當然，result.txt 也不會再生成一次。

這邊補充一個用法，如果要生成多個文件，往往採用下面的寫法:
```makefile
generate: file1 file2 file3
```
上述的 `generate`是一個偽目標，只有三個前置文件，然後如下指令:
```makefile
$ make generate
```
就會做出三個文件，file1, file2, file3。

## command
命令就是要表示要對文件做甚麼操作，由一行或多行 shell 命令組成。command 是構建目標的具體指令，通常，運行結果就是生成目標文件，或是執行完偽目標。

# 實際案例
有了基本的概念，那麼就來看看一個實際的案例吧。

這是某次作業的要求，在 c++ 文件中所有的打印部分都要由 nasm 彙編語言來實現，並且用 Makefile 來啟動整個項目。當然我們首先就需要兩個文件，main.cpp 和 my_print.asm。那麼就來看看 Makefile 文件怎麼寫:
```c++
// main.cpp

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

extern "C" {
    void nasm_print(const char *, const int);
}

void myPrint(const char * p);

int main() {
    const char* s = "Hello World!";
    myPrint(s);
    return 0;
}

void myPrint(const char * p) {
    nasm_print(p, strlen(p));
}
```

```nasm
; my_print.asm

SECTION .text
global	nasm_print
nasm_print:
        push    ebp
        mov     ebp, esp
        mov     edx, [ebp+12]
        mov     ecx, [ebp+8]
        mov     ebx, 1
        mov     eax, 4
        int     80h
        pop     ebp
        ret
```

```makefile
# Makefile

init: main.cpp my_print.asm
    nasm -f elf32 my_print.asm # 用 nasm 編譯 my_print.asm
    g++ -m32 main.cpp my_print.o -o main # 用 g++ 將 cpp 文件和 asm 文件 link
    rm -rf my_print.o
clean:
    rm -rf main
```
這邊用到的案例是將 cpp 和 asm 進行聯合彙編，若有不清楚，可以去參考 [C++和NASM联合编译](https://blog.csdn.net/HermitSun/article/details/102905085)。

有了上面的 Makefile，就可以在命令行中直接執行以下命令:
```makefile
$ make init
$ make clean
```
相信效果一目瞭然的 ! 這篇關於 Make 的分享就到這邊結束。
