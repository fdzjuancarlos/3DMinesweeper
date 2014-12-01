#include <Ogre.h>
#include <OIS/OIS.h>
#include <string.h>
#include <CEGUI.h>
#include "Box.h"

#define STAGE 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

using namespace Ogre;
using namespace std;

//	mines::Box example = mines::Box();
//	mines::Box** board = example.createMatrix(10);
//	example.insertMine(board,10,20);
//	example.insertNumber(board,10);
//	example.printMatrix(board,10);

//, OIS::KeyListener, OIS::MouseListener

class MyFrameListener : public Ogre::FrameListener, OIS::KeyListener, OIS::MouseListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  Camera* _camera;
  RenderWindow* _win;
  OverlayManager* _overlayManager;
  SceneManager* _sceneManager;
  RaySceneQuery *_raySceneQuery;

  
  //Game Logic
  mines::Box* executionBox;
  mines::Box** board;
  Ogre::Timer timer;
  unsigned long seconds;
  int boardSize;
  int flags;
  int mines;
  int boxesToWin;
  bool initialized;
  bool emptyBoard;
  SceneNode *_selectedNode; 
  
  std::string *s_previousMaterial;
  std::string *s_previousCube;

  Ray setRayQuery(int posx, int posy, uint32 mask);
  bool keyPressed(const OIS::KeyEvent& evt);
  bool keyReleased(const OIS::KeyEvent& evt);
  bool mouseMoved(const OIS::MouseEvent& evt);
  bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
  
  void createBoard(Ogre::SceneManager* _sceneManager,Ogre::SceneNode* board, unsigned int size);
  void restartGame();
  
  bool _quit;
  float _timeSinceLastFrame;
  
  //string utils
  std::vector<std::string> split(const std::string &s, char delim);
  std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);
  void keepRecord(int seconds, int discoveredBoxes);
  std::vector<std::string> getRecords();
public:
  MyFrameListener(RenderWindow* win, Camera* cam, 
		  OverlayManager* om, SceneManager* sm, mines::Box** n_board);
  ~MyFrameListener();
  bool frameStarted(const FrameEvent& evt);
  void checkMatrix();  
  bool quit(const CEGUI::EventArgs &e);
  bool startGame(const CEGUI::EventArgs &e);
  void createVictoryLayout();
  void createDefeatLayout();
};
