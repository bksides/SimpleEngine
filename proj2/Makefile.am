ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h PongApplication.h Wall.h PongBall.h ../SimpleEngine/SimpleEngineWorld.h ../SimpleEngine/SimpleEngineGameObject.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -std=c++11 -I$(top_srcdir)
OgreApp_SOURCES= BaseApplication.cpp PongApplication.cpp Wall.cpp PongBall.cpp ../SimpleEngine/SimpleEngineGameObject.cpp ../SimpleEngine/SimpleEngineWorld.cpp
OgreApp_CXXFLAGS= -I/usr/include/OGRE -pthread $(OIS_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(BULLET_CFLAGS)
OgreApp_LDADD= -L/usr/lib/x86_64-linux-gnu/OGRE-1.9.0 -lOgreMain -lSDL -lSDL_mixer -lpthread $(OIS_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(BULLET_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system
#CEGUI_CFLAGS = -pthread -I/lusr/opt/cegui-0.8.4/include/cegui-0 -I/usr/include/OGRE
#CEGUI_LIBS = -L/lusr/opt/cegui-0.8.4/lib -lCEGUIOgreRenderer-0 -lOgreMain -lpthread -lCEGUIBase-0
BULLET_CFLAGS = -I/usr/include/bullet/
BULLET_LIBS = -L/usr/lib/x86_64-linux-gnu/BulletSoftBody -L/usr/lib/x86_64-linux-gnu/BulletDynamics -L/usr/lib/x86_64-linux-gnu/BulletCollision -L/usr/lib/x86_64-linux-gnu/LinearMath -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath


EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
