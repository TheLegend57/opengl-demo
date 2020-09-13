g++ src/* -O3 -o ./Release/main -L/Dependencies/GLFW/lib/macos/ -Wl,-rpath /Dependencies/GLFW/lib/macos/ -Ibuild/include Dependencies/glad/src/glad.c -lglfw3 -framework OpenGL -std=c++17 -DNDEBUG
echo "If there were no errors file 'main' should be in Release folder"
