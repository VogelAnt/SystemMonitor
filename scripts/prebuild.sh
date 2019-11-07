if [ ! -d external ]; then
    mkdir external
fi

cd external

# opcua-server-c (reusing their structure)
if [ ! -d "opc-ua-server-c" ]; then
    git clone https://gitex.itq.de/smart4i_v3/mocks/opc-ua-server-c.git
else
    cd "opc-ua-server-c"
    git pull
    cd ..
fi

exit # delete this line after testing is done

# redistorium
if [ ! -d redistorium ]; then
	git clone https://gitex.itq.de/smart4i_v3/libraries/redistorium.git
else
    cd redistorium
    git pull
    cd ..
fi
cd redistorium
./ci-scripts/prebuild.sh
qmake && make
cd ..

# nlohmann's JSON library
if [ ! -d json ]; then
    git clone https://github.com/nlohmann/json.git
else
    cd json
    git pull
    cd ..
fi



# open62541 (OPC-UA)
rm -rf open62541/
mkdir open62541
curl -L https://github.com/open62541/open62541/releases/download/v0.3.0/open62541-linux64.tar.gz -o open62541-v0.3.0.tar.gz
tar -xzvf open62541-v0.3.0.tar.gz -C open62541
