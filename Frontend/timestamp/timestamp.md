@[TOC](前端開發 - 淺談時間戳)

# 前言
時間戳這個東西，其實一直以來我都知道有這個東西大概能幹嘛，反正就是跟時間有關的東西啦，但一直都沒有去深入的系統學習，因為實際在開發時，有關時間的一切在項目中都沒有想太多，也沒有做過多的思考和處理。直到最近在實習時，因應需求加上 mentor 的提點，才明白到實際開發中時間處理中時間戳的重要性，所以趕緊來寫寫這邊，好好的學明白時間戳的相關知識點。

# 正文

作為一個前端，有可能你會碰到在頁面上要顯示各種時間，例如對於一個博客網站，要顯示發布時間，對於一個餐廳訂位系統，可能要顯示訂位時間，複雜一點的話，比如對於像是淘寶這類的拍賣網站，或許要對不同訂單顯示各種時間，反正身為一個前端，不管做什麼，都可能常常碰到要處理『時間』這個東西。

而且這些都還只是顯示時間而已，還有一部份是要考慮與後端的交互，或甚至更複雜一點，對於國際化業務，多時區是不避免的問題。

總而言之，會發現其實處理好時間並不是一件簡單的事情，所以通過這篇分享一下對於時間戳這個東西一點小小的心得。

## 先從 timestamp 開始談起

要談時間，最好直接從時間戳(timestamp)談起，或是更精確一點說，是 Unix 時間戳。

什麼是時間戳？我們在瀏覽器控制台，或是隨便 IDE 輸入下面這行，打印出來的東西就是所謂的時間戳：

```js
console.log(new Date().getTime());

// 1630569486511
```
這一串看似是亂碼的東西其實就是所謂的時間戳，是一個 number 類型。時間戳的定義其實指的是：

> 從 UTC+0 時區的 1970 年 1 月 1 號 0 時 0 分 0 秒開始，總共過了多少毫秒

參考 ECMAScript 對時間戳的定義是這樣寫的：

> 20.4.1.1 Time Values and Time Range

> Time measurement in ECMAScript is analogous to time measurement in POSIX, in particular sharing definition in terms of the proleptic Gregorian calendar, an epoch of midnight at the beginning of 01 January, 1970 UTC, and an accounting of every day as comprising exactly 86,400 seconds (each of which is 1000 milliseconds long).

在 Unix 系統中的時間就是這樣表示的，而許多語言得到的 timestamp 也都是類似的表示方法，但有些可能只能精確到「秒」，有些可以精確到「毫秒」。如果你發現程序中有些地方需要除以 1000 或是乘以 1000，就很有可能是在做秒跟毫秒等單位之間的轉換。

上面我們有提到「UTC +0」這東西，這其實就是 +0 時區的意思。

舉例來說，北京標準時區是 +8，或如果要講得更標準一點，就是 GMT +8 或是 UTC +8，這兩者的區別可以參考 [到底是 GMT+8 還是 UTC+8 ?](https://pansci.asia/archives/84978)，現在的標準基本上都是 UTC 了，所以這篇文章接下來都只會用 UTC。

## 儲存時間的標準格式

有了一些基本概念之後，可以來談該如何儲存時間。其中一種儲存方式就是存上面所說的 timestamp，但缺點顯而易見，就是無法用肉眼直接看出時間是什麼，一定要經過轉換。

而另外一種儲存時間的標準叫做 ISO 8601，這個標準被應用於許多地方。

直接看 RFC 3999 對 ISO 8601 時間標準的說法如下：

> This document defines a date and time format for use in Internet protocols that is a profile of the ISO 8601 standard for representation of dates and times using the Gregorian calendar.

那這到底是個什麼樣的格式呢？其實就是像 `2020-12-26T12:38:00Z` 這種格式，用字符串表現一個帶有時區的時間。

更詳細的規則可以參考下圖：

![](https://s3.bmp.ovh/imgs/2021/09/ab588732d7cf72d3.png)

RFC 的規則會定義的比較完整，但總而言之就是如上面說的那種形式。然後最後面如果是 `Z` 就代表 `UTC +0` 時區，如果要其他時區可以這樣寫：`2020-12-26T12:38:00+08:00`，代表 +8 時區的 12 月 26 號 12 點 38 分 0 秒。

在 JavaScript 裡面則是基於 ISO 8601 的延伸格式，在 ECMAScript spec 中的 Date Time String Format 有提到：

![](https://s3.bmp.ovh/imgs/2021/09/d16730be5947b1a2.png)

理解了表示時間的標準格式以後，有個重要的觀念，在處理時間的時候要時刻提醒自己，就是時間的`相對性`。

舉例來說，`1593163158` 這個 timestamp 代表的是：
「UTC +0 時區的 2020-06-26 09:19:00」，同時也代表著
「UTC +8 時區的 2020-06-26 17:19:00」，這兩個時間雖然看起來不一樣，但是因為時區的不同，所以共用同一個時間戳。

所以當你拿到一個 timestamp 以後，你無法從 timestamp 本身知道你應該要顯示成什麼時區的時間，這個在涉及到時區問題時是很重要的。

了解完了這些概念之後，我們來看看 JS 中怎麼處理這些時間。

## JavaScript 中的時間處理

在 JS 裡面可以用 `Date` 來處理時間相關的需求，例如說 `new Date()` 可以產生出現在的時間，然後 `new Date().toISOString()` 就可以產生 ISO 8601 格式的日期字符串，像是：2020-12-26T04:52:26.255Z。

在 new Date() 裡面放上參數的話則是會幫你 parse 時間，例如說 `new Date(1593163158000)` 或是 `new Date('2020-12-26T04:52:26.255Z')`，那麼該 Date 對象的所有屬性都會以該時間戳為主。

除此之外還有許多方法可以拿到時間的各個部分，以上面那個字符串 2020-12-26T04:52:26.255Z 為例，我們用 `new Date('2020-12-26T04:52:26.255Z')` 搭配底下的各個 函數：

|函數|結果|
|:-----|:-----:|
|getYear|120|
|getMonth|11|
|getDate|26|
|getHours|12|
|getMinutes|52|
|getSeconds|26|
|getMilliseconds|255|

有些部分看起來完全沒問題，所以下面就挑一些看起來比較怪的進行講解。

### getYear

你可能預期會拿到 2020 但卻拿到了 120，因為 `getYear` 會是 `年份 - 1900` 之後的結果，如果你想拿到 2020 要用 `getFullYear`。

### getMonth

你會預期拿到 12，但卻拿到了 11，這是因為這邊拿到的數字會從 0 開始，所以如果是 1 月會拿到 0，因此 12 月拿到了 11。

### getHours

傳進去的時間是 4，所以你預期會拿到 4，但卻拿到了 12。這是因為 JS 在進行這些操作之前，都有一個步驟是把時間轉成「Local Time」:

![](https://s3.bmp.ovh/imgs/2021/09/e9ec5a20b37bac28.png)

因為 UTC +0 是 4 點，但因為我所在區域是 +8 時區，所以轉成 UTC +8 就變成 12 點了，因此拿到的就會是 12。

## 日期時間需注意的地方

用 `new Date(string)` 其實就等於 `Date.parse(string)`，可以讓 JS 來幫你解析一個字串並轉換成時間。如果你給的字串符合標準格式的話那沒有問題，但如果不符合標準的話，就會根據實作的不同而有不同的結果：

![](https://s3.bmp.ovh/imgs/2021/09/63ae19c5acde9013.png)

這就是需要小心的地方了，比如說這兩個字串：

```js
new Date('2020-02-10')
new Date('2020/02/10')
```
對我們來說看起來都像是 2020 年 2 月 10 號，但我們會發現 JS 卻 parse 出意想不到的結果：

![](https://s3.bmp.ovh/imgs/2021/09/8c6a88230a8a397d.png)

前者是符合 ISO 8601 格式的，所以被解析成為 UTC +0 的 2 月 10 號 0 點 0 分，所以我們看到的結果才會是 +8 時區的 8 點。

而後者並不符合 ISO 8601 格式，所以會根據實作不同而產生不同的結果。

還有另外一個常見的非標準格式是這樣：2020-02-02 13:00:00。

這個格式少了中間分隔日期和時間的 `T`，在 Safari 上面會直接回給你一個 Invalid Date，而在 Chrome 上面則可以正常解析。這我其實覺得滿合理的，你丟一個非標準格式的東西，本來就應該是一個 invalid。瀏覽器可以正常解析是額外幫你多做事，但不能正常解析你也不能怪它，或應該說才是正常的表現～(不過前端開發應該也沒什麼人在用 safari 吧 hhh)

- Chrome

![](https://s3.bmp.ovh/imgs/2021/09/1d5cd544335d132a.png)

- Safari

![](https://s3.bmp.ovh/imgs/2021/09/69b6cf88e45cdf78.png)

## ISO 8601 vs RFC3339

前面有提到 ISO 8601 跟 RFC3339，這兩個其實有一點細微的差異。

直接來看看 ISO 8601 官方的解釋：

> The character [T] shall be used as time designator to indicate the start of the representation of the time of day component in these expressions.

> NOTE By mutual agreement of the partners in information interchange, the character [T] may be omitted in applications where there is no risk of confusing a date and time of day representation with others defined in this International Standard.

也就是說在 ISO 8601 的標準裡面，`T` 這個字元在溝通的雙方都同意之下是可以省略的，會變成像是：2020-02-0213:00:00 這樣，不過默認是有的。

那再來看看 RFC 3339 官方的解釋：

> NOTE: ISO 8601 defines date and time separated by "T". Applications using this syntax may choose, for the sake of readability, to specify a full-date and full-time separated by (say) a space character.

所以 RFC 3339 為了可讀性，是可以用 `空白` 取代 `T` 的。因此用空白來分割的字串的時間格式，是遵守 RFC 3339 但不遵守 ISO 8601。

那回到我們最關心的 ECMAScript，ECMAScript 是遵守哪一種標準呢？根據 spec 上的說明，看起來 `T` 是必須要有的，所以 在 ECMAScript 裡面一個正確的 date time 需要用 `T` 來分隔，不能用空白取代。

但有趣的事情來了，那就是在 ES5 之前，其實 ECMAScript 的規範裡對於 date time 的格式是沒有說明的，也就是說並沒有強制規定什麼才是標準的格式。所以少了一個 `T` 也可以解析，可以當作是為了支援 ES5 以前而保留的行為。

再回到上一小節的那個 safari 跟 chrome 比較的例子，總之呢，加上 `T` 之後就沒問題了，加上去之後會變成少了時區的 date time：2020-02-02T13:00:00，來看看效果：

- chrome

![](https://s3.bmp.ovh/imgs/2021/09/a6f39e123f8b0645.png)

- safari

![](https://s3.bmp.ovh/imgs/2021/09/4775945d42902a40.png)

這邊看起來都是很正常的沒什麼問題，不過其實其中隱藏一個小小的注意點。

根據 spec 節錄，如果缺少了時區而且符合 date time format 的話，應該要當作是 local time 處理，也就是說你如果不戴上時區信息，應該就在默認當作是你所在時區的時間。Chrome 一直都是沒問題的，不過在以前的 safari，如上的字符串會被解析成下面這樣：

```js
Sun Feb 02 2020 21:00:00 GMT+0800
```
原因就是因為 Safari 把這個時間當成是 UTC +0 的時間，所以差了八個小時。

而經過優化之後，注意到現在的 Safari 解析出來的確實就是正確的 local time，且後面也多了一個提示 `(CST)`，其實表示的就是 China Standard Time。

## 時區的顯示

前面講了這麼多，終於可以來談開頭講的時區的問題了。在處理時間這一塊，應該都是挑一個順眼的庫來用，例如說 moment、date-fns、day.js 或是 luxon 之類的，這些庫如果沒有正確使用的話，其實會跟你想像的結果不同。

例如下面這段代碼，會輸出什麼呢？

```js
luxon.DateTime
  .fromISO('2020-02-02T13:00:00+03:00')
  .toFormat('HH:mm:ss')
```
有許多人都會誤以為如果你的 date time 有帶時區的話，format 出來的結果就會依照那個時區。但不是這樣的，最後 format 還是會以 local time 為主。

所以上面的例子中，由於我的電腦是北京 +8 時區，所以結果會是 18:00:00 而不是 13:00:00。

這點一定要記住，無論是 day.js 或是 moment 也都一樣，如果沒有在 format 之前特別指定時區，format 出來的結果都會依照使用者當前的時區。所以同一段代碼，在不同使用者的電腦可能會有不同的輸出。

因此其實後端給你什麼都不重要，給你 2020-02-02T13:00:00+03:00 或是 2020-02-02T10:00:00Z 或 2020-02-02T18:00:00+08:00，對前端來說都是一樣的，都代表著同一個時間，用 format 也都會產生出一樣的結果。

那麼如果我們的需求是必須要根據 date time 中的時區進行顯示，通常有兩種解決辦法。第一種就是讓後端控制好時區的選擇，通常會通過一個中端服務比如 Express, KOA 處理後傳給前端，以 luxon 為例大概長這樣：

```js
luxon.DateTime
  .fromISO('2020-02-02T13:00:00+03:00', { 
    setZone: true
  })
  .toFormat('HH:mm:ss')
```
但是大部分情形下建議的做法都是由前端自行決定要顯示成哪個時區的時間，而不是由後端給的 date time 來決定。

所以另一種辦法就是讓前端自己進行時區上的控制。以 luxon 來說會是這樣：

```js
luxon.DateTime
  .fromISO('2020-02-02T13:00:00+03:00')
  .setZone('Asia/Tokyo')
  .toFormat('HH:mm:ss')
```
moment 則是這樣：
```js
moment('2020-02-02T13:00:00+03:00')
  .tz('Asia/Tokyo')
  .format('HH:mm:ss')
```
day.js 也類似：
```js
dayjs('2020-02-02T13:00:00+03:00')
  .tz('Asia/Tokyo')
  .format('HH:mm:ss')
```
透過這樣的方式，我們就可以保證輸出的時間一定是固定在同一個時區。

什麼時候會需要這樣做呢？假設我是一個位於深圳的前端工程師，對於一家在深圳的餐廳的訂位系統，後端會傳給我餐廳可以訂位的時段，例如說下午一點，下午兩點之類的，這邊後端會用標準格式給我們，例如說：'2020-02-02T13:00:00+08:00'，代表 2020 年 2 月 2 號的下午 1 點可以訂位。

在前端顯示的時候，如果只是用 moment('2020-02-02T13:00:00+08:00').format('HH:mm') 的話，在我的電腦上看會是正確的，結果會是 13:00。然後這其實也是 bug 的開端，以為自己看是正確的就覺得是正確的。

若是換了一個時區，假設換到日本好了，那同一段代碼所產生出的結果就會變成 12:00(日本時區是 +9)，就是預期外的結果了。因為要訂的是深圳的餐廳，所以訂位時間應該都要永遠都要顯示深圳時間才對，而不是使用者所在時區的時間。

這時候就要按照上面所說的，用：

```js
moment('2020-02-02T13:00:00+03:00')
  .tz('Asia/Shenzhen')
  .format('HH:mm:ss')
```
就能夠保證對於無論在任何地方的使用者，看到的都是用深圳時區顯示的結果。

## 把時間傳到後端去

前面講的是後端給你一個時間然後你要正確顯示出來，解決辦法就是上面所說的，加上正確的一些方法，才能確保是以固定的時區顯示時間。

還有另外一種需要注意的則是相反過來的情況，那就是前端要產生一個 date time 送到後端去。

舉例來說，延續剛剛餐廳訂位網站的例子好了，假設今天有一個聯絡客服的頁面要填想要預訂餐廳的日期，格式是：2020-12-26 這樣子，但你送到後端去的要求要是 date time，所以你要把它變成 ISO 8601 的標準格式。

有些人會想說，這不就很簡單嗎？原生的方法就是 `new Date('2020-12-26').toISOString()`，如果用其他庫可能就是 `moment('2020-12-26').format()`。

但這其實是一個經典的錯誤。

假設去的餐廳是在台灣的餐廳，那這個 2020-12-26 就應該是台灣時間，正確的輸出應該要是：2020-12-26T00:00:00+08:00 或是 2020-12-25T16:00:00Z 之類的，簡單來說就是台灣時間的 12 月 26 號 0 點 0 分。

而上面的經典錯誤寫法，有可能產生的是「UTC +0 時區的 0 點 0 分」或者是「使用者電腦時區的 0 點 0 分」，這時候產生出來的 date time 就會是錯誤的，就有了時差。

解決辦法也跟剛剛差不多，你需要使用時區相關的方法，像是這樣：

```js
// moment
moment.tz('2020-12-26', 'Asia/Taipei').format()

// day.js
dayjs.tz('2020-12-26', 'Asia/Taipei').format()
```
才能正確告訴庫說：「我的這個日期是在台北的日期，而不是在 UTC 也不是在使用者時區」。也就是說，經過這樣處理之後的時間戳，肉眼直接看上去其實不一定會像是 2020-12-26，但經過轉換之後就會是台灣時間的 12 月 26 號 0 點 0 分。

這個視角問題可能會較為複雜一點，可以想像是說，假設對於一個在德國的前端開發，在處理頁面上 2020-12-26 的時候，他自己要知道說通過正確寫法生成的 date time 其實看起來不像是 12 月 26 號 0 點 0 分，但是其實這樣才是對的，因為這樣發給後端，到時候後端再傳回來給前端進行轉換，才會顯示 2020-12-26。

# 結語

在處理時間的時候，最常碰到的就是多一天或是少一天的問題，明明就應該顯示 12/26，怎麼使用者看到的是 12/25？而會有這些問題，往往都跟時區有關，沒有正確處理好時區的話就會產生這些問題。

在處理時區上面只要能謹記幾個原則，就可以避免掉這些基本的問題：

1. 前後端都用標準格式的字串溝通
2. 由前端來決定用什麼時區顯示
3. 前端產生 date time 時記得想一下要不要指定時區

其實一直以為處理時間也就那樣應該沒什麼複雜的，不過真的寫完這篇之後才意識到對於前端，處理時間跟時區還真不是一個想像中那麼 simple 的東西，其實有很多複雜且瑣碎的點要注意。通過這篇學習也只能說是稍微了解了一點，實際上以後碰到具體需求時還要再好好著墨關於時間戳的東西。

# 參考

|參考|鏈接|
|:-----|:-----|
|In an ISO 8601 date, is the T character mandatory?|https://stackoverflow.com/questions/9531524/in-an-iso-8601-date-is-the-t-character-mandatory|
|到底是 GMT+8 還是 UTC+8 ?|https://pansci.asia/archives/84978|

