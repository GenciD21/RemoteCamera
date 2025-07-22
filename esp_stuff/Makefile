build_flash:
	idf.py set-target esp32s3
	idf.py build
	idf.py -p /dev/ttyACM0 flash

check_port:
	idf.py monitor -p /dev/ttyACM0 -b 115200