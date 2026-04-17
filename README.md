# iLoabotDevice
设备控制驱动器

基于 C++ 的抽象工厂模式 + 工厂模式示例，抽象了 iLoabot 对象，包含以下设备工厂：

- RobotFactory
- AGVFactory
- SuctionCupFactory
- BatteryFactory
- ChargerFactory
- LightFactory
- PLCFactory

## 构建运行

```bash
cmake -S . -B build
cmake --build build
./build/iLoabotDevice
```
