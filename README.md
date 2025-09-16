Arduino CH592F 核心支持

概述

CH592F 是一款集成 BLE 无线通信的 32 位 RISC 微控制器。本核心为 Arduino 环境提供支持，让开发者能够利用其丰富外设资源。

参考实现: https://github.com/ElectronicCats/arduino-wch58x

功能支持状态

功能 状态 说明

基础功能

数字输出 ✅ digitalWrite()

数字输入 ✅ digitalRead()

模拟读取 ✅ analogRead()

模拟写入 ✅ analogWrite()

通信接口

串口 (UART) ✅ 支持4个串口

I²C ✅ 完整实现

SPI ❌ 我这无法测试，就没开发，大佬有需要可以开发

USB ❌ 开发中

无线功能

BLE 外设 ❌ 虽然库支持了蓝牙所需的一切环境但由于无法很好和loop配合，大佬们可以尝试一下

BLE 主机 ❌ 虽然库支持了蓝牙所需的一切环境但由于无法很好和loop配合，大佬们可以尝试一下

特色功能

触摸检测 ❌ 我这无法测试，就没开发，大佬有需要可以开发

低功耗模式 ❌ 我这无法测试，就没开发，大佬有需要可以开发

开发工具

程序烧录 ✅ 完整支持

快速开始

1. 添加开发板管理网址: https://github.com/TouchBlueSky/arduino-core-ch592f/releases/download/tools/package_ch592f_index.json
2. 通过 Arduino 开发板管理器安装 "WCH Boards"
3. 在工具菜单中选择 "CH592" 开发板
4. 上传示例程序验证安装
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}

引脚规则：
PA0 对应 0，依次递增
PB0 对应 16，依次递增

调试接口：
默认串口为串口1，即PA9 ，PA8
默认LED为PA8
所以默认情况下，串口和LED不能同时使用
可以使用其他串口可以采用这种方式，例如采用串口0（PB7、PB4）：
#undef Serial
#define Serial Serial0

一共支持串口0~3，详细可以看手册，可以硬件重映射
这里简单提一下默认值
串口0 PB7 PB4
串口1 PA9 PA8
串口2 PA7 PA6
串口3 PA5 PA4

下载步骤（需要管理员权限）为：
1、点击上传，会出现正在上传百分之0
2、按住PB22按键插入usb
3、等待下载完成

如果提示如下则说明arduino软件没用管理员权限打开
WCHISPTool_CH57x-59x.exe: The requested operation requires elevation.

开发计划
   本来想移植个功能比较全的给大家使用，但目前由于蓝牙问题一直没好思路搞定，就半成品搁在这里了
   欢迎各位大佬参靠IIC代码继续贡献代码，感谢大家支持

• 触摸检测库

• 低功耗模式

• USB 主机功能

欢迎提交 Issue 和 Pull Request 参与项目贡献！
