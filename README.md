# CX

Custome hex dump written in C following the style of `xxd`

## Requirements

- `cmake`

## Building

1. Clone the repo
    - `git clone https://github.com/CesareDev/cx.git`
2. Run the script `build.sh`
3. Navigate to the to the desired build and compile
    - `cd build/release/cx && make` 
    - `cd build/debug/cx && make`
4. Run the bin
    - `./ck some_file`

- For the library go to `build/<release/debug>/cxlib/`
- Here you can find the header file `cxapi.h` inside the `include` directory
