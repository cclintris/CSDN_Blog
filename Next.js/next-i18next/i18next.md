@[TOC](前端 Next.js 國際化應用)

# 前言

國際化對於很多場景來說都是挺常見的，舉凡稍微有點知名度的網站，其實全部都提供了國際化的功能。最近因為需要開始學習使用，本文檔就來總結一下在 Next.js 中實現國際化的過程以及涉及到的技術棧。

# 正文

## 準備工作

Next.js 中實踐國際化，首先安裝一個庫：

```js
npm install next-i18next

// or

yarn add next-i18next
```

下面簡單實現，詞條中英文的轉換。

## next-i18next.config.js

使用 next-i18next 我們要先在根目錄下新建 `next-i18next.config.js` 配置文件，以我的項目中配置如下：

```js
module.exports = {
  i18n: {
    defaultLocale: "ch",
    locales: ["ch", "en"],
    localeDetection: false,
  },
};
```

- `defaultLocale` 是你希望默認的語言包

- `locales` 是所有要使用到的語言包

- `localeDetection` 注意一定要加上。這個配置參數表示使瀏覽器不要檢測瀏覽器默認的語言。如果不加上該參數，國際化仍然會生效，但是 `defaultLocale` 則不會生效，因為瀏覽器會自動檢測到英文作為默認語言，進而產生一些其他的問題。

注意這個配置文件所配置的語言包名稱要與下面實際的包名稱相同，可以參考大部分對於語言命名的規範。

## next.config.js

再來我們還要修改一個配置文件。項目創建之初就會有一個 `next.config.js`，關於國際化的配置如下，當然視自己需求自行修改：

```js
const { i18n } = require("./next-i18next.config");

module.exports = {
  i18n,
};
```

## public/locales

我們新建一個 `locales` 文件夾於 `/public` 目錄下，把需要進行轉換的語言包分成兩個文件夾，例如 `en`( 英), `ch`(中)，並且在各自語言目錄下，以 json 文件形式編寫各語言的詞條。

- 文件目錄

![](https://ftp.bmp.ovh/imgs/2021/07/d6b0707531967b2f.png)

舉例來說，同樣的詞條的中英文如下：

```js
// /public/locales/ch/common.js
{
    "Welcome": "欢迎回来",
    "ViewBlog": "查看我的博客",
    "WriteBlog": "撰写新博客"
}

// /public/locales/en/common.js
{
    "Welcome": "Welcome back",
    "ViewBlog": "See my Blogs",
    "WriteBlog": "Write New Blog"
}
```

其他都視自己項目的需求自行添加詞條和語言包。

## serverSideTranslations

使國際化真正生效，首先我們必須要 `pages` 級別的  頁面下使用 `serverSideTranslations` 這個函數。舉例來說，假設我想要在一個 `/login` 頁面使用上面的 `user` 語言包，代碼如下：

```js
// /pages/login.js

function Login() {}

export const getStaticProps = async ({ locale }) => ({
  props: {
    ...(await serverSideTranslations(locale, ["user"])),
  },
});

export default Login;
```

 注意，該  函數必須且只能在 `pages` 頁面層級之下才會生效，用意就是相當於  為該頁面以及該頁面所用到的組件所需要的語言包註冊語言包。

## useTranslation

最後通過一個 `useTranslation` 函數就可以實現國際化，根據一些操作來改變語種，並觸發詞條轉換。

```js
// /pages/login.js
import { useTranslation } from "react-i18next";

function Login() {
  const { t } = useTranslation("user");
  return (
    <>
      <p>{t("具體詞條名稱")}</p>
    </>
    // ...
  );
}

export const getStaticProps = async ({ locale }) => ({
  props: {
    ...(await serverSideTranslations(locale, ["user"])),
  },
});

export default Login;
```

使用方式如上，應該很清楚～最後給出關於一個修改  語種使國際化生效的範例代碼：

```js
import Link from "next/link";
import { router } from "next/router";
import { useTranslation } from "react-i18next";

function Component() {
  const router = useRouter();
  const { t } = useTranslation("button");
  return (
    <>
      <Link href={router.asPath} locale={(router.locale = "en")}>
        <a>{t("Eng")}</a>
      </Link>
      <Link href={router.asPath} locale={(router.locale = "ch")}>
        <a>{t("Chi")}</a>
      </Link>
    </>
  );
}
```
通過配合 Next.js 提供的 `router`，我們可以方便的檢測當前 next-i18next 所適配的語種，並進而將 `t` 函數中的詞條對應起來。

# 結語

該文檔簡單的總結了我自己在項目中實踐國際化的一些過程和步驟。其實這樣只是最基礎的應用，像團隊內的國際站官網前端項目，經過研究後我就發現其實也是基於這樣的國際化庫(不過不是該庫，是 React 專用的，本文介紹的是 Next 專用的)，但是將這種國際化的代碼封裝為一個 `Trans` 組件，這樣就不用一直 `t`，代碼重複度過高。

