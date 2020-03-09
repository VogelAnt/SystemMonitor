#!/bin/bash
if [ ! -d "build" ]; then
    mkdir build    
fi

doxygen Doxyfile

cd build

python3 -m coverxygen --xml-dir documentation/xml --src-dir .. --output documentation-coverage.txt || {
    echo "Could not generate documentation coverage info file."
    exit 1
}

genhtml --no-function-coverage --no-branch-coverage documentation-coverage.txt -o documentation-coverage || {
    echo "Could not generate html files from documentation coverage info file."
    exit 1
}

# # TODO add check if lcov exists
# lcov --summary coverxygenated.info || {
#     exit 1;
# }

cd ..
