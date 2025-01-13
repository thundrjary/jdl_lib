#gcc -g -o app.exe main.c -I"C:\code\languages\clang\include" -L"C:\code\languages\clang\lib\glfw"  -lglfw3 -lopengl32 -lgdi32
#.\app.exe
gcc -o app.exe main.c -IC:\code\languages\clang\include -IC:\code\languages\clang\include\GLFW -IC:\code\languages\clang\include\GL -LC:\code\languages\clang\lib\glfw -LC:\code\languages\clang\lib\glew -lglfw3 -lglew32 -lopengl32 -lgdi32 -luser32 -lshell32 -lm
