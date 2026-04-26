# iLoabotDevice
设备控制驱动器。

该项目基于 C++17，采用“Device 抽象工厂 + 产品工厂注册器 + 设备能力接口 + 通信协议抽象”的结构，用于管理 iLoabot 系列设备，并为后续接入真实控制链路预留统一扩展点。通信层优先基于 Boost.Asio 实现，以满足 Linux / Windows 等平台的可移植性要求。

## 架构概览

项目分为 4 层：

1. 抽象工厂层：`Device` 负责协调多个产品工厂，按型号装配出完整设备。
2. 产品接口层：`DeviceProduct` 定义所有产品共享的名称、类型、型号接口。
3. 产品实现层：每个产品族都按“产品接口 + 多个具体产品 + 一个产品工厂”组织具体行为。
4. 通信协议层：提供串口、TCP Client、TCP Server、Socket 等通用通信封装，便于设备接入真实硬件或上位机。

## 项目结构

```text
iLoabotDevice/
├── CMakeLists.txt
├── README.md
├── include/iloabot/
│   ├── device.h                     # Device 抽象工厂与装配结果
│   ├── device_product.h             # 产品基类
│   ├── device_types.h               # 共用类型（DeviceStatus、Position）
│   ├── product_factory.h            # 产品工厂基类与工厂注册器
│   ├── factories/                   # 具体工厂声明
│   │   ├── robot_factory.h
│   │   ├── agv_factory.h
│   │   ├── suction_cup_factory.h
│   │   ├── battery_factory.h
│   │   ├── charger_factory.h
│   │   ├── light_factory.h
│   │   └── plc_factory.h
│   ├── interfaces/                  # 通用控制接口
│   │   ├── controllable.h           # 初始化/关闭/重置
│   │   ├── status_provider.h        # 状态查询
│   │   ├── configurable.h           # 参数读写
│   │   ├── movable.h                # 运动控制
│   │   ├── switchable.h             # 开关控制
│   │   └── power_manageable.h       # 电源管理
│   ├── devices/                     # 产品接口、具体产品与具体抽象工厂声明
│   │   ├── robot.h                  # ILoabotRobot 产品接口
│   │   ├── robot002.h
│   │   ├── agv.h                    # ILoabotAGV 产品接口
│   │   ├── agv002.h
│   │   ├── suction_cup.h            # ILoabotSuctionCup 产品接口
│   │   ├── suction_cup002.h
│   │   ├── battery.h                # ILoabotBattery 产品接口
│   │   ├── battery002.h
│   │   ├── charger.h                # ILoabotCharger 产品接口
│   │   ├── charger002.h
│   │   ├── light.h                  # ILoabotLight 产品接口
│   │   ├── light002.h
│   │   ├── plc.h                    # ILoabotPLC 产品接口
│   │   ├── plc002.h
│   │   └── iloabot_device.h
│   └── comm/                        # 通信协议抽象
│       ├── connection.h             # 通用连接接口
│       ├── socket.h                 # Boost.Asio Socket 封装
│       ├── serial_port.h            # 串口通信
│       ├── tcp_client.h             # TCP 客户端
│       └── tcp_server.h             # TCP 服务端
└── src/
    ├── main.cpp
    ├── product_factory.cpp          # 工厂注册器实现
    ├── factories/                   # 具体工厂实现
    │   ├── robot_factory.cpp
    │   ├── agv_factory.cpp
    │   ├── suction_cup_factory.cpp
    │   ├── battery_factory.cpp
    │   ├── charger_factory.cpp
    │   ├── light_factory.cpp
    │   └── plc_factory.cpp
    ├── devices/                     # 产品与抽象工厂实现
    │   ├── robot.cpp
    │   ├── robot002.cpp
    │   ├── agv.cpp
    │   ├── agv002.cpp
    │   ├── suction_cup.cpp
    │   ├── suction_cup002.cpp
    │   ├── battery.cpp
    │   ├── battery002.cpp
    │   ├── charger.cpp
    │   ├── charger002.cpp
    │   ├── light.cpp
    │   ├── light002.cpp
    │   ├── plc.cpp
    │   ├── plc002.cpp
    │   └── iloabot_device.cpp
    └── comm/                        # 通信实现
        ├── socket.cpp
        ├── serial_port.cpp
        ├── tcp_client.cpp
        └── tcp_server.cpp
```

## 核心设计

### 1. Device 抽象工厂

`Device` 不再表示某个具体产品，而是表示“设备装配器”。

它负责：

- 接收一份装配规格 `DeviceBuildSpec`
- 调用不同产品工厂按型号生产产品
- 把这些产品组装进 `AssembledDevice`

这样 `Device` 关注的是“如何组装一台设备”，而不是“某个产品本身的行为”。

### 2. 产品基类

所有具体产品统一继承 `DeviceProduct`，对外暴露：

- `name()`：产品名称
- `type()`：产品族类型
- `model()`：产品型号

### 3. 产品接口按产品划分

每个产品族对外暴露一个独立的产品接口，调用方只需包含对应的产品头文件，即可使用该产品的全部方法：

| 产品接口 | 方法 |
|---|---|
| `ILoabotRobot` | `initialize / shutdown / reset / status / moveTo / stop / currentPosition` |
| `ILoabotAGV` | `initialize / shutdown / reset / status / moveTo / stop / currentPosition` |
| `ILoabotSuctionCup` | `initialize / shutdown / reset / status / turnOn / turnOff / isOn` |
| `ILoabotBattery` | `status / batteryLevel / isCharging` |
| `ILoabotCharger` | `initialize / shutdown / reset / status / turnOn / turnOff / isOn` |
| `ILoabotLight` | `turnOn / turnOff / isOn` |
| `ILoabotPLC` | `initialize / shutdown / reset / status / setParam / getParam` |

不再使用 `Controllable`、`Movable`、`Switchable` 等横切功能 mixin，接口边界以产品为单位。

### 4. 产品工厂 + 注册器

`iloabot::factories::ProductFactory` 是产品工厂抽象基类；`RobotFactory`、`AGVFactory`、`SuctionCupFactory` 等具体工厂负责各自产品族的生产。

每个工厂都支持：

- 返回自己支持的型号列表 `supportedModels()`
- 给出默认型号 `defaultModel()`
- 按型号创建产品 `create(model)`

`ProductFactoryRegistry` 负责维护“产品族类型 -> 具体工厂”的映射，并统一提供按类型、按工厂类型、按产品类型的查找与创建入口。

约定：

- 每个具体工厂都继承 `TypedProductFactory<ProductT>`，并显式声明 `using ProductType = ProductT`。
- `ProductType` 必须是 `DeviceProduct` 的子类，用于统一生成产品类型索引，支持按具体产品类型创建。
- 这样新增工厂时无需手写 `productTypeKey()`，避免重复代码和映射不一致。
- 如果同一产品族下存在多个实现，例如 `SuctionCup`、`SuctionCup002`，则由同一个产品工厂根据型号选择返回哪一个具体产品实现。

优势：

- `Device` 和 `Factory` 职责分离：前者负责组装，后者负责生产
- 每个工厂都可以生产同一产品族下的不同型号
- 便于后续在工厂层注入默认配置、连接参数或初始化策略

### 5. 通信协议抽象

通信层从设备层中独立出来，便于复用与替换：

- `Connection`：统一定义 `open()`、`close()`、`send()`、`receive()`
- `Socket`：对底层 Boost.Asio TCP Socket/Acceptor 的轻量封装
- `SerialPort`：串口协议封装，支持波特率、校验位、停止位等配置
- `TcpClient`：面向主动连接场景
- `TcpServer`：面向监听和接收客户端连接场景

后续如果要增加 UDP、Modbus、CAN、WebSocket，也可以沿用当前模式继续扩展。

## 设备方法一览

| 设备 | 控制 | 状态 | 运动 | 开关 | 参数 | 电源 |
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

### 新增一个产品族

1. 在 `include/iloabot/devices/` 中新增产品接口头文件，继承 `DeviceProduct` 和所需能力接口。
2. 为该产品族新增一个或多个具体产品头文件，例如 `*002.h`。
3. 在 `src/devices/` 中实现共享基类逻辑和各个具体产品逻辑。
4. 在 `include/iloabot/factories/` 中新增对应工厂，继承 `TypedProductFactory<ProductT>`。
5. 在工厂中实现 `supportedModels()`、`defaultModel()` 和 `create(model)`，由工厂根据型号选择具体实现。
6. 在 `src/product_factory.cpp` 中注册该工厂。
7. 在 `CMakeLists.txt` 中加入对应源文件。

以吸盘为例：

- `suction_cup.h` 只定义产品接口 `ILoabotSuctionCup`
- `suction_cup002.h` 声明具体产品
- `suction_cup.cpp` 实现共享逻辑
- `suction_cup002.cpp` 实现第二个具体产品
- `SuctionCupFactory` 根据 `SC-200` 等型号决定返回哪个具体实现

### 新增一个抽象工厂设备

1. 在 `include/iloabot/devices/` 中新增设备装配器头文件，继承 `Device`。
2. 在 `src/devices/` 中实现 `assemble(const DeviceBuildSpec&)`。
3. 在装配函数里通过 `ProductFactoryRegistry` 调用不同产品工厂。
4. 把产出的产品加入 `AssembledDevice`。

### 新增一个通信协议

1. 在 `include/iloabot/comm/` 中定义接口或协议类。
2. 如果可复用 `Connection`，优先继承它以保持统一调用方式。
3. 在 `src/comm/` 中实现协议细节。
4. 在 `CMakeLists.txt` 中加入源文件。
5. 由设备类按需组合该通信对象，而不是把通信逻辑散落到业务代码中。

## 使用示例

### 组装设备

```cpp
iloabot::factories::registerILoabotProductFactories();

auto& registry = iloabot::factories::ProductFactoryRegistry::instance();
ILoabotDevice device(registry);
DeviceBuildSpec spec;
spec.robotModel = "R-200";
spec.suctionCupModel = "SC-200";

auto assembled = device.assemble(spec);
for (const auto& product : assembled->products()) {
    if (product) {
        std::cout << product->type() << " / " << product->model() << std::endl;
    }
}
```

### 使用具体工厂按型号生产

```cpp
#include "iloabot/factories/suction_cup_factory.h"

iloabot::factories::SuctionCupFactory factory;
auto suctionCup = factory.createAs<ILoabotSuctionCup>("SC-200");

if (suctionCup) {
    suctionCup->initialize();
    suctionCup->turnOn();
}
```

### 通过注册器按类型创建

```cpp
auto& registry = iloabot::factories::ProductFactoryRegistry::instance();

// 按具体工厂类型创建具体产品
auto byFactory = registry.createByFactoryAs<
    iloabot::factories::SuctionCupFactory,
    ILoabotSuctionCup>("SC-200");

// 按具体产品类型创建
auto byProduct = registry.createByProduct<ILoabotSuctionCup>("SC-200");

// 生命周期管理
registry.unregisterFactory("SuctionCup");
registry.unregisterFactoryByType<iloabot::factories::SuctionCupFactory>();
registry.clear();
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
