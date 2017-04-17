if [ ! -d build ]
then
    mkdir build
    cd build
else
    cd build
    rm -r *
fi
cmake ../
