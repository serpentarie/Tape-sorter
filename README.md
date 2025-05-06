# Tape-sorter
External tape sorting in C++ with limited memory and I/O delays.

Building and running program:

1. ``mkdir build && cd build``

2. ``cmake ..``

3. ``cmake --build .``

4. ``./tape-sorter ../input.bin ../output.bin`` - but there's a need to have files (input.bin, output.bin) already been created.

Building and running tests:

1. ``cmake .. -DBUILD_TESTS=ON``

2. ``cmake --build .``

3. ``./tape_tests``