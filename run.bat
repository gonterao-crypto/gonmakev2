del /s /q gonmakev2.exe

gonmake -dir "./" -o "gonmakev2.exe" -cu "--target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths -fcolor-diagnostics -fansi-escape-codes -fexperimental-library"
if exist "gonmakev2.exe" (
  call runonly.bat
) else (
  echo Error!
)
