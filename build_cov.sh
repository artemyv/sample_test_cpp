#!/bin/sh
set -e

rm -rf build
mkdir -p build
cmake -B build -G Ninja -DCODE_COVERAGE=ON -DSAMPLE_CPP_BUILD_TESTING=ON
cmake --build build
cd build
ctest --output-on-failure
lcov --capture --directory . --output-file coverage.info --ignore-errors gcov --ignore-errors mismatch
lcov --remove coverage.info '/usr/*' '*/_deps/*' --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
cd ..
