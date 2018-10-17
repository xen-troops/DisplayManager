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
## Configuration file:
DisplayManager uses Wayland IVI Extension to manipulate with Wayland surfaces. See https://at.projects.genivi.org/wiki/display/WIE/Wayland+IVI+Extension+Home and https://at.projects.genivi.org/wiki/display/PROJ/Wayland+IVI+Extension for more details about Wayland IVI Extension.

Basically DisplayManager operates with following primitives:
* Displays - represent physical Wayland displays;
* Layers - belongs to the display and manages group of surfaces;
* Surface - renders image.

All configuration is done in the DisplayManager configuration file. The format of the configuration file described in https://hyperrealm.github.io/libconfig/libconfig_manual.html#Configuration-Files.

The configuration file consists of following sections:
* displays - array of display descriptions;
* layers - array of layer descriptions;
* surfaces- array of surfaces descriptions;
* events - array of event descriptions.

With displays, layers and surfaces sections initial composition and initial surfaces layout can be described:

### displays

In display section all Wayland displays used by DisplayManager should be described. Each display item has following mandatory parameters:
* id - Wayland IVI Extension existing display ID;
* name - any unique meaningful name to be referenced in the configuration file.

Example:
```
displays = (
    {
        id = 0;
        name = "First";
    },
    {
        id = 1;
        name = "Second";
    }
);
```

### layers

In layer section all layers used by DisplayManager should be described. In order to display surfaces at least one layer for one display should be defined. Each layer item has following mandatory parameters:
* id - Wayland IVI Extension unique layer ID. Unlike the displays, layers will be created by DisplayManager on initialization. So, ID field should be unique in the whole system not only in this particular configuration file.
* name - any unique meaningful name to be referenced in the configuration file;
* display - name of display the layer belongs to;
* width - width of layer in pixels;
* height - height of layer in pixels.
  
Optional parameters:
* visibility - indicates should be this layer visible or not. Can be 0 or 1 (default: 1);
* opacity - sets layer opacity. Float value between 0.0 - 1.0 (default: 1.0);
* order - layer render order. Layers with lower order will be rendered on top of layers with higher order value. Integer value (default: 0);
* source - rectangle layer area which will be take for rendering (default: x = 0, y = 0, width = layer width, height = layer height);
* destination - rectangle display area into which source of layer will be render to (default: destination = source).

Source and destination sections describes rectangular area as horizontal offset, vertical offset, width and height and consist of following optional parameters:
* x - horizontal offset in pixels;
* y - vertical offset on pixels;
* width - width in pixes;
* height - height in pixels.

Example:
```
layers = (
    {
        name = "First";
        id = 0;
        display = "First";
        width = 1024;
        height = 768;
    },
    {
        name = "Second";
        id = 1;
        display = "Second";
        width = 1024;
        height =768;
        opacity = 0.5;
        destination = {
            x = 100,
            y = 100,
            width = 500,
            height = 500
        }
    }
);
```

### surfaces

In surface section all surfaces used by DisplayManager should be described. Each surface item has similar to layer parameters except display referencing it uses layer parameter with layer name.
Surface mandatory parameters:
* id - Wayland IVI Extension unique surface ID. DisplayManager doesn't create surfaces but manage how they are rendered. ID should be equal to certain surface ID in the system;
* name - any unique meaningful name to be referenced in the configuration file;
* layer - name of layer the surface belongs to;
* width - width of surface in pixels;
* height - height of surface in pixels.
  
Optional parameters:
* visibility - indicates should be this surface visible or not. Can be 0 or 1 (default: 1);
* opacity - sets surface opacity. Float value between 0.0 - 1.0 (default: 1.0);
* order - surface render order. Order has same logic as layer order but indicates surface render order inside layer. Integer value (default: 0);
* source - rectangle surface area which will be take for rendering (default: x = 0, y = 0, width = surface width, height = surface height);
* destination - rectangle layer area into which source of surface will be render to (default: destination = source).

Source and destination sections describes rectangular area in same format as layer destination and source sections.

Example:
```
surfaces = (
    {
        name = "Main";
        id = 1000;
        layer = "First";
        width = 1920;
        height = 720;
        destination = {
            width = 1024;
            height = 600;
        }
        order = 0;
    },
    {
        name = "Clock";
        id = 1001;
        layer = "First";
        visibility = 0;
        opacity = 1;
        width = 200;
        height = 200;
        destination = {
            x = 412;
            y = 200;
        }
        order = 1;
    }
);
```

### events

Events is designed to allow dynamically layout changing. For example when a new surface appears, existing surfaces can be moved to another layer, scaled or can be hidden etc. Currently following events are supported:
* create - occurs when a new object is created (supported objects: surface, layer);
* destroy - occurs when an existing object is destroyed (surface or layer);
* user - occurs when a user event received by DBus. DBus XML file to send user event can be found here: https://github.com/xen-troops/DisplayManager/blob/master/src/com.epam.displaymanager.xml

Each event has common parameters and individual depends on event type. Event common parameters:
* event - name of event. String value: "create", "destroy", "user";
* actions - array of action to be performed when the event occurs.

Create, destroy event parameters:
* object - type of created object. String value: "surface", "layer";
* name - name of created object. String value.

User event parameters:
* id - user event id. Integer value received in user event DBus notification.

The action changes some object (surface or layer) parameters.
Currently following actions are supported:
* source - change object source parameter;
* destination - change object destination parameters;
* parent - change object parent (move surface to another layer or move layer to another display);
* order - change object render order;
* visibility - change object visibility;
* opacity - change object opacity.

Each action has common parameters and individual depends on action type.
Action common parameters:
* object - type of object which parameters are changing. String value: "surface", "layer";
* name - name of object which parameters are changing;

Source, destination action parameters:
* x - horizontal offset in pixels. Integer value (default: -1);
* y - vertical offset in pixels. Integer value (default: -1);
* width - width in pixels. Integer value (default: -1);
* height - height in pixels. Integer value (default: -1).

*"-1" means this parameter will not be changed.*

Parent action parameters:
* parent - name of new parent (name of layer for surface or name of display for layer).

Order action parameters:
* order - new object order. Mandatory integer value.

Visibility action parameters:
* visibility - new object visibility. Mandatory integer value: 0 or 1.

Opacity action parameters:
* opacity - new object opacity. Mandatory float value: 0.0 - 1.0.

Example:
```
events = (
    {
        event = "USER";
        id = 0;
        actions = (
            {
                action = "VISIBILITY";
                object = "SURFACE";
                name = "Clock";
                visibility = 0;
            }
        );
    },
    {
        event = "CREATE";
        object = "SURFACE"
        name = "Tools"
        actions = (
            {
                action = "VISIBILITY";
                object = "SURFACE";
                name = "Clock";
                visibility = 0;
            }
        );
    },
    {
        event = "DESTROY";
        object = "SURFACE"
        name = "Tools"
        actions = (
            {
                action = "VISIBILITY";
                object = "SURFACE";
                name = "Clock";
                visibility = 1;
            }
        );
    },
    {
        event = "CREATE";
        object = "SURFACE"
        name = "Help"
        actions = (
            {
                action = "VISIBILITY";
                object = "SURFACE";
                name = "Clock";
                visibility = 0;
            },
            {
                action = "DESTINATION";
                object = "SURFACE";
                name = "Main";
                width = 512
                height = 384
            }
        );
    },
    {
        event = "DESTROY";
        object = "SURFACE"
        name = "Help"
        actions = (
            {
                action = "VISIBILITY";
                object = "SURFACE";
                name = "Clock";
                visibility = 1;
            },
            {
                action = "DESTINATION";
                object = "SURFACE";
                name = "Main";
                width = 1024
                height = 768
            }
        );
    }
);
```

## How to run:
```
DisplayManager -c ${PATH_TO_CONFIG_FILE} -v${LOG_MASK}
```

Example:

```
DisplayManager -c display.cfg -v *:Debug
```