#include <Ogre.h>
#include "MyFrameListener.h"

class MyApp {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  MyFrameListener* _framelistener;
  
  void createBoard(Ogre::SceneManager* _sceneManager,Ogre::SceneNode* board, unsigned int size);
  
public:
  MyApp();
  ~MyApp();  
  int start();
  void loadResources();
  void createScene();
};
