# Gemini Gaggle - A free crawler written in C++ with minimal libraries required

## Dependencies
- OpenSSL
- SQLiteCpp (Git Submodule)

### macOS
Make sure to have a basic CPP development set up with a compiler and CMake or Meson and ideally Ninja
```
brew install openssl
```

### Linux
Make sure to have a basic CPP development set up with a compiler and CMake or Meson and ideally Ninja
```
sudo apt install libssl-dev
```

### Windows
¯\\\_(ツ)\_/¯ Good luck

## Building SQLiteCpp Dependency
I prefer to use Meson since its a little more user friendly than CMake.
This will compile and install the SQLiteCpp library to your machine.

### Pull submodule
```
git submodule update --init --recursive
```

### Create build directory and move into it
```
cd SQLiteCpp
mkdir build
cd build
```

### Generate build scripts, compile and install
- With Meson:
```
meson setup ../
meson compile
meson install
```
- With CMake:
```
cmake ../ -GNinja
ninja
ninja install
```

## Building Gaggle-Gemini
Ensure all dependencies are compiled and installed then simply run

### Create build directory and move into it
```
mkdir build
cd build
```

### Generate build scripts, compile and install
- With CMake:
```
cmake ../ -GNinja
ninja
./gemini-gaggle
```
- With Meson:
```
meson setup ../
meson compile
./gemini-gaggle
```

## Contributing
Feel free to contribute, however this is not a project that will be maintained regularly and is simply just for fun.
