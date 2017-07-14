mkdir build_web

mkdir data
mkdir data\res
xcopy res\layout data\res\layout /E /Y /I
xcopy res\config.json data\res\config.json /E /Y /I

del build_web\macao.*
del web\macao.*

cmake -DCMAKE_TOOLCHAIN_FILE="C:\Program Files\Emscripten\emscripten\1.35.0\cmake\Modules\Platform\Emscripten.cmake" -G "Unix Makefiles" -Bbuild_web -DOUT=build_web -Hweb -DCMAKE_BUILD_TYPE=Release -DOPENGL=true -DAPP_DEBUG=true -DBUILD_WEB=true
make -C build_web

del data /Q /S

xcopy build_web\macao.data web /Y
xcopy build_web\macao.js web /Y
xcopy build_web\macao.html.mem web /Y
xcopy res web\v1\res /E /Y /I
