Create new folder in C:\Documents named "vmshare"
Copy everything except for ubuntu iso file and virtual box exe into the new vmshare folder
Set up shared folder in VM settings, to connect C:\Documents\vmshare to /mnt/vmshare

Inside the VM open the terminal and write:
sudo mkdir /mnt/vmshare
sudo mount -t vboxsf vmshare /mnt/vmshare
cd /mnt/vmshare
./prep.sh

let it all compile
when it is done, restart the VM
in the vm search for "Malenia", right click > "add to favorites"
you are done

Whenever you want to update the code, update the USB
then, go to C:\Documents\vmshare and delete Malenia folder
copy updated Malenia folder from usb drive to C:\Documents\vmshare
In the vm, go to ~/Malenia/build
do:
qmake6 .
make

and you are done
