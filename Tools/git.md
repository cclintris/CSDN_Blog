@[TOC](完全理解 Git 到底是什麼)

# 前言
其實這個東西大家一定都知道了，一般使用上也都應該使用過，大一點點的項目都會要求要有個代碼倉庫共同開發，首選就是 git。不過我自己個人用歸用，實際上對於很多指令以及 git 本身的一些概念其實並不是很清楚，除了一些最最最基礎的命令比較熟悉之外，對於有些問題還是都不太知其所以，導致有時都還是有種在解決 git 問題時瞎貓亂碰的感覺。所以這篇要來好好補足一下這個知識點，對於之後實習應該也多少有點幫助的。

# 正文
在日常工作中，經常會用到 Git 操作。但是對於很多人來講，剛上來對Git很陌生，操作起來也很懵逼。本篇文章主要針對剛開始接觸 Git 的新人(像我一樣)，理解 Git 的基本原理，掌握常用的一些命令。

## 關於版本控制
首先就先來說一下什麼事版本控制。簡單來說，版本控制就是一種紀錄若干文件的內容變化，以便將來可以查閱特定版本，回退特定版本，追蹤開發軌跡的一種技術。

## DVCS
DVCS 的全稱是 Distributed Version Control System，也就是分布式版本控制系統。在這類系統中，像是 Git、Mercurial 等等，客戶端不只提取最新版本的文件快照，而是把原始的代碼倉庫完整得整個鏡像下來。這樣好處就是，任遠端服務器故障也沒關係，因為客戶端這邊都是完整的鏡像，可以用任何一個鏡像的本地倉庫進行恢復。
![](https://obohe.com/i/2021/06/05/ftvxrl.png)

## Git 工作流程
![](https://obohe.com/i/2021/06/05/fuj6l1.jpg)

我們都知道使用 git 的時候有很多指令，但是先暫時不關心這些，先來了解塗上的幾個概念。

1. Workspace: 工作區
2. Index/Stage: 暫存區
3. Repository: 倉庫區(本地倉庫)
4. Remote: 遠程倉庫

### 工作區 / Workspace
工作區指的是 pc 終能具體看到的一個管理倉庫的目錄，比如下圖的 test 文件夾就是一個工作區。
![](https://obohe.com/i/2021/06/05/fv33xk.jpg)

### 暫存區 / Index

上面的圖我們看到工作區下面有一個 `.git` 文件，暫存區會紀錄 `git add` 添加文件的相關信息，像是文件名、大小、timestamp 等等，而不會直接保存文件實體，而是通過 `id` 指向每個具體的文件實體。可以使用 `git status` 來查看暫存區的狀態，暫存區會標記你當前的工作區下，哪些內容是被 git 管理的。

當你完成某個需求或是功能然後需要提交到遠程倉庫，那麼第一步就是要先通過 `git add` 先提交到暫存區，使這個欲提交的文件被 git 管理，相當於通過 `git add` 提交到緩存區的東西就會被加入到 git 版本控制當中。

### 本地倉庫 / Repository
本地倉庫保存了對象被提交的各個本版，比起工作區和暫存區的內容來說，它要更舊一些。

`git commit` 後同步 index 的目錄樹到 repository，方便從下一步通過 `git push` 同步本地倉庫和遠程倉庫。

### 遠程倉庫 / Remote
遠程倉庫的內容可能被分佈在多個地點，因此它可能與本地倉庫同步，也可能不同步，但是總而言之它得內容是最舊的。

### 小結

1. 任何對象都是在 workspace 誕生和修改
2. 任何修改都是從進入 index 後才開始被 git 版本控制
3. 只有把修改提交到 repository，該修改才會在倉庫中留下痕跡
4. 通過把這些在 repository 的對象 push 到 remote 可以與協作者分享本地的修改，相當於把 remote 更新至最新，大家看到都是最新的

下面這幅圖更直接的說明了四個區域之間的關係，截至為止提到的命令如果不是很熟悉也沒關係，下面會在介紹到。
![](https://obohe.com/i/2021/06/05/gjmnfx.jpg)

## HEAD
![](https://obohe.com/i/2021/06/05/gk7306.jpg)

在掌握具體命令前，首先我們先來理解一個超重要的概念 `HEAD`。

這裡要從 git 分支說起。 git 中的分支其實本質上只是指向 commit 對象的可變指針，常見的分支像是 master, dev 等等。那 git 是如何知道你當前在哪個分支上工作呢？其實它是通過保存一個名為 HEAD 的特別指針，在 git 中，它是一個指向你正在工作中的本地分支的指針，可以將 HEAD 想像成是當前分支的別名。
![](https://obohe.com/i/2021/06/05/gkpo9u.jpg)

## add
![](https://obohe.com/i/2021/06/05/gl7453.jpg)

`add` 命令很簡單，也很基本，主要實現將 workspace 修改的內容提交到 index，加入 git 版本管理。

- `git add .` : 添加當前目錄下的所有文件到 index

- `git add dir` : 添加指定目錄，包括子目錄下的所有文件到 index

- `git add file` : 添加指定文件到 index

關於 `add` 命令我自己覺得有三種比較常見的用法，下面就整理一下他們之間的區別：

|add 命令|描述|
|:-----|:-----|
|git add -A|把所有變化提交到 index，包含當前 git 倉庫的所有目錄|
|git add -u|提交被修改(modified)和被刪除(deleted)的文件，不包括新文件(new)|
|git add .|這其實是我自己最常用的用法，不過這個命令會因為版本有所區別：<br />-1.x 版本：提交新文件(new)和被修改(modified)的文件，不包括被刪除(deleted)的文件<br />-2.x 版本：和 git add -A 一樣，提交所有變化|

## commit
![](https://obohe.com/i/2021/06/05/gl7453.jpg)

`commit` 主要是將 index 中的改動提交到 repository。每次使用 `git commit` 命令我們都會在本地版本庫生成一個 40 位的哈希值(hashcode)，這個哈希值也就是 commit-id，這個 commit-id 再版本回退特別有用，它相當於一個快照，可以在未來任何時候通過與 `git reset` 的配合使用回退到某版本。關於 `reset` 下面還會提到。

|commit 命令|描述|
|:-----|:-----|
|git commit -m [message]|提交 index 到 repository，message 是說明信息|
|git commit [file] -m [message]|提交 index 中的指定文件到 repository|
|git commit amend -m [message]|使用一次新的 commit 替代上一次 commit|

## branch
![](https://obohe.com/i/2021/06/05/glv7hw.jpg)

通常使用到 git 也是因為項目開發涉及到多人協作，那麼涉及到協作自然會涉及到分支，常見的操作包括 `展示分支、切換分支、創建分支、刪除分支` 這四種操作。

|branch 命令|描述|
|:------|:-----|
|git branch|列出所有本地分支|
|git branch -r|列出所有遠程分支|
|git branch -a|列出所有本地分支和所有遠程分支|
|git branch [branch-name]|新建一個分支，但依然停留在當前分支|
|git checkout -b [branch-name]|新建一個分支，並切換到該分支|
|git branch -d [branch-name]|刪除分支|
|git push origin --delete [branch-name]|刪除遠程分支|

## merge
![](https://obohe.com/i/2021/06/05/gm3zcz.jpg)

Git 中的 `git-merge` 是在 git 使用中使用的非常頻繁的一個命令，我自己一開始覺得 git 合併是一個非常麻煩的事情，一個不小心就有能丟失代碼，所以就覺得對 git 望而卻步。

merge 命令把不同分支合併起來到一個上。如上圖，在實際開發中，我們可能從 master 分支上切出一個 feature 分支，然後進行開發完成功能，中間可能經過 R3,R4,R5 的 commit 紀錄，最後完成需求就需要合併進 master 中，這就是要用到 merge。

通常我們會先使用 `git fetch remote merge` 把 remote 的最新代碼拉下來。再來 `git merge branch` 將指定分支合併到當前分支上，這時會出現衝突(conflict)，需要我們去手動解決。原因就是因為在同一個 workspace 對同一文件有兩份不同的修改紀錄，需要你自己去選擇要哪一個不要哪一個。

## rebase
![](https://obohe.com/i/2021/06/05/gpkohj.jpg)

rebase 又稱為衍合，是合並的另外一種選擇。

在開始階段，我們處於 new 分支上，執行 `git rebase dev`，那麽 new 分支上新的 commit 都在 master 分支上重演一遍，最後 checkout 切換回到 new 分支。這一點與 merge 是一樣的，merge 前後所處的分支並沒有改變。對於 `git rebase dev`，通俗的解釋就是 new 分支想站在 dev 的肩膀上繼續下去。rebase 也需要手動解決沖突。

### merge vs rebase
現在我們有這樣的兩個分支,test 和 master，提交如下：
```
    D---E  test
    |
A---B---C---F  master
```
在 master 進行 `git merge test`，會得到如下結果：
```
    D-----------E
    |           |
A---B---C---F---G  test master
```
在 master 進行 `git rebase test`，會得到如下結果：
```
A---B---D---E---C---F  test, master
```
可以看到，merge 操作會生成一個新的節點，之前的提交分開顯示。而rebase操作不會生成新的節點，是將兩個分支融合成一個線性的提交，不過注意，不管是 merge 還是 rebase 完後都仍然在原先的分支上。

如果你想要一個幹凈的，沒有 merge commit ·的線性歷史樹，那麽你應該選擇`git rebase` 如果你想保留完整的歷史記錄，並且想要避免重寫 commit history 的風險，你應該選擇使用 `git merge`。

## reset
![](https://obohe.com/i/2021/06/05/gx7g8x.jpg)

有時候，我們用 Git 的時候有可能 commit 提交代碼後，發現這一次 commit的內容是有錯誤的，那麽有兩種處理方法：

1. 修改錯誤內容，再次commit一次
2. 使用 `git reset` 命令撤銷這一次錯誤的 commit

第一種方法比較直接，但會多一次 commit 記錄。實際上應該第二種方法更好些，錯誤的 commit 確實沒必要保留下來。

reset命令把當前分支指向另一個位置，並且相應的變動工作區和暫存區。

|reset 命令|描述|
|:-----|:-----|
|git reset —soft commit|只改变提交点，暂存区和工作目录的内容都不改变|
|git reset —mixed commit|改变提交点，同时改变暂存区的内容|
|git reset —hard commit|暂存区、工作区的内容都会被修改到与新提交点完全一致的状态|
|git reset --hard HEAD|让工作区回到上次提交时的状态|

## push
`git push` 的一般形式為 `git push <遠程主機名> <本地分支名>  <遠程分支名>`，例如 git push origin master：refs/for/master ，即是將本地的 master 分支推送到遠程主機 origin 上的對應 master 分支， origin 是遠程主機名的默認名。

|push 命令|描述|
|:-----|:-----|
|git push remote branch|上傳本地指定分支到遠程倉庫|
|git push remote --force|強行推送當前分支到遠程倉庫，即使有沖突|
|git push remote --all|推送所有分支到遠程倉庫|

## 其他命令

|其他命令|描述|
|:-----|:-----|
|git status|顯示有變更的文件|
|git log|顯示當前分支的版本歷史|
|git diff|顯示 index 和 workspace 的差異|
|git diff HEAD|顯示 workspace 與當前分支最新 commit 之間的差異|
|git cherry-pick commit|選擇一個commit，merge 進當前分支|


## Git 常用命令
最後整理一些 git 比較常用的命令，也方便以後自己回頭查詢，感謝網上提供的這麼一張詳細整理～

![](https://obohe.com/i/2021/06/05/h649ex.jpg)

# 結語
這篇大致上介紹了一些關於 git 的基本概念上的理解，以及個人的一些感想，整體上應該還算是比較詳細的。之後在使用時可能也不會那麼不明所以了，不過之後還是要更多多使用，這次紀錄的有很多我也不常用或是幾乎沒用過(太菜了)，並在使用前都要先想想我這麼做代表什麼，也許可以更加理解 git～

