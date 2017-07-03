#
# Copyright (C) 2017 Manh Tran
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#

#create target folder
mkdir -p build_web

#copy file from res to data for preloading
mkdir -p data
mkdir -p data/res
cp -rf res/layout data/res
cp res/config.json data/res

#remove previous dependencies
rm build_web/macao.*

#build
cmake -DCMAKE_TOOLCHAIN_FILE=/home/manh/Downloads/emsdk_portable/emscripten/master/cmake/Modules/Platform/Emscripten.cmake -Bbuild_web -DOUT=build_web -Hweb -DCMAKE_BUILD_TYPE=Release -DOPENGL=true -DAPP_DEBUG=true -DBUILD_WEB=true
make -C build_web

#clear temp data
rm -rf data

cp build_web/macao* web
