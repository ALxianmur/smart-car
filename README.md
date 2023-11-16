## 说明

这个仓库用于团队 2023 智能车校赛代码仓库,maybe 进明年决赛?
仓库架构说明

- [原仓库代码](https://gitee.com/seekfree/STC32G12K128_Library)
- demo 文件下是独立的工程以及相关说明(目前直接使用老师的代码,后期平地起高楼)

---

![逐飞LOGO](https://images.gitee.com/uploads/images/2019/0924/114256_eaf16bad_1699060.png "逐飞科技logo 中.png")

# 逐飞科技 STC32G 开源库

#### 简介

逐飞科技针对参加各类竞赛以及使用 STC32G12K128 进行产品开发，制作的 STC32G12K128 开源库。

#### 环境准备

1.  **STC32G 硬件环境：**

- 推荐使用本公司 STC32G 核心板，[点击此处购买](https://item.taobao.com/item.htm?spm=a1z10.3-c-s.w4002-22508770840.9.516749ccPBxWt6&id=669462141907)。

2.  **软件开发环境：**
    （MDK FOR C251）

- MDK 推荐使用版本：V5.60。

3.  **下载器：**
    （核心板自带）

- 核心板自带有 CH340E 芯片，只需要插上 TYPE-C 数据线即可实现下载功能。
- USB 转 TTL：推荐使用本公司 USB 转 TTL，[点击此处购买](https://item.taobao.com/item.htm?spm=a1z10.3-c-s.w4002-22508770840.9.12dc49ccL93dhG&id=524829874577)。

3.  **内部晶振频率：**

- board.h 文件中 FOSC 的值设置为 0，则程序自动设置系统频率。
- board.h 文件中 FOSC 的值设置不为 0，则系统频率为 FOSC 的值。在使用 stc-isp 工具下载程序的时候需要将 IRC 频率设置为 FOSC 的值。

#### 使用说明

1.  **下载开源库：** 点击页面右侧的克隆/下载按钮，将工程文件保存到本地。您可以使用 git 克隆（Clone）或下载 ZIP 压缩包的方式来下载。推荐使用 git 将工程目录克隆到本地，这样可以使用 git 随时与我们的开源库保持同步。关于码云与 git 的使用教程可以参考以下链接 [https://gitee.com/help](https://gitee.com/help)。
2.  **打开工程：** 将下载好的工程文件夹打开（若下载的为 ZIP 文件，请先解压压缩包）。在打开工程前，请务必确保您的 IDE 满足环境准备章节的要求。

#### 逐飞科技 STC32G 核心板

![逐飞科技STC32G12K128核心板](https://gd2.alicdn.com/imgextra/i4/2364650632/O1CN01KB6ZVU1GXV3kXoJ6a_!!2364650632.jpg "逐飞科技STC32G12K128核心板.jpg")
