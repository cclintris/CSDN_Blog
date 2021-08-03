@[TOC](Next.js 個人博客項目總結)

# 項目總結

關於 Next.js 博客練手項目開發部分大致上完成，所涉及到的技術棧如下：

1. 前端

- Next.js
- React.js
- Ant design
- Markdown 相關組件庫
- Axios
- cookie

2. 後端

- Node.js
- Express
- JWT

3. 數據庫

- Mysql

關於本項目過程以及學習知識點的總結，分為本文檔以及另外三份文檔，鏈接如下：

- [Next.js 基礎入門總結](https://blog.csdn.net/weixin_46803507/article/details/119188874?spm=1001.2014.3001.5501)

- [Next.js 路由守衛應用](https://blog.csdn.net/weixin_46803507/article/details/119211527?spm=1001.2014.3001.5501)

- [前端 Next.js 國際化應用](https://blog.csdn.net/weixin_46803507/article/details/119330933?spm=1001.2014.3001.5501)

若有興趣，代碼倉庫訪問下面鏈接：

- [next-blogger-web](https://github.com/cclintris/next-blogger-web)

# Next.js + Ant-design 配置

這邊補充下關於在 Next.js 中配置 ant-design 的總結，因為個人認為沒必要單獨拉出來一份文檔。

1. 首先安裝好 antd

```js
npm install antd

// or

yarn add antd
```

2. 於項目根目錄下新建 `.babelrc` 文件：

```json
{
  "presets": [
    "next/babel"
  ],
  "plugins": [
    [
      "import",
      {
        "libraryName": "antd"
      }
    ]
  ]
}
```
3. 全局引入 antd 樣式於 `_app.js`

```js
import Layout from '../components/Layout'
import '../styles/globals.css'

// 引入 antd 樣式
import 'antd/dist/antd.css'
import { appWithTranslation } from 'next-i18next'

function MyApp({ Component, pageProps }) {
  return (
    <>
      <Layout />
      <main className="container">
        <Component {...pageProps} />
      </main>
    </>
  )
}

export default appWithTranslation(MyApp)
```

