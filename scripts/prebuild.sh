rm -rf external/
mkdir external
cd external

# download and build redistorium
git clone https://gitex.itq.de/smart4i_v3/libraries/redistorium.git
cd redistorium
./ci-scripts/prebuild.sh
qmake && make
cd ..

# download and build JSON for modern C++
git clone https://github.com/nlohmann/json.git
