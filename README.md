# Personal Assistant

> 把待办、笔记、常用文件收进一个桌面窗口的集中管理环境。



---

## 为什么做这个

该项目初衷旨在将学习到的QT知识整合，做出一个相对完整的项目，目前仍有许多不足之处

---

## 功能

- **待办卡片** — 待办项目的增删改查
- **笔记卡片** — 笔记项目的分类及增删改查
- **文件管理** — 通过按钮/拖拽添加文件、打开文件/文件位置、删除、文件删除/分类/搜索
- **主题** — 深色 / 浅色，QSS 全模块皮肤
- **状态记忆** — 关闭时保存当前页面与主题，下次启动恢复
-**日志管理**（未完成）—记录程序运行时各模块状态
---
## 运行截图
### 待办：
<img width="1041" height="697" alt="QQ_1792653262707" src="https://github.com/user-attachments/assets/1a560c01-1ad3-4238-9ac6-7f1f412e5b52" />

### 文件：
<img width="1041" height="697" alt="QQ_1792653336755" src="https://github.com/user-attachments/assets/1427b7e4-b201-4fcd-8fe0-edf4fc434ce1" />

### 设置页：
<img width="1041" height="697" alt="QQ_1792653367155" src="https://github.com/user-attachments/assets/ea0e9b04-0120-47a6-b890-66ed6c70b5ae" />
---

## 技术栈

| 项 | 选择 |
|---|---|
| UI 框架 | Qt 6.9.3（Widgets） |
| 数据库 | SQLite（Qt Sql） |
| 构建 | CMake + Ninja |
| 编译器 | MSVC 2022 (x64) |
| 样式 | QSS，按 `darkMode` / `lightMode` 分目录 |

---

## 目录结构

```
PA/
├── Dao/          数据访问层，一张表一个 Dao
├── manager/      业务层：todo / note / file / theme
├── coreCard/     卡片基类 BaseKard
├── todo/         待办卡片
├── note/         笔记卡片
├── file/         文件卡片 + 表格模型
├── ui/           主窗口
├── utils/        数据库连接、日志
├── res/          图标资源
├── style/        QSS 皮肤
└── main.cpp
```

---

## 构建与运行

### 前置

- Qt 6.9.3 (msvc2022_64)
- Visual Studio 2022（MSVC 工具链）
- CMake ≥ 3.16

### 构建

```powershell
$env:QTDIR = "D:\Qt\6.9.3\msvc2022_64"      # 改成你的路径

cmake -S . -B out/build/debug -G Ninja
cmake --build out/build/debug
```

### 运行

```powershell
D:\Qt\6.9.3\msvc2022_64\bin\windeployqt.exe --debug out\build\debug\personalassistant.exe
.\out\build\debug\personalassistant.exe
```

> 开发期也可以直接把 `%QTDIR%\bin` 加进 `PATH`，省掉部署这步。

---

## 设计说明

- 三层结构：Dao 层负责 SQL → manager层负责业务 → ui 层负责展示
- 卡片为自定义 QWidget，统一继承 `BaseKard`
- 文件页用 `QStandardItemModel` + `QSortFilterProxyModel`做表格展示和分类
- 主题靠 QSS 分目录切换，窗口状态用 `QSettings` 持久化

---

## 已知问题 / TODO

- 删除卡片后不会自动选中下一张
- `cur_Selected_Kard` 用裸指针记录选中项，卡片销毁后易悬空
- `manager` 同时持有数据和卡片列表，职责偏重
- 日志为固定文件追加，无级别过滤与滚动

---
## 可能的更新
- 自定义的笔记/文件分类，新增笔记/文件时用户自主选择分类
-  卡片多余文本省略显示
- 文本块功能
- 真正的待办事项：支持进行中/已完成/已过期分类
- 跨平台支持
---
## License

MIT