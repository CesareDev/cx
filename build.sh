#! /usr/bin/bash
rm -r build/
rm compile_commands.json 
mkdir -p build/debug
mkdir -p build/release
cmake -S . -B build/release/ -DCMAKE_BUILD_TYPE=Release
cmake -S . -B build/debug/ -DCMAKE_BUILD_TYPE=Debug
ln -s build/debug/compile_commands.json .
