#include <Ogre.h>
#include "MyFrameListener.h"
#include "Box.h"
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

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
  CEGUI::OgreRenderer* renderer; 
  
  void createBoard(Ogre::SceneManager* _sceneManager,Ogre::SceneNode* board, unsigned int size);

  
public:
  MyApp();
  ~MyApp();  
  int start();
  void loadResources();
  void createScene();
  void createOverlay();
  void createMenu();
  void createVictoryLayout();
  void createDefeatLayout();
   
  bool createRecordLayout(const CEGUI::EventArgs &e);
  bool watchCredit(const CEGUI::EventArgs &e);
  bool back(const CEGUI::EventArgs &e);
  bool back1(const CEGUI::EventArgs &e);
  void initUI();
};
