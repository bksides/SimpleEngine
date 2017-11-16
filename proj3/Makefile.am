ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h RaceApplication.h RaceWorld.h FloorTile.h ../SimpleEngine/SimpleEngineWorld.h ../SimpleEngine/SimpleEngineGameObject.h ../SimpleEngine/SimpleEngineCollisionEvent.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -std=c++11 -I$(top_srcdir)
OgreApp_SOURCES= BaseApplication.cpp RaceApplication.cpp RaceWorld.cpp FloorTile.cpp ../SimpleEngine/SimpleEngineWorld.cpp ../SimpleEngine/SimpleEngineGameObject.cpp ../SimpleEngine/SimpleEngineCollisionEvent.cpp
OgreApp_CXXFLAGS= -I/usr/include/OGRE -pthread $(OIS_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(CEGUIOGRE_CFLAGS) $(BULLET_CFLAGS)
OgreApp_LDADD= -L/usr/lib/x86_64-linux-gnu/OGRE-1.9.0 -lOgreMain -lSDL -lSDL_mixer -lSDL_net -lpthread $(OIS_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(CEGUIOGRE_LIBS) $(BULLET_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system
CEGUIOGRE_CFLAGS= -pthread -I/lusr/opt/cegui-0.8.7/include/cegui-0 -I/usr/include/OGRE
CEGUIOGRE_LIBS= -L/lusr/opt/cegui-0.8.7/lib -lCEGUIOgreRenderer-0 -lOgreMain -lpthread -lCEGUIBase-0 
BULLET_CFLAGS = -I/usr/include/bullet/
BULLET_LIBS = -L/usr/lib/x86_64-linux-gnu/BulletSoftBody -L/usr/lib/x86_64-linux-gnu/BulletDynamics -L/usr/lib/x86_64-linux-gnu/BulletCollision -L/usr/lib/x86_64-linux-gnu/LinearMath -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
