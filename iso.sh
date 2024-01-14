#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/snotreeos.kernel isodir/boot/snotreeos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "snotreeos" {
	multiboot /boot/snotreeos.kernel
}
EOF
grub-mkrescue -o snotreeos.iso isodir
