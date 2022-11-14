#!/bin/bash
sudo apt update
sudo apt install vim gcc perl make qt6-base-dev qt6-tools-dev qt6-multimedia-dev libpqxx-dev
sudo apt install gcc-10 gcc-10-base gcc-10-doc g++-10
sudo apt install libstdc++-10-dev libstdc++-10-doc g++

sudo mkdir -p /mnt/vmshare
sudo mount -t vboxsf vmshare /mnt/vmshare
echo "vmshare   /mnt/vmshare   vboxsf   defaults  0   0" | sudo tee -a /etc/fstab

ln -s /mnt/vmshare/Malenia $HOME/Malenia
cp Malenia.desktop $HOME/.local/share/applications/
sudo cp libvmshare.conf /etc/ld.so.conf.d/
sudo ldconfig

echo "Exec=$HOME/Malenia/build/Malenia" >> ~/.local/share/applications/Malenia.desktop
echo "Path=$HOME/Malenia/build" >> ~/.local/share/applications/Malenia.desktop
echo "Icon=$HOME/Malenia/build/assets/malenia-menu-icon.png" >> ~/.local/share/applications/Malenia.desktop
cd ~/Malenia/build
qmake6 .
make

