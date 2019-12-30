CI_PREFIX=""
for i in "$@"; do
    case $i in
    --ci-prefix=*)
        CI_PREFIX="${i#*=}"
        ;;
    -f|--force)
        if [ -d external ]; then
            rm -rf external
        fi
        ;;
    *)
        echo "Unknown argument $i" && exit 1
    esac
done

if [ ! -d external ]; then
    mkdir external
fi
cd external



# googletest
if [ ! -d googletest ]; then
    curl -L -o googletest.zip https://github.com/google/googletest/archive/release-1.8.1.zip
    unzip googletest.zip
    mv googletest-release-1.8.1 googletest
    rm -f googletest.zip
fi



# nlohmann's JSON library
if [ ! -d json ]; then
    mkdir json
    cd json
    echo "Downloading nlohmann json..."
    curl -L -o json.zip https://github.com/nlohmann/json/releases/download/v3.7.3/include.zip
    unzip json.zip
    rm -f json.zip
    cd ..
fi



# open62541
if [ ! -d open62541 ]; then
    echo "Downloading open62541..."
	git clone https://github.com/open62541/open62541.git || {
        echo "Could not download open62541." && exit 1
    }
else
    cd open62541
    echo "Updating open62541..." && git pull || {
        echo "Could not update open62541." && exit 1
    }
    cd ..
fi
cd open62541
if [ ! -d build ]; then
    mkdir build
fi
cd build
cmake -DBUILD_SHARED_LIBS=ON -DUA_ENABLE_AMALGAMATION=ON .. || { exit 1; }
make || { exit 1; }
cd ../..



# redistorium
if [ ! -d redistorium ]; then
    echo "Downloading redistorium..."
	git clone https://${CI_PREFIX}gitex.itq.de/smart4i_v3/libraries/redistorium.git || {
        echo "Could not download redistorium." && exit 1
    }
else
    cd redistorium
    echo "Updating redistorium..." && git pull || {
        echo "Could not update redistorium." && exit 1
    }
    cd ..
fi
cd redistorium
./ci-scripts/build.sh || { exit 1; }
cd ..
