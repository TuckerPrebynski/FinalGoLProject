Final project 
Tucker Prebynski

The game needs the external library ncurses, which is included under /lib

To compile the game, run "make" in the main project directory
then run the created executable (either a.out, or a.exe)

Alternatively:
run:
  cd lib/ncurses-master && chmod +x configure &&./configure && make
then:
	g++ mainGame.cpp BoardDriver.cpp interfaceDriver.cpp gameDriver.cpp -lncurses -DNCURSES_STATIC
	
