#include <Ogre.h>
#include <OIS/OIS.h>

#define STAGE 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

using namespace Ogre;
using namespace std;

class MyFrameListener : public FrameListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  Camera* _camera;
  RenderWindow* _win;
  OverlayManager* _overlayManager;
  SceneManager* _sceneManager;
  RaySceneQuery *_raySceneQuery;
  SceneNode *_selectedNode;

  Ray setRayQuery(int posx, int posy, uint32 mask);

public:
  MyFrameListener(RenderWindow* win, Camera* cam, 
		  OverlayManager* om, SceneManager* sm);
  ~MyFrameListener();
  bool frameStarted(const FrameEvent& evt);  
};
