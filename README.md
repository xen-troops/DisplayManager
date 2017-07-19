# DisplayManager

## Dependencies:
### Required:
* libxenbe log
* libconfigc++
* wayland-ivi-extension

## How to configure
```
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}
cmake ${PATH_TO_SOURCES} -D${VAR1}=${VAL1} -D{VAR2}=${VAL2} ...
```
There are variables and options. Options can be set to ON and OFF.

Supported options:

| Option | Description |
| --- | --- |
| `WITH_DOC` | Creates target to build documentation. It required Doxygen to be installed. If configured, documentation can be create with `make doc` |

Supported variables:

| Variable | Description |
| --- | --- |
| `CMAKE_BUILD_TYPE` | `Release`, `Debug`, `RelWithDebInfo`, `MinSizeRel`|
| `CMAKE_INSTALL_PREFIX` | Default install path |
| `XENBE_LOG_INCLUDE_PATH` | Path to libxenbe includes if they are located in non standard place |

Example:
```
// Debug build
cmake ${PATH_TO_SOURCES} -DCMAKE_BUILD_TYPE=Debug
```

## How to build:
```
cd ${BUILD_DIR}
make     // build sources
make doc // build documentaion
```
## How to install
```
make install // to default location
make DESTDIR=${PATH_TO_INSTALL} install //to other location
```
## How to run:
```
DisplayManager -c ${PATH_TO_CONFIG_FILE} -v${LOG_MASK}
```

Example:

```
DisplayManager -c display.cfg -v *:Debug
```