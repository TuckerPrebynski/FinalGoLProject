ncurses:
	cd lib/ncurses-master && chmod +x configure &&./configure && make
csciGOL: ncurses
	g++ mainGame.cpp BoardDriver.cpp interfaceDriver.cpp gameDriver.cpp -lncurses -DNCURSES_STATIC
    