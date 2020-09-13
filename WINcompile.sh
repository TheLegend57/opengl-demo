g++ src/*.cpp -std=c++17 -O3 -o ./Release/main.exe -Ibuild/include Dependencies/glad/src/glad.c -L./Dependencies/GLFW/lib/windows/ -Wl,-rpath Dependencies/GLFW/lib/windows/ -lglfw3dll -DNDEBUG
echo "If there were no errors file 'main.exe' should be in Release folder"
