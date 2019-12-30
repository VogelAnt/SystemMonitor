#!/bin/bash
BUILD_TESTS_OPTION=OFF

for i in "$@"; do
    case $i in
    --build-tests)
        BUILD_TESTS_OPTION=ON
        ;;
    -f|--force-rebuild)
        if [ -d "build" ]; then
            rm -rf "build"
        fi
        ;;
    *)
        echo "Unknown argument $i"
        exit 1
    esac
done

if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake -DSMG_BUILD_TESTS=$BUILD_TESTS_OPTION .. || {
    echo "CMake failed."
    exit 1
}
make -j$(nproc) || { exit 1; }
cd ..