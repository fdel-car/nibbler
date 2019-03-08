#!/bin/bash
ERROR="\033[1;31m"
WARNING="\033[1;33m"
RESET="\033[0m"

which python >> /dev/null || (echo "You need to have python installed first, you can use 'brew install python' for instance." || exit 0)
# pip install
which pip >> /dev/null
if [ $? == 1 ]; then
    curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
    python get-pip.py --user
    rm get-pip.py
	echo -e "${WARNING}In order to finish the install script you need to add the path to the pip binary to your PATH env variable.${RESET}"
    exit 0;
fi

# GLAD install
which glad >> /dev/null || pip install glad --user
if [[ ! -d "./libs/srcs/glad" && ! -d "./libs/includes/glad" && ! -d "./libs/includes/KHR" ]]; then
    glad --api gl=4.1 --profile=core --generator=c --out-path=tmp-glad
    mkdir -p ./libs/srcs/glad
    mv tmp-glad/src/glad.c ./libs/srcs/glad/glad.cpp
    mkdir -p ./libs/includes/glad
    mv tmp-glad/include/glad/glad.h ./libs/includes/glad
    mkdir -p ./libs/includes/KHR
    mv tmp-glad/include/KHR/khrplatform.h ./libs/includes/KHR
    rm -rf tmp-glad
fi

which brew >> /dev/null
if [ $? == 1 ]; then
	echo -e "${RED}Without brew you can't proceed further.${RESET}"
    exit 1;
else
    which pkg-config >> /dev/null || brew install pkg-config
    pkg-config --cflags glm >> /dev/null || brew install glm
    pkg-config --libs glfw3 >> /dev/null || brew install glfw3
    pkg-config --libs sfml-all >> /dev/null || brew install sfml
fi

LD_LIBRARY_PATH=`pkg-config --libs-only-L sfml-graphics | cut -c 3-`
cat ~/.zshrc | grep $LD_LIBRARY_PATH >> /dev/null
if [ $? == 1 ]; then
    echo export LD_LIBRARY_PATH=$LD_LIBRARY_PATH >> ~/.zshrc
    echo -e "${WARNING}You need to source your ~/.zshrc to run nibbler with the SFML.${RESET}"
fi
