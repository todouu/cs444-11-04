#!/bin/bash

echo "==============================="
echo "This script work only on Bash!!"
echo "   I am sourcing the script    "
#Start source
source /scratch/opt/environment-setup-i586-poky-linux
echo "         Source done           "
echo "       VM is booting...        "
echo "==============================="
#wait for user reading

sleep 1

#qemu-system-i386 -nographic -kernel bzImage-qemux86.bin -drive file=core-image-lsb-sdk-qemux86.ext3,if=virtio -enable-kvm -net none -usb -localtime --no-reboot --append "root=/dev/vda rw console=ttyS0 debug"

qemu-system-i386 -nographic -kernel linux-yocto-3.14/arch/x86/boot/bzImage -drive file=core-image-lsb-sdk-qemux86.ext3,if=virtio -enable-kvm -net none -usb -localtime --no-reboot --append "root=/dev/vda rw console=ttyS0 debug"

