# Gemini Gaggle - A free crawler written in C++ with minimal libraries required

## Dependencies
- OpenSSL
- SQLite3

### macOS
Make sure to have a basic CPP development set up with a compiler and CMake

```
brew install openssl
brew install sqlite
```

### Linux
Make sure to have a basic CPP development set up with a compiler and CMake

```
sudo apt install libssl-dev
sudo apt install libsqlite3-dev
```

### Windows
¯\\\_(ツ)\_/¯ Good luck


## Building
Ensure all dependencies are installed then simply run

```
mkdir build
cd build
cmake ../ -GNinja
ninja
./gemini-gaggle
```

## Contributing
Feel free to contribute, however this is not a project that will be maintained regularly and is simply just for fun.
