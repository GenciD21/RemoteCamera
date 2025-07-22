build_all: compile_main flash_esp run

gurt:
	cd ~/esp/esp-idf && ./install.sh && . ./export.sh

monitor:
	cd C:\Users\GenciDuraku\new_opnecv_projects\RemoteCamera\testing && idf.py monitor -p COM3 -b 115200

