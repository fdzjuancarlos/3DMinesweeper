/*********************************************************************
 * Módulo 2. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Carlos González Morcillo     Carlos.Gonzalez@uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/
#include "MyFrameListener.h"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, 
				 Ogre::Camera* cam, 
				 Ogre::SceneNode *node,
				 Ogre::Root* root) {
  OIS::ParamList param;
  size_t windowHandle;  std::ostringstream wHandleStr;

  _camera = cam;  _node = node; _root = root; 
  
  _sceneManager = _root->getSceneManager("PlayScene");
  
  win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));
  
  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*>
    (_inputManager->createInputObject(OIS::OISKeyboard, false));
  _mouse = static_cast<OIS::Mouse*>
    (_inputManager->createInputObject(OIS::OISMouse, false));
}

MyFrameListener::~MyFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  OIS::InputManager::destroyInputSystem(_inputManager);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  Ogre::Vector3 vt(0,0,0);     Ogre::Real tSpeed = 20.0;  
  Ogre::Real r = 0;
  Ogre::Real deltaT = evt.timeSinceLastFrame;

  _keyboard->capture();
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
  if(_keyboard->isKeyDown(OIS::KC_UP))    vt+=Ogre::Vector3(0,0,-1);
  if(_keyboard->isKeyDown(OIS::KC_DOWN))  vt+=Ogre::Vector3(0,0,1);
  if(_keyboard->isKeyDown(OIS::KC_LEFT))  vt+=Ogre::Vector3(-1,0,0);
  if(_keyboard->isKeyDown(OIS::KC_RIGHT)) vt+=Ogre::Vector3(1,0,0);
  if(_keyboard->isKeyDown(OIS::KC_X)){
  	Ogre::SceneNode *boardNode = _sceneManager->getSceneNode("BoardNode");
  	Ogre::Entity* boardEntity = static_cast<Ogre::Entity*>(boardNode->getAttachedObject(0));
  	
  	Ogre::AxisAlignedBox charAABB = boardEntity->getWorldBoundingBox();
	Ogre::Vector3 min = charAABB.getMinimum();
	Ogre::Vector3 max = charAABB.getMaximum();
	//Ogre::Vector3 center = charAABB.getCenter();
	Ogre::Vector3 size( fabs( max.x - min.x), fabs( max.y - min.y), fabs( max.z - min.z ) );
  	std::cout << size[0] << " " << size[1] << " "<< size[2] <<std::endl;
  	
  	//Create square

  }
  _camera->moveRelative(vt * deltaT * tSpeed);

  if(_keyboard->isKeyDown(OIS::KC_R)) r+=180;
  _node->yaw(Ogre::Degree(r * deltaT));
  
  _mouse->capture();
  float rotx = _mouse->getMouseState().X.rel * deltaT * -1;
  float roty = _mouse->getMouseState().Y.rel * deltaT * -1;
  _camera->yaw(Ogre::Radian(rotx));
  _camera->pitch(Ogre::Radian(roty));

  return true;
}
