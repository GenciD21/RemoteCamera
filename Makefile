build_all: compile_main flash_esp run

run_main:
	cmake --build build
	./build/Debug/my_app.exe
	
run:
	./build/debug/my_app.exe

monitor:
	cd C:\Users\GenciDuraku\new_opnecv_projects\RemoteCamera\testing && idf.py monitor -p COM3 -b 115200 
