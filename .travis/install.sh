#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    brew install qt5
else
    sudo apt-get -y install qt58base qt58declarative qt58quickcontrols2 qt58svg qt58graphicaleffects
    source /opt/qt58/bin/qt58-env.sh
fi
