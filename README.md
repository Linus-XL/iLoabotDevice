# iLoabotDevice
设备控制驱动器。

该项目基于 C++17，采用“注册型工厂 + 设备能力接口 + 通信协议抽象”的结构，用于管理 iLoabot 系列设备，并为后续接入真实控制链路预留统一扩展点。通信层优先基于 Boost.Asio 实现，以满足 Linux / Windows 等平台的可移植性要求。

## 架构概览

项目分为 4 层：

1. 设备抽象层：定义所有设备共享的基础能力，例如名称、类型。
2. 能力接口层：把运动、开关、电源、配置、状态等能力拆分为可复用接口，避免巨型基类。
3. 设备实现层：不同设备按需组合能力接口，实现自己的业务行为。
4. 通信协议层：提供串口、TCP Client、TCP Server、Socket 等通用通信封装，便于设备接入真实硬件或上位机。

## 项目结构

```text
iLoabotDevice/
├── CMakeLists.txt
├── README.md
├── include/iloabot/
│   ├── device.h                     # Device 基类
│   ├── device_factory.h             # 注册型工厂
│   ├── interfaces/                  # 通用控制接口
│   │   ├── controllable.h           # 初始化/关闭/重置
│   │   ├── status_provider.h        # 状态查询
│   │   ├── configurable.h           # 参数读写
│   │   ├── movable.h                # 运动控制
│   │   ├── switchable.h             # 开关控制
│   │   └── power_manageable.h       # 电源管理
│   ├── devices/                     # 具体设备声明
│   │   ├── robot.h
│   │   ├── agv.h
│   │   ├── suction_cup.h
│   │   ├── battery.h
│   │   ├── charger.h
│   │   ├── light.h
│   │   └── plc.h
│   └── comm/                        # 通信协议抽象
│       ├── connection.h             # 通用连接接口
│       ├── socket.h                 # Boost.Asio Socket 封装
│       ├── serial_port.h            # 串口通信
│       ├── tcp_client.h             # TCP 客户端
│       └── tcp_server.h             # TCP 服务端
└── src/
    ├── main.cpp
    ├── device_factory.cpp
    ├── devices/                     # 设备实现
    │   ├── robot.cpp
    │   ├── agv.cpp
    │   ├── suction_cup.cpp
    │   ├── battery.cpp
    │   ├── charger.cpp
    │   ├── light.cpp
    │   └── plc.cpp
    └── comm/                        # 通信实现
        ├── socket.cpp
        ├── serial_port.cpp
        ├── tcp_client.cpp
        └── tcp_server.cpp
```

## 核心设计

### 1. 设备基类

所有设备统一继承 `Device`，对外暴露：

- `name()`：设备名称
- `type()`：设备类型

这样可以通过统一容器和统一工厂管理不同设备实例。

### 2. 能力接口拆分

不同设备不是继承一个庞大的总接口，而是按能力组合：

- `Controllable`：启动、关闭、重置
- `StatusProvider`：状态查询
- `Movable`：位置控制
- `Switchable`：开关控制
- `Configurable`：参数配置
- `PowerManageable`：电源状态

这种设计更贴合设备建模，也更方便后续扩展新的设备类型。

### 3. 注册型工厂

`DeviceFactory` 使用注册机制维护“设备类型 -> 创建设备对象”的映射。

优势：

- 新增设备时不需要修改工厂接口定义
- 避免为每种设备单独维护一组 Factory 类
- 降低模板代码和耦合度

### 4. 通信协议抽象

通信层从设备层中独立出来，便于复用与替换：

- `Connection`：统一定义 `open()`、`close()`、`send()`、`receive()`
- `Socket`：对底层 Boost.Asio TCP Socket/Acceptor 的轻量封装
- `SerialPort`：串口协议封装，支持波特率、校验位、停止位等配置
- `TcpClient`：面向主动连接场景
- `TcpServer`：面向监听和接收客户端连接场景

后续如果要增加 UDP、Modbus、CAN、WebSocket，也可以沿用当前模式继续扩展。

## 设备与接口映射

| 设备 | Controllable | StatusProvider | Movable | Switchable | Configurable | PowerManageable |
|------|:---:|:---:|:---:|:---:|:---:|:---:|
| Robot | ✓ | ✓ | ✓ |  |  |  |
| AGV | ✓ | ✓ | ✓ |  |  |  |
| SuctionCup | ✓ | ✓ |  | ✓ |  |  |
| Battery |  | ✓ |  |  |  | ✓ |
| Charger | ✓ | ✓ |  | ✓ |  |  |
| Light |  |  |  | ✓ |  |  |
| PLC | ✓ | ✓ |  |  | ✓ |  |

## 通信模块说明

### SerialPort

适用于设备串口接入，例如控制器、传感器、下位机串口链路。

可配置项包括：

- 设备路径
- 波特率
- 数据位
- 校验位
- 停止位
- 超时时间

### TcpClient

适用于设备主动连接远端服务，例如连接 PLC 网关、上位机或中控服务。

### TcpServer

适用于本程序作为服务端等待外部系统接入，例如调试工具、本地控制台或中控平台。

### Socket

适合作为更底层的网络基础组件，被 `TcpClient` 和 `TcpServer` 复用，也可供后续其他协议封装直接使用。

## 扩展方式

### 新增一个设备

1. 在 `include/iloabot/devices/` 中新增头文件。
2. 继承 `Device` 和所需能力接口。
3. 在 `src/devices/` 中实现对应方法。
4. 在 `src/device_factory.cpp` 中注册新设备。
5. 在 `CMakeLists.txt` 中加入对应源文件。

### 新增一个通信协议

1. 在 `include/iloabot/comm/` 中定义接口或协议类。
2. 如果可复用 `Connection`，优先继承它以保持统一调用方式。
3. 在 `src/comm/` 中实现协议细节。
4. 在 `CMakeLists.txt` 中加入源文件。
5. 由设备类按需组合该通信对象，而不是把通信逻辑散落到业务代码中。

## 使用示例

### 创建设备

```cpp
registerILoabotDevices();

auto& factory = DeviceFactory::instance();
auto robot = factory.create("Robot");

if (robot) {
    std::cout << robot->name() << std::endl;
}
```

### 使用 TCP 客户端

```cpp
#include "iloabot/comm/tcp_client.h"

TcpClient client({"127.0.0.1", 9000, 3000});

if (client.open()) {
    client.send("ping");
    auto response = client.receiveString();
    client.close();
}
```

### 使用串口

```cpp
#include "iloabot/comm/serial_port.h"

SerialConfig config;
config.device = "/dev/ttyUSB0";
config.baudRate = BaudRate::Baud115200;

SerialPort serial(config);
if (serial.open()) {
    serial.send("AT\r\n");
    auto response = serial.receiveString();
    serial.close();
}
```

## 构建运行

项目依赖 Boost.System：

```bash
cmake -S . -B build
cmake --build build
```

默认会生成两个目标：

- `build/libiLoabotDevice.so`：共享库，可供其他程序动态链接
- `build/iLoabotDeviceDemo`：示例程序，用于验证设备注册和基础调用流程

运行示例程序：

```bash
./build/iLoabotDeviceDemo
```
