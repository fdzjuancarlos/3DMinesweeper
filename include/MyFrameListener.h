#include <Ogre.h>
#include <OIS/OIS.h>

class MyFrameListener : public Ogre::FrameListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  Ogre::Camera* _camera;
  Ogre::SceneNode *_node;

public:
  MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, 
		  Ogre::SceneNode* node);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);  
};
