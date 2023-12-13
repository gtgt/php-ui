#!/bin/bash
rm -rf deps libui modules
mkdir -p deps/lib
mkdir -p deps/include
git clone --depth=1 -bfeature/path-improv https://github.com/vike2000/libui
cd libui
meson build
ninja -C build
cp build/meson-out/libui.* ../deps/lib
cp ui*.h ../deps/include
cd ../
ln -sf libui.so.0 deps/lib/libui.so

phpize
./configure --with-ui=deps
make clean
make
