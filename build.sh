#!/bin/bash
rm -rf deps libui
mkdir -p deps/lib
mkdir -p deps/include
git clone --depth=1 https://github.com/andlabs/libui
cd libui
meson build
ninja -C build
cp build/meson-out/libui.* ../deps/lib
ln -s libui.so.0 ../deps/lib/libui.so
cp ui*.h ../deps/include
cd ../

phpize
./configure --with-ui=deps
make clean
make
