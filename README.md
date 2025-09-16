Arduino CH592F 核心支持

概述

CH592F 是一款集成 BLE 无线通信的 32 位 RISC 微控制器。本核心为 Arduino 环境提供支持，让开发者能够利用其丰富外设资源。

参考实现: https://github.com/ElectronicCats/arduino-wch58x

功能支持状态

基础功能

功能 状态 说明

数字输出 ✅ digitalWrite()

数字输入 ✅ digitalRead()

模拟读取 ✅ analogRead()

模拟写入 ✅ analogWrite()
通信接口
功能 状态 说明

串口 (UART) ✅ 支持4个串口

I²C ✅ 完整实现

SPI ❌ 暂未测试，欢迎贡献

USB ❌ 暂未开发，欢迎贡献
无线功能
功能 状态 说明

BLE 外设 ❌ 库环境已支持，但伪任务不是很好和arduino配合，当前没很好思路

BLE 主机 ❌ 库环境已支持，但伪任务不是很好和arduino配合，当前没很好思路
特色功能
功能 状态 说明

触摸检测 ❌ 暂未开发，欢迎贡献

低功耗模式 ❌ 暂未开发，欢迎贡献
开发工具
功能 状态 说明

程序烧录 ✅ 完整支持

快速开始

安装步骤

1. 添加开发板管理网址：

   https://github.com/TouchBlueSky/arduino-core-ch592f/releases/download/tools/package_ch592f_index.json
   
2. 通过 Arduino 开发板管理器安装 "WCH Boards"
3. 在工具菜单中选择 "CH592" 开发板
4. 上传示例程序验证安装

示例代码

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}


技术细节

引脚映射规则

• PA0 对应 0，依次递增

• PB0 对应 16，依次递增

串口配置

• 默认串口：串口1 (PA9, PA8)

• 默认LED：PA8（因此串口和LED默认不能同时使用）

可使用其他串口：
#undef Serial
#define Serial Serial0  // 使用串口0 (PB7, PB4)


串口映射表：
串口 引脚

串口0 PB7, PB4

串口1 PA9, PA8

串口2 PA7, PA6

串口3 PA5, PA4

下载步骤

1. 点击上传，显示"正在上传"
2. 按住PB22按键插入USB
3. 等待下载完成

注意：需要以管理员权限运行Arduino IDE，否则可能出现错误：

WCHISPTool_CH57x-59x.exe: The requested operation requires elevation.


开发状态说明

当前版本已提供基础功能支持，但由于以下原因部分功能尚未完善：
• 蓝牙功能与Arduino loop()函数以及延时的协调需要进一步优化

• 部分外设功能因测试设备限制暂未开发

诚邀各位开发者参与贡献：

• 触摸检测库开发

• 低功耗模式实现

• USB主机功能完善

• SPI接口开发

• 蓝牙功能

欢迎参考现有的IIC代码结构进行开发！

贡献指南

我们欢迎各位开发者提交：
• Issue（问题反馈）

• Pull Request（代码贡献）

• 功能建议

• 测试报告

共同完善CH592F的Arduino生态支持！

项目地址: https://github.com/TouchBlueSky/arduino-core-ch592f
