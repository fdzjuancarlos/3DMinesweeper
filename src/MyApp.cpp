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
  _root = new Root();
  
  if(!_root->restoreConfig()) {
    _root->showConfigDialog();
    _root->saveConfig();
  }
  
  RenderWindow* window = _root->initialise(true,"MyApp Example");
  _sceneManager = _root->createSceneManager(ST_INTERIOR);
  
  Camera* cam = _sceneManager->createCamera("MainCamera");
  cam->setPosition(Vector3(5.98, 2.73, 7.09));
  cam->lookAt(Vector3(-3.45,1.45,2.69));
  cam->setNearClipDistance(0.1);
  cam->setFarClipDistance(100);
  
  Viewport* viewport = window->addViewport(cam);
  viewport->setBackgroundColour(ColourValue(0.0,0.0,0.0));
  double width = viewport->getActualWidth();
  double height = viewport->getActualHeight();
  cam->setAspectRatio(width / height);
  
  loadResources();
  createScene();
  createOverlay();

  _framelistener = new MyFrameListener(window, cam, _overlayManager, _sceneManager);
  _root->addFrameListener(_framelistener);
  
  _root->startRendering();
  return 0;
}

void MyApp::loadResources() {
  ConfigFile cf;
  cf.load("resources.cfg");
  
  ConfigFile::SectionIterator sI = cf.getSectionIterator();
  String sectionstr, typestr, datastr;
  while (sI.hasMoreElements()) {
    sectionstr = sI.peekNextKey();
    ConfigFile::SettingsMultiMap *settings = sI.getNext();
    ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typestr = i->first;    datastr = i->second;
      ResourceGroupManager::getSingleton().addResourceLocation
            (datastr, typestr, sectionstr);	
    }
  }
  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApp::createScene() {
  // Creamos la geometria estatica del escenario
  StaticGeometry* stage = _sceneManager->createStaticGeometry("Stage");
  Entity* ent1 = _sceneManager->createEntity("Escenario.mesh");
  stage->addEntity(ent1, Vector3(0,0,0));
  stage->build();  // Operacion para construir la geometria
  
  // Objeto movable "suelo" para consultar al SceneManager
  SceneNode *nodecol = _sceneManager->createSceneNode("Col_Suelo");
  Entity *entcol = _sceneManager->createEntity("Col_Suelo", "Col_Suelo.mesh");
  entcol->setQueryFlags(STAGE);   // Usamos flags propios!
  nodecol->attachObject(entcol);
  nodecol->setVisible(false);     // Objeto oculto
  _sceneManager->getRootSceneNode()->addChild(nodecol);

  // Cajas del escenario (baja poligonalizacion) 
  std::stringstream sauxnode, sauxmesh;
  string s = "Col_Box";
  for (int i=1; i<6; i++) {
    sauxnode << s << i; sauxmesh << s << i << ".mesh";
    SceneNode *nodebox = _sceneManager->createSceneNode(sauxnode.str());
    Entity *entboxcol = _sceneManager->createEntity(sauxnode.str(), sauxmesh.str());
    entboxcol->setQueryFlags(STAGE);    // Escenario
    nodebox->attachObject(entboxcol);
    nodebox->setVisible(false);
    nodecol->addChild(nodebox);
    sauxnode.str(""); sauxmesh.str(""); // Limpiamos el stream
  }

  SceneNode* node2 = _sceneManager->createSceneNode("Luces");
  _sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);	
  Light* light = _sceneManager->createLight("Light1");
  light->setType(Light::LT_SPOTLIGHT);
  light->setPosition(9,2,5);
  light->setDirection(Vector3(3, -2, 0));
  light->setSpotlightInnerAngle(Degree(5.0f));
  light->setSpotlightOuterAngle(Degree(55.0f));
  light->setSpotlightFalloff(0.0f);
  node2->attachObject(light);
  _sceneManager->getRootSceneNode()->addChild(node2);
}

void MyApp::createOverlay() {
  _overlayManager = OverlayManager::getSingletonPtr();
  Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}
