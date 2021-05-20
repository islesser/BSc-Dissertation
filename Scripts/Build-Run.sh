./vendor/bin/premake/mac/premake5 gmake2
make -j8 verbose=1 config=debug
echo -e "\n\n\n\nBuild completed.\n\n\n\n"
./bin/Debug-macosx-x86_64/Tidal/Tidal