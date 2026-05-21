# Rastreador Personal de Salud

Desktop application for tracking personal health indicators: weight, blood pressure, and glucose levels.

Built with C++17, Qt 5.15.2, and SQLite3.

## Features

- User authentication (register and login)
- Record biometric measurements with automatic timestamp
- View measurement history in a sortable table
- Calculate averages, weight trend, BMI, and daily caloric needs
- Export all data to CSV

## Requirements

- Qt 5.15.2 (Desktop gcc 64-bit, Qt Charts, Qt SQL)
- CMake 3.16+
- SQLite3
- GCC 13+

## Build Instructions

```bash
git clone https://github.com/CHHMNT/RastreadorSalud.git
cd RastreadorSalud
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/5.15.2/gcc_64
make -j$(nproc)
./RastreadorSalud
```

## Project Structure

RastreadorSalud/
├── src/          # Source files (.cpp)
├── include/      # Header files (.h)
├── db/           # SQL schema
├── docs/         # Doxygen generated documentation
└── CMakeLists.txt

## Documentation

Generate with:
```bash
doxygen Doxyfile
```
Then open `docs/html/index.html` in a browser.