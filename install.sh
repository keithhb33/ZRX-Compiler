#!/bin/bash

cd src
gcc zrx_compiler.c -o zrx_compiler
sudo mv zrx_compiler /usr/local/bin/zrx
sudo chmod +x /usr/local/bin/zrx

if [ -f "$HOME/.bashrc" ]; then
    profile_file="$HOME/.bashrc"
elif [ -f "$HOME/.zshrc" ]; then
    profile_file="$HOME/.zshrc"
elif [ -f "$HOME/.bash_profile" ]; then
    profile_file="$HOME/.bash_profile"
elif [ -f "$HOME/.zprofile" ]; then
    profile_file="$HOME/.zprofile"
elif [ -f "$HOME/.profile" ]; then
    profile_file="$HOME/.profile"
else
    echo "No suitable shell profile file found."
    echo "Creating a .bash_profile for PATH update."
    profile_file="$HOME/.bash_profile"
    touch "$profile_file"
fi

if [[ ":$PATH:" != *":/usr/local/bin:"* ]]; then
    echo 'export PATH=$PATH:/usr/local/bin' >> "$profile_file"
    echo 'PATH updated to include /usr/local/bin'
fi

source "$profile_file"

echo "Installation complete! You can now use the 'zrx' command from any terminal."
