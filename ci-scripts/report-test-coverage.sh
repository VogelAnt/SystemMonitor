if [ ! -d build ]; then
    ./scripts/build.sh --build-tests
fi

cd build/test
./unit-test

# exclude moc files and source files of unrelated code, including the tests themselves.
gcovr -r ../.. \
--exclude ../source/__/ \
--exclude ../source/system-monitor-gui_autogen/ \
--exclude ../external/ \
--exclude ../../external/ \
--exclude ../../test/ \
|| {
    exit 1
}
