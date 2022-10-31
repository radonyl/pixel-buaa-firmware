# Pixel BUAA 固件

该项目使用 STM32CubeMX V6.2.1 + CLion 2021.2 生成
环境的配置，请参考：[Pixel BUAA 开发板环境配置教程](https://blog.t123yh.xyz:2/index.php/archives/821)
## 硬件

### STM32F042G6U6
| 引脚 | 定义 | 备注 |
| --- | --- | --- |
| PB8 | 按钮1  | 按下为高电平 |
| PB7 | I2C1_SDA | |
| PB6 | I2C1_SCL | 速率：400kHz |
| PA13 | SWD | |
| PA14 | SWC | |

### IS31FL3731
- 地址：`0b1110100` (`0xE4`)

### LED 矩阵
R: 只有红色 / W: 只有白色 / B: 双色
```
0  R R R R R R R R           <- TITLE

   0 1 2 3 4 5 6 7 8 9 A -> x

0  W W W B B B B B W W W     <- BUILDING
1  W W W B B B B B W W W
2  W W W B B B B B W W W
3  W W W B B B B B W W W
4    W W   B B B   W
5      W B B B B B W
y
```

## 软件
核心代码位于 `Core/Src` 目录下，主要包括：
- `button.c`: 定义了不同按键事件的 Callback
- `led_matrix.c`: 定义了写入 LED 矩阵的 API，`led_matrix_def.c` 储存了逻辑 Index 和硬件 Index 之间的映射关系（无需改动）
- `morse.c`: 莫尔斯码解码器
- `font5x5.h`: 莫尔斯码解码结果（仅字母）的字体
- `display.c`: 显示循环，根据不同的莫尔斯码解码结果，显示不同的动画，动画的源代码位于 `animation` 目录下
- 该目录下其余的 `.c` 文件，均为 STM32CubeMX 自动生成，所有的修改要在 `User Code Begin ... End` 之间，否则重新生成项目时会被删除
  - `stm32f0xx_it.c`: 系统生成的中断服务函数，在 Systick 的服务函数中，加入了 `display.c` 中的时间系统增量函数，保证时间在流逝
  - `main.c`: 包含了初始化代码和最顶层循环

调用的第三方库位于 `Drivers/Peripheral` 目录下，主要包括：
- `is31fl3731.c`: LED 灯控芯片的驱动
- `multi_button.c`: 按钮扫描驱动