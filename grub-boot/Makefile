alpacos.iso:
	mkdir -p isodir/boot/grub
	cp ../kernel/alpacos.bin isodir/boot/alpacos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o alpacos.iso isodir

clean:
	rm -rf isodir alpacos.iso