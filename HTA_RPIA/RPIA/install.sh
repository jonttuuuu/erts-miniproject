#!/bin/bash

# this script installs all required packages for the RPIA
echo "Installing RPIA"

# install python packages
echo "Installing python packages"
pip install -r requirements.txt

# install npm
echo "Installing npm"
sudo apt-get install npm

# install web packages
echo "Installing web packages"
cd web/
sudo npm install
sudo npm install -g serve

echo "RPIA installation complete!!"
