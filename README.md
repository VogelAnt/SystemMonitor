# System monitor GUI mocc
## Compilation
- Install dependencies: `./scripts/prebuild.sh`
- Compilation:
```
mkdir build && cd build
cmake .. && make
```
  - Compile options:
    - SMG_BUILD_TESTS=ON if unit tests are needed. Default: OFF

## Usage
- Start the SMG: `./build/source/system-monitor-gui`
- Monitor the state of each device/skill in the device tab widget. Each tab represents a device.
