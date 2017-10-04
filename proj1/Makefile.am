ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h BallApplication.h Ball.h BallWorld.h ../SimpleEngine/SimpleEngineWorld.h ../SimpleEngine/SimpleEngineGameObject.h ../SimpleEngine/SimpleEngineCollisionEvent.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -std=c++11 -I$(top_srcdir)
OgreApp_SOURCES= BaseApplication.cpp BallApplication.cpp Ball.cpp BallWorld.cpp ../SimpleEngine/SimpleEngineWorld.cpp ../SimpleEngine/SimpleEngineGameObject.cpp ../SimpleEngine/SimpleEngineCollisionEvent.cpp
OgreApp_CXXFLAGS= -I/usr/include/OGRE -pthread $(OIS_CFLAGS)
OgreApp_LDADD= -L/usr/lib/x86_64-linux-gnu/OGRE-1.9.0 -lOgreMain -lSDL -lSDL_mixer -lpthread $(OIS_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
