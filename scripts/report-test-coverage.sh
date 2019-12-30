if [ ! -d build ]; then
    ./scripts/build.sh --build-tests
fi

cd build/unit_test
./unit-test

# exclude moc files and source files of unrelated code, including the tests themselves.
gcovr -r ../.. \
--exclude ../../external/ \
--exclude ../../unit_test/ \
|| {
    exit 1
}
