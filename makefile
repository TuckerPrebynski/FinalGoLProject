csciGOL: ncurses
	g++ mainGame.cpp BoardDriver.cpp interfaceDriver.cpp gameDriver.cpp -lncurses -DNCURSES_STATIC
ncurses:
	cd lib/ncurses-master && chmod +x configure &&./configure && make
    