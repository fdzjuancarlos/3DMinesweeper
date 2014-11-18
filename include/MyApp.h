#include <Ogre.h>
#include "MyFrameListener.h"
#include "Box.h"

//	mines::Box example = mines::Box();
//	mines::Box** board = example.createMatrix(10);
//	example.insertMine(board,10,20);
//	example.insertNumber(board,10);
//	example.printMatrix(board,10);

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
