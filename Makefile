CXX=g++
INTELIB=../intelib
TRANS=$(INTELIB)/prolog/pl/trans.pro
CXXFLAGS_WARN=-W -Wall -Wno-extra -Wno-unused-parameter -Wno-unused-function -W-no-unused-variable 
CXXFLAGS=$(CXXFLAG_WARN) -O0 -g -std=c++0x -DDEBUG -I$(INTELIB) -L$(INTELIB)/build/intelib
MODULES=reversi.o matrix.o gamefield.o player.o randbot.o plgbot.o bot.o
LIBRARIES=-lSDL -lSDL_gfx -lintelib

.PHONY: build clean default

default: build

build: reversi

run: build
	./reversi

reversi: $(MODULES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBRARIES)

$(MODULES): %.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

gamefield.hpp: matrix.hpp reversi.hpp

reversi.cpp: gamefield.hpp player.hpp randbot.hpp plgbot.hpp

player.hpp: gamefield.hpp

player.cpp: reversi.hpp

randbot.hpp: player.hpp gamefield.hpp

plgbot.cpp: bot.hpp

bot.hpp: bot.cpp

bot.cpp: bot.pro
	prolog -qs $(TRANS) -t "translate('$<')"

clean:
	rm -f reversi $(MODULES)
