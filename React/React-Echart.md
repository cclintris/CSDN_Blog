@[TOC](Echarts-for-React 雷達圖)

# 前言
最近正著手一個課上的大作業，由於其他兩位組員還在和其他項目功能掙扎，於是就由我來負責製作最後項目的網頁展示。關於這個項目的展示，我最後是採用了 React + flask 作為前後端的開發。在另一篇會繼續談談這次學習 flask 下來的一些心得和理解，但在本篇，我想先說說關於 Echarts 這個有趣的組件庫。
之所以學習了 Echarts 也是應組員和項目展示需求，希望可以將數據可視化展現在頁面上，使網頁更活潑並且提升用戶體驗。接下來就進入 Echarts 的世界吧 !

# 正文

## 示例
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/echart-1.png)


## 安裝
要使用 Echarts，當然首先要使用 npm 安裝。
```
npm install --save echarts-for-react
npm install --save echarts
```
安裝完畢後，也可以前往 React 項目中的 package.json 確認是否成功安裝。

## 使用
```js
import ReactEcharts from 'echarts-for-react'

<ReactEcharts 
    option={this.getOption()}
    notMerge={true}
    lazyUpdate={true}
    onEvents={onEvents}
    style={{width: '100%', height: '100%'}}
/>
```

## echarts API

### 設置區域顏色漸進
> echarts.graphic.LinearGradient
```js
{
    value: this.state.overall_student_value,
    itemStyle: {
        normal: {
            color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [
                {
                offset: 0.5,
                color: 'rgba(48, 107, 231, 1)'
                },
                {
                offset: 1,
                color: 'rgba(73, 168, 255, 0.7)'
                }
            ]),
            opacity: 0,
            lineStyle: {
                width: 0,
                color: '#306BE7'
            },
        },
    },
    name: '总体软院学生',
    id: 'overall_student'
}
```

### legend 標籤配置
```js
const radarTextStyle = {
    color: '#333',
    fontStyle: 'normal',
    fontWeight: 'normal',
    fontSize: 12,
}

// 圖例文字展示
legend: {
    data: [
        { name: '总体软院学生'},
        { name: '目标学生'}
    ],
    // 圖例顯示位置在底部
    bottom: 0,
    // 圖例背景顏色
    backgroundColor: 'transparent',
    // 圖例符號大小
    itemWidth: 12,
    itemHeight: 9,
    // 圖例文字樣示
    textStyle: radarTextStyle
}
```

### 雷達圖分類配置
```js
indicator: [
    {"name":"代码估计相似度","value":'',"max":500},
    {"name":"代码时间复杂度","value":'',"max":500},
    {"name":"代码风格水平","value":'',"max":500},
    {"name":"代码空间复杂度","value":'',"max":500},
],
name: {
    textStyle: {
        color: '#999',
        backgroundColor: 'transparent'
    },
    formatter: function(value, indicator) {
        // S{x} 表示類別文字以 x 字為一行
        value = value.replace(/\S{4}/g, function(match) {
            return match + '\n'
        })
        return '{a|'+value+'}' + '\n' + '{b|'+indicator.value+'}'
    },
    // 類別文字樣示
    rich: {
        a: {
            color: '#999',
            fontSize: 12,
            align: 'center'
        },
        b: {
            color: '#333',
            fontSize: 13,
            align: 'center'
        }
    }
}
```
### 點擊事件綁定
```js
// 點擊雷達圖效果
onChartClick(param) {
    console.log(param)
}
// 點擊 legend 標籤效果
onChartLegendselectchanged(param) {
    console.log(param)
}

render () {
    let onEvents = {
        'click': this.onChartClick.bind(this),
        'legendselectchanged': thisonChartLegendselectchanged.bind(this)
    }
    return (
        <div className="echartsRadar">
            <ReactEcharts 
                option={this.getOption()}
                notMerge={true}
                lazyUpdate={true}
                onEvents={onEvents}
                style={{width: '100%', height: '100%'}}
            />
        </div>
    )
}
```
## 完整代碼
本人是將 echarts 做為一個組件，並在其他文件中 import 進去使用。以下是完整的 echarts 組件代碼。
```js
import React, { Component } from 'react'
import echarts from 'echarts/lib/echarts'
import ReactEcharts from 'echarts-for-react'

const radarTextStyle = {
    color: '#333',
    fontStyle: 'normal',
    fontWeight: 'normal',
    fontSize: 12,
};

export class Echart extends Component {
    constructor(props) {
        super(props)
        this.state = {
            overall_student_value: [220, 410, 398, 400],
            specific_student_value: [120, 290, 287, 320],
        }
    }

    getOption() {
        return {
            title: {
                text : ''
            },
            legend: {
                data: [
                    { name: '总体软院学生'},
                    { name: '目标学生'}
                ],
                bottom: 0,
                backgroundColor: 'transparent',
                itemWidth: 12,
                itemHeight: 9,
                textStyle: radarTextStyle
            },
            radar: {
                shape: 'polygon',
                splitNumber: 3,
                center: ['50%', '50%'],
                radius: '65%',
                nameGap: 5,
                triggerEvent: true,
                name: {
                    textStyle: {
                        color: '#999',
                        backgroundColor: 'transparent'
                    },
                    formatter: function(value, indicator) {
                        value = value.replace(/\S{4}/g, function(match) {
                            return match + '\n'
                        })
                        return '{a|'+value+'}' + '\n' + '{b|'+indicator.value+'}'
                    },
                    rich: {
                        a: {
                            color: '#999',
                            fontSize: 12,
                            align: 'center'
                        },
                        b: {
                            color: '#333',
                            fontSize: 13,
                            align: 'center'
                        }
                    }
                },
                axisLine: {
                    lineStyle: {
                        color: '#ddd'
                    }
                },
                indicator: [
                    {"name":"代码估计相似度","value":'',"max":500},
                    {"name":"代码时间复杂度","value":'',"max":500},
                    {"name":"代码风格水平","value":'',"max":500},
                    {"name":"代码空间复杂度","value":'',"max":500},
                ],
                splitArea: {
                    show: false,
                    areaStyle: {
                        color: 'rgba(255,0,0,0)'
                    }
                }
            },
            series: [{
                name: '代码質量',
                type: 'radar',
                areaStyle: {normal: {}},
                data: [
                    {
                        value: this.state.overall_student_value,
                        itemStyle: {
                            normal: {
                                color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [{
                                    offset: 0.5,
                                    color: 'rgba(48, 107, 231, 1)'
                                },{
                                    offset: 1,
                                    color: 'rgba(73, 168, 255, 0.7)'
                                }]),
                                opacity: 0,
                                lineStyle: {
                                    width: 0,
                                    color: '#306BE7'
                                },
                            },
                        },
                        name: '总体软院学生',
                        id: 'overall_student'
                    },
                    {
                        value: this.state.specific_student_value,
                        itemStyle: {
                            normal: {
                                color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [{
                                    offset: 0.5,
                                    color: 'rgba(139, 241, 134, 0.7)'
                                },{
                                    offset: 1,
                                    color: 'rgba(0, 208, 131, 1)'
                                }]),
                                opacity: 0,
                                lineStyle: {
                                    width: 0,
                                    color: '#8BF186'
                                },
                            },
                        },
                        name: '目标学生',
                        id: 'specific_student'
                    }
                ]
            }]
        }
    }

    onChartClick(param) {
        console.log(param)
    }

    onChartLegendselectchanged(param) {
        console.log(param)
    }

    render () {
        let onEvents = {
            'click': this.onChartClick.bind(this),
            'legendselectchanged': this.onChartLegendselectchanged.bind(this)
        }
        return (
            <div className="echartsRadar">
                <ReactEcharts 
                    option={this.getOption()}
                    notMerge={true}
                    lazyUpdate={true}
                    onEvents={onEvents}
                    style={{width: '100%', height: '100%'}}
                />
            </div>
        )
    }
}

export default Echart
```

# 結語
這篇代碼量確實多了點，但是 Echarts 確實是個針對使用 React 如果要顯示雷達圖時，非常適合的組件庫。希望看完這篇對需要的人有所幫助啦 !

