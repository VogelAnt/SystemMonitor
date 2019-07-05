#!/bin/bash
if [ ! -d "build" ]; then
    mkdir build    
fi

doxygen Doxyfile

cd build

python3 -m coverxygen --xml-dir documentation/xml --src-dir .. --output documentation-coverage.txt || { exit 1; }
genhtml --no-function-coverage --no-branch-coverage documentation-coverage.txt -o documentation-coverage || { exit 1; }
