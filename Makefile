all:
	sdcc -mmcs51 prog.c
	packihx ./prog.ihx > prog.hex
	sudo python /home/f0/51/stcgal/stcgal.py -P stc89 -p /dev/ttyUSB0 ./prog.hex
	make clean
packihx:
	packihx ./prog.ihx > prog.hex
download:
	sudo python /home/f0/51/stcgal/stcgal.py -P stc89 -p /dev/ttyUSB0 ./prog.hex
clean:
	mv prog.c pro.c
	rm prog.*
	mv pro.c prog.c	
