# Where Is My Boat

An OpenCPN plugin. When your own ship is outside the visible chart, an arrow
at the chart edge points towards it and shows its distance and bearing from
the chart centre. Click the arrow to move the chart to the boat at the current
scale.

- Works in OpenGL and non-OpenGL mode, on every chart canvas.
- Follows chart rotation (course up, head up).
- The arrow turns grey with "(old fix)" when no new position has arrived for
  a minute.

Requires OpenCPN 5.8 or newer (plugin API 1.18).

## Building

The build files are the [OpenCPN plugin template](https://github.com/Rasbats/shipdriver_pi).
Only `Plugin.cmake`, `src/`, `data/` and `po/POTFILES.in` belong to this
plugin.

```sh
git clone --recurse-submodules https://github.com/istoh/whereismyboat_pi.git
cd whereismyboat_pi
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target tarball
```

Import the resulting tarball in **Options → Plugins → Import Plugin Tarball**.
Packages for Linux, Raspberry Pi and Android are built by
[opencpn-plugin-builds](https://github.com/istoh/opencpn-plugin-builds).

The geometry has standalone tests:

```sh
g++ -std=c++17 -Isrc tests/pointer_test.cpp src/pointer.cpp -o pointer_test && ./pointer_test
```

## License

GPL-3.0-or-later, see `COPYING`. The build files come from the OpenCPN
plugin template under the same license.
