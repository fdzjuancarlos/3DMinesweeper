#include <Ogre.h>
#include "MyFrameListener.h"


class MyApp {
  
private:
  SceneManager* _sceneManager;
  Root* _root;
  MyFrameListener* _framelistener;
  OverlayManager* _overlayManager;
  
  void createBoard(Ogre::SceneManager* _sceneManager,Ogre::SceneNode* board, unsigned int size);

  
public:
  MyApp();
  ~MyApp();  
  int start();
  void loadResources();
  void createScene();
  void createOverlay();
};
