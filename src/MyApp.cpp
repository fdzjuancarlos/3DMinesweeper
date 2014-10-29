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

#include "MyApp.h" 

MyApp::MyApp() {
  _sceneManager = NULL;
  _framelistener = NULL;
}

MyApp::~MyApp() {
  delete _root;
  delete _framelistener;
}

int MyApp::start() {
  _root = new Ogre::Root();
  
  if(!_root->restoreConfig()) {
    _root->showConfigDialog();
    _root->saveConfig();
  }
  
  Ogre::RenderWindow* window = _root->initialise(true,"MyApp Example");
  _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC, "PlayScene");
  
  Ogre::Camera* cam = _sceneManager->createCamera("MainCamera");
  cam->setPosition(Ogre::Vector3(5,20,20));
  cam->lookAt(Ogre::Vector3(0,0,0));
  cam->setNearClipDistance(5);
  cam->setFarClipDistance(10000);
  
  Ogre::Viewport* viewport = window->addViewport(cam);
  viewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));
  double width = viewport->getActualWidth();
  double height = viewport->getActualHeight();
  cam->setAspectRatio(width / height);
  
  loadResources();
  createScene();

  Ogre::SceneNode *node = _sceneManager->getSceneNode("BoardNode");
  
  _framelistener = new MyFrameListener(window, cam, node, _root);
  _root->addFrameListener(_framelistener);
  
  _root->startRendering();
  return 0;
}

void MyApp::loadResources() {
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");
  
  Ogre::ConfigFile::SectionIterator sI = cf.getSectionIterator();
  Ogre::String sectionstr, typestr, datastr;
  while (sI.hasMoreElements()) {
    sectionstr = sI.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = sI.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typestr = i->first;    datastr = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation
            (datastr, typestr, sectionstr);	
    }
  }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApp::createScene() {
  Ogre::Entity* ent1 = _sceneManager->createEntity("cube.mesh");
  Ogre::SceneNode* node1 = _sceneManager->createSceneNode("BoardNode");
  node1->attachObject(ent1);
  _sceneManager->getRootSceneNode()->addChild(node1);
  node1->setScale(11,2,11);
  node1->setPosition(0,-15,0);
  
  	//firstSquare
    Ogre::Entity* square1 = _sceneManager->createEntity("cube.mesh");
	Ogre::SceneNode* n_square1 = _sceneManager->createSceneNode("Square1Node");
	n_square1->attachObject(square1);
	node1->addChild(n_square1);
	//Lets supose we have a 8x8 board
	//Lets supose that 0.2 of general width will be the "gap"
	// 1/9 = 0.111
	
	float relativePos = 1 - (1.0/9.0) - (1.0/9.0);
	
	n_square1->setScale(0.111,1,0.111);
	n_square1->setPosition(relativePos,1,relativePos);
	
	//secondSquare
    Ogre::Entity* square2 = _sceneManager->createEntity("cube.mesh");
	Ogre::SceneNode* n_square2 = _sceneManager->createSceneNode("Square2Node");
	n_square2->attachObject(square2);
	node1->addChild(n_square2);
	//Lets supose we have a 8x8 board
	//Lets supose that 0.2 of general width will be the "gap"
	// 1/9 = 0.111
	
	float relativeXPos = 1 - (1.0/9.0) - (1.0/9.0);
	float relativeZPos = 1 - (1.0/9.0)*4 - (1.0/9.0) ;
	//+ (1.0/9.0);
	
	n_square2->setScale(0.111,1,0.111);
	n_square2->setPosition(relativeXPos,1,relativeZPos);



  Ogre::SceneNode* node2 = _sceneManager->createSceneNode("light");
  _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);	
  Ogre::Light* light = _sceneManager->createLight("Light1");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1,-1,0));
  node2->attachObject(light);

  _sceneManager->getRootSceneNode()->addChild(node2);
}
