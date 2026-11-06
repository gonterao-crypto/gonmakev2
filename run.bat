del /s /q program.exe

gonmake -dir "./" -o "program.exe" -cu "--target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths -fcolor-diagnostics -fansi-escape-codes -fexperimental-library"
if exist "program.exe" (
  call runonly.bat
) else (
  echo Error!
)
