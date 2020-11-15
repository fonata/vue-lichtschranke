build/web-socket-server: web-socket-server.cpp
	mkdir -p build
	cd build; cmake ..
	cd build; make

all: clone-dependencies install-boost build

build-Simple-WebSocket-Server:
	cd Simple-WebSocket-Server; printf 1:; 	pwd; mkdir -p build
	cd Simple-WebSocket-Server/build; cmake ..
	cd Simple-WebSocket-Server/build; make

install-boost:
	sudo apt-get install libboost-all-dev

clone-dependencies:
	if [[ ! --d Simple-WebSocket-Server ]]; then git clone https://gitlab.com/eidheim/Simple-WebSocket-Server.git; fi

clean:
	rm -rf build
