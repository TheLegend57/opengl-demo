g++ src/*.cpp -O3 -o ./Release/main -L./Dependencies/GLFW/lib/linux/ -lglfw3 -Ibuild/include Dependencies/glad/src/glad.c -ldl -lGL -lX11 -lpthread -std=c++17 -DNDEBUG
echo "If there were no errors file 'main' should be in Release folder"
