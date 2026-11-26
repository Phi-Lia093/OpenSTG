# 重制版架构

本文描述重制版的目标形态。**底线：永远在原版基线上改代码，不重写。**
`src/` 里的 C++ 是起点，每一层抽象都是从现有代码里抽出来的，不是凭空设计的框架。

## 目标

| # | 目标 | 落地方式 |
|---|---|---|
| 1 | 现代渲染、跨平台 | SDL3 + OpenGL 3.3 替换 D3D8 |
| 2 | 摒弃 anm/ecl/std/sht/msg/dat 二进制格式 | 文件夹 + PNG + MID/WAV + 文本配置 |
| 3 | 二进制配置全部文本化 | 原版 config 结构体 → 可读配置文件 |
| 4 | anm/ecl VM 用现代脚本替代 | ANM → 自定义 DSL；ECL → 语义化 Lua |
| 5 | 抽象 STG 引擎层 + 妖妖梦内容包 | 引擎不知道任何 TH07 枚举 |

已定的决策（用户确认）：

- 渲染 **SDL3 + OpenGL 3.3**
- 引擎核心 **C++**；Lua 只承担 ECL
- 第一版范围 **Stage 1 全流程**
- 不追求逐帧一致，**优先架构干净**
- 美术资源 **离线转换一次，产物入库**
- ANM 用**自定义 DSL**，不进 Lua
- ECL 用**语义化 Lua**，不要 `add()`/`call_sub()` 这种把 opcode 换名字的写法

## 为什么 ANM 不用 Lua

实测 63 个归档、9,833 条指令：

```
脚本总数   984
平均长度   10 条指令
中位数     9
最长       25
跳转类     1,245 (12.66%)，其中条件跳转仅 22 (0.22%)
```

指令分布前六类占 58%，全是设值或补间：切帧 (20.7%)、中断标记 (11.8%)、
设透明度 (5.4%)、透明度渐变 (4.8%)、淡入淡出 (4.7%)、停止 (11.0%)。

跳转的用途是**回跳到脚本开头做无限循环**（待机动画、Boss 呼吸），没有嵌套循环、
没有递归、没有计算。984 个脚本最长 25 条指令，平均 10 条——这是时刻表，不是程序。

所以 ANM 是数据，ECL 才是逻辑（30,017 行反汇编，条件分支密集）。

## 分层

```
┌─────────────────────────────────────────────┐
│  content/th07/       妖妖梦内容包            │
│    stage1.dsl  enemies.lua  bullets.toml    │
│    player/  sprites/  audio/  config.toml   │
└──────────────────┬──────────────────────────┘
                   │ 只通过引擎公开接口
┌──────────────────▼──────────────────────────┐
│  engine/            抽象 STG 引擎            │
│    render/ audio/ input/ scene/ vm/         │
│    timeline/ (ANM DSL)  ecl/ (Lua)  data/   │
└─────────────────────────────────────────────┘
```

**引擎不认识 TH07。** 判断标准：`engine/` 里不应该出现 `TH07`、`妖々夢`、
`stg1enm`、`ANM_FILE_PLAYER` 这类名字。出现了就说明抽象漏了。

### engine/ 职责

| 模块 | 职责 | 从哪里抽 |
|---|---|---|
| `render/` | SDL3 窗口 + GL3.3、sprite batch、纹理、混合模式 | `AnmManager` 的绘制部分、`dxutil.hpp` |
| `audio/` | BGM/SE，OGG/WAV | `SoundPlayer`、`MidiOutput`（MIDI 弃用，转 OGG） |
| `input/` | 键盘手柄 → 抽象动作 | `Supervisor::SetupDInput`、`ControllerMapping` |
| `scene/` | 场景栈、状态机 | `Chain`、`Supervisor::OnUpdate` |
| `timeline/` | **ANM DSL 解释器**：帧驱动的属性关键帧 | `AnmManager::RunScript` |
| `ecl/` | **Lua 宿主**：加载、沙箱、绑定 | `EclManager`、`EnemyEclInstr` |
| `data/` | 纹理、DSL、配置的加载与缓存 | `AnmManager::LoadAnms`、`Pbg4Archive` |

### content/th07/ 职责

所有 TH07 特有的东西：角色、符卡、关卡、分数公式、残机数、菜单布局。

## 三个替换

### 1. dat 包 → 文件夹

原版一个 `th07.dat`（23.8 MB，63 个 anm + 8 ecl + 8 std + 12 sht + msg + jpg + mid/wav）。

转换后：

```
content/th07/
  anm/title01.dsl, stg1enm.dsl, ...
  tex/title01_0.png, etama_3.png, ...
  ecl/ecldata1.lua ... ecldata8.lua
  stage/stage1.toml ... stage8.toml      (std)
  bullets/ply00a.toml ...                (sht)
  dialog/msg1.toml ... msg8.toml
  audio/bgm/*.ogg  audio/se/*.wav
  ending/*.png
```

原始 `.dat` 不再需要。仓库自包含。

**纹理**：ANM 里是内嵌 THTX，不是独立 PNG。转换工具拆成 `图标名_序号.png`，
DSL 里引用文件名。

### 2. 二进制配置 → 文本

**不只 `th07.cfg`。** 全部：

| 二进制 | 内容 | 文本形式 |
|---|---|---|
| `th07.cfg` (56 B) | 残机、音量、16/32bit、MIDI/WAV、窗口 | `config.toml` |
| `*.std` | 关卡背景、BGM、敌人波次表 | `stage/N.toml` |
| `*.sht` | 玩家弹幕发射参数 | `bullets/*.toml` |
| `msg*.dat` | 对话 | `dialog/N.toml` |
| `*.anm` | 精灵 + 动画脚本 | `*.dsl` + `.png` |
| `*.ecl` | 敌人逻辑 | `.lua` |

`config.toml` 直接对应原版结构体字段，注释写清每个字段的来历和范围：

```toml
# 对应 GameConfiguration (src/Supervisor.hpp)
# LoadConfig 会校验 life_count < 5，越界整个文件丢弃并回退默认（全屏）
life_count = 2
bomb_count = 3
color_mode_16bit = false     # 原版 255 表示"未定"，全屏时才解析
music_mode = "wav"
windowed = true
```

### 3. VM → DSL / Lua

**ANM → 自定义 DSL。** 一行一条指令，时间作前缀：

```dsl
# stg1enm.dsl — 原 stg1enm.anm
# 一个 anm 文件含多个具名纹理，每个纹理再切成若干精灵矩形
texture "stg1enm_bg" file="stg1enm_bg.png"
  sprite 0  80x32 at (0, 0)
  sprite 1  80x32 at (0, 64)
  sprite 2 160x32 at (80, 0)

script "idle":
  at 0:    sprite 0
           alpha 255
           fade alpha to 0 over 16
           move to (-48, -32) over 60 linear
  at 60:   sprite 1
           fade alpha to 0 over 32
  at 92:   hide
           stop
```

原版 27 个 opcode 变成这些动词：`sprite`、`alpha`、`color`、`scale`、`rotation`、
`move`、`fade`、`flip`、`hide`、`stop`、`wait`、`tween`、`label`、`jump`、`loop`。
条件跳转（0.22%）用 `jump_if` 表达。

**ECL → 语义化 Lua。** 原版 `ins_117(20, 4, -1)` 在 Lua 里应该是：

```lua
-- 原 ecldata1.ecl Sub1_176
while true do
  wait(20)
  spawn_bullet {
    x = self.x, y = self.y,
    speed = 4, angle = aim_at_player()
  }
  local n = 0
  while n < 3 do
    wait(20)
    spawn_bullet { speed = 1, angle = n * 120 }
    n = n + 1
  end
end
```

不是 `ins_3(...)`，不是 `call_sub(...)`，不是 `add(...)`。是 `while`、`if`、
局部变量、表。C++ 侧提供动词绑定：`wait`、`spawn_bullet`、`aim_at_player`、
`set_pos`、`play_sound`、`call_sub`（子脚本确实存在，保留为函数调用是合理的）。

## 迁移顺序

**不要一次性重构。** 每一步都要能跑。

| 步 | 做什么 | 验证 |
|---|---|---|
| 0 | 现状：D3D8 + 二进制 + 原版 VM 能跑 | 已达成 |
| 1 | 抽出 `engine/render`，D3D8 → GL3.3，其他不动 | Stage 1 画面和原版一致 |
| 2 | `engine/data` 支持文件夹资源；写转换工具 | 用转换后的资源跑起来 |
| 3 | ANM VM 换成 DSL 解释器 | 动画表现一致 |
| 4 | ECL VM 换成 Lua 宿主 | Stage 1 敌人行为一致 |
| 5 | 拆 `Player`/`GameManager` 的全局耦合 | 代码里不再出现 `g_*` 单例 |
| 6 | 抽出 `content/th07`，引擎层完成隔离 | 能加第二个内容包 |

第 5 步是最难的。`Player.cpp` 一个文件就有 **120 处**直接访问
`g_GameManager`/`g_AnmManager`/`g_BulletManager`。原版用全局单例把逻辑和数据绑死，
这是"引擎 + 内容包"最大的结构障碍。做法是把这些单例收进一个 `World` 上下文对象，
逐个传递，不改变行为。

## 不做的事

- 不重写游戏逻辑。`Player.cpp`、`GameManager.cpp` 里的数值和流程照搬。
- 不先设计框架再填内容。每层抽象都从跑得起来的代码里长出来。
- 不追求逐帧一致。手感相近即可，replay 格式重新设计。
- 不保留 MIDI。转 OGG，省掉 MIDI 设备依赖。
- 不保留 `resources/`、`legacy/` 这类历史包袱。内容全在 `content/`。

## 已知问题（会随着重构消失）

- **日文乱码**：`TextHelper::RenderTextToTextureBold` 用 `SHIFTJIS_CHARSET` 字体 +
  `TextOutA` + `strlen` 按字节处理双字节文本。换成 SDL_ttf/FreeType 后自然解决。
- **设置菜单闪退**：`DrawStringFormat2` 不返回。同样在换掉 GDI 文字路径后消失。

两个问题都在原版代码的 GDI 文字路径上，第 1 步（渲染替换）就会一起解决。
