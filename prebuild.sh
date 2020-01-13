# check the executables
uname -a || { exit 1; }
cmake --version || { exit 1; }
g++ --version || { exit 1; }

# download external libraries
if [ -d "external" ]; then
    echo "Updating external libraries..."
else
    echo "Downloading external libraries..."
    mkdir external
fi

cd external

# googletest
if [ -d "googletest" ]; then
    echo "Updating googletest..."
    cd googletest
    git pull || {
        echo "Error updating googletest!"
        exit 1
    }
    echo "Library googletest updated."
    cd ..
else
    git clone https://github.com/google/googletest.git || {
        echo "Error downloading googletest!"
        exit 1
    }
    echo "Library googletest downloaded."
fi

