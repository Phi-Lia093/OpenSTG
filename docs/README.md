# 東方妖々夢 重制项目

这个项目的行为基线是原版日文 TH07 1.00b。目标不是重新实现一遍，而是**在已有的、
经过逐函数验证的重建源码之上做改动和抽象**。

## 目录

```
src/            游戏源码（重建自原版反汇编）
  legacy/       只有头文件。这些 .hpp 被主源码引用，对应的 .cpp 是同一个
                翻译单元的拆分实现，当前构建不参与编译。
  pbg4/         归档格式与 LZSS 压缩
tools/          构建与代码生成
  build.py      编译 + 链接，产出可直接运行的 build/
  generate_i18n.py   i18n.csv -> i18n.hpp
  i18n.csv      原版字符串表
.tools/         本地工具链与游戏数据（不入库）
build/          构建产物（不入库）
```

## 构建

```
python tools/build.py
```

需要：

- MSVC 32 位编译器（`cl.exe` / `link.exe`）
- Windows SDK（脚本自动探测版本）
- DirectX 8 SDK，解压到 `.tools/dx8sdk`，需要其中的 `include/d3d8.h` 与 `lib/d3d8.lib`
- `.tools/th07.dat` 与 `.tools/thbgm.dat`

不依赖 Wine：源码目标是 Win32，本机有 `Hostx86\x86\cl.exe`，原生编译即可。

`build/` 每次构建前清空，只留运行游戏需要的四个文件：

```
build/th07.exe     产物
build/th07.dat     从 .tools 拷贝，图形 / ECL / msg
build/thbgm.dat    从 .tools 拷贝，音乐
build/th07.cfg     脚本生成，窗口模式
```

直接双击 `build/th07.exe` 即可运行。

中间产物全在 `build/tmp/`，构建结束默认删掉：

```
build/tmp/obj/         .obj
build/tmp/log/         cl.exe 与 link.exe 日志
build/tmp/generated/   i18n.hpp、th07.map
build/tmp/lib/         链接器副产物（.lib / .exp / .pdb）
build/tmp/dat/         dat 拷贝的中转缓存
```

出错时加 `--keep-tmp` 保留下来看日志。

### th07.cfg 的来历

脚本按 `src/Supervisor.hpp` 里的 `GameConfiguration` 逐字段写这个文件，不是塞一段
固定的十六进制。字段偏移来自编译器（`sizeof(GameConfiguration) == 0x38`）：

| 偏移 | 字段 | 值 |
|---|---|---|
| `0x00` | controllerMapping 9×i16 | `0,1,2,4,-1,-1,-1,-1,3` |
| `0x14` | version i32 | `0x70002` |
| `0x18`/`0x1a` | padAxisX/Y i16 | `600` |
| `0x1c` | lifeCount | `2` |
| `0x1d` | bombCount | `3` |
| `0x1e` | colorMode16bit | `0`（32 位） |
| `0x1f` | musicMode | `1`（WAV） |
| `0x20` | playSounds | `1` |
| `0x21` | defaultDifficulty | `1`（Normal） |
| **`0x22`** | **windowed** | **`1`** |
| `0x23` | frameskipConfig | `0` |
| `0x24` | effectQuality | `2` |
| `0x25` | slowMode | `0` |
| `0x26` | shotSlow | `1` |
| `0x27` | unused_27[13] | 全 0 |
| `0x34` | opts u32 | `0` |

`Supervisor::LoadConfig` 会逐项校验范围并要求大小正好 56 字节，任何一项不符就
**静默回退到默认值**，而默认是全屏。所以这个文件必须精确，不能"差不多"。
脚本里有两道断言：`windowed` 必须落在 `0x22`，以及所有字段都必须通过
`LoadConfig` 的范围检查。

除了 `windowed` 和 `colorMode16bit`，其余值都取自 `LoadConfig` 的默认初始化，
等于"原版默认设置，只把全屏换成窗口"。

**`colorMode16bit` 不能写 255。** `LoadConfig` 的默认值是 255，含义是"还没定"：
`main.cpp` 的 `InitD3dDevice` 会按显示模式把它解析成 0（32 位）或 1（16 位）——
但这段解析只在**全屏分支**里跑。窗口模式下分支不执行，255 会一直留着；而
`LoadConfig` 的 `colorMode16bit < 2` 校验在那之前就发生了，于是整个文件被判非法、
丢弃、重新初始化，结果回到全屏。所以这里直接写解析后的值 `0`。

`.tools/th07.dat` 是 PBG4 归档，游戏通过 `Pbg4Archive` 直接读，不需要解包。

## 正确的做法

**从可运行的基线开始改，一次改一处。** 这个仓库的价值在于 `src/` 已经是能编译出
可玩程序的真实实现，不是需要从头搭的架子。任何抽象都应该是在这份代码上抽出来的，
而不是先设计一套框架再把内容填进去。

**验证靠跑，不靠读代码推断。** 之前反复出现「代码看起来对、测试全过、实际画面
完全错」的情况。改动之后必须真的运行并观察结果。

**构建保持可直接调用编译器。** `tools/build.py` 就是编译加链接，没有中间层。
需要更多能力时加一个参数或一个生成步骤，不要引入构建系统。

**原版的数值来自反汇编，不要凭印象改。** 速度、判定、分数公式、精灵索引这些都
有确定来源，写代码时在注释里标出依据。

## 已知问题

### 日文字符渲染乱码

游戏内用 GDI 绘制的文字（菜单描述、设置项）显示为乱码，预渲染的 PNG 艺术字正常。

原因方向：文字通过 `TextHelper::RenderTextToTextureBold` 用 `SHIFTJIS_CHARSET` 的
GDI 字体绘制到表面，但取字模时按单字节处理（`TextOutA` + `strlen`）。日文是双字节，
逐字节输出自然得不到正确字模。原版可能依赖特定的系统代码页或字体，
在非日文 Windows 上就会暴露。

不影响游玩，未修。

### 进入设置菜单闪退

标题界面进入 Options（残机数、色彩模式、BGM、声音、窗口、慢速、重置、键位、退出
这个菜单）后立刻崩溃。**其他部分正常**：可以开始游戏、打到 Boss、符卡正常运行。

已经排除的：

| 检查 | 结果 |
|---|---|
| 配置文件读取 | 正常，`windowed=1` 生效，版本 `0x70002`，大小 56 字节匹配 |
| `vms` 数组 | `vmCount=164`，指针有效，9 个菜单项 sprite 全部有效 |
| 描述文字的 VM | 14 个全部 sprite 有效 |
| 状态分派 | `MENU_STATE_OPTIONS` 确实到达，`this` 指针前后一致 |
| 文字渲染 | 两次 `RenderTextToTextureBold` 全部返回 `hr=0` |
| 缓冲区溢出 | `vsprintf` 换 `vsnprintf` 后仍崩，且日志显示未进入格式化 |

现场是：设置菜单的 9 项描述文字渲染顺利完成（日志显示两遍绘制都成功），但
`DrawStringFormat2` 没有返回。崩溃点在绘制完成到函数返回之间。

下一步可以查的方向：

- 用 WinDbg 附加，直接看崩溃时的调用栈和异常地址，比加日志快
- 检查 `descriptionVms` 在渲染过程中是否被其他代码改写
- 对比原版在相同菜单下的 GDI 状态（字体、DC、表面格式）

## 历史

之前尝试过从零写一个 C++20 + OpenGL + Lua 的引擎框架，把原版行为作为内容填进去。
方向是错的：工程量巨大，而且做出来的东西和原版在观感、手感上差距明显。
那次的代码在 `archive/remake-attempt-main` 分支和 `archive-remake-2026-11-26` 标签里，
只作参考。
