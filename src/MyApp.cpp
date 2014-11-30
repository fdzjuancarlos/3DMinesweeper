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
#include "Box.h"
#include <stdlib.h>

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
	cam->setPosition(Vector3(5.98, 8.73, -17.09));
	cam->lookAt(Vector3(0, -15, 0));
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

	mines::Box example = mines::Box();
	mines::Box** board = example.createMatrix(10);

	_framelistener = new MyFrameListener(window, cam, _overlayManager, _sceneManager, board);
	_root->addFrameListener(_framelistener);

	createMenu();

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
	
	Ogre::Entity* ent1 = _sceneManager->createEntity("cube.mesh");
	ent1->setQueryFlags(STAGE);
	Ogre::SceneNode* node1 = _sceneManager->createSceneNode("BoardNode");
	node1->attachObject(ent1);
	_sceneManager->getRootSceneNode()->addChild(node1);
	node1->setScale(11,2,11);
	node1->setPosition(0,-15,0);

	//Main Board
	createBoard(_sceneManager, node1, 10);

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
  
  Overlay *roverlay = _overlayManager->getByName("Records");
  roverlay->show();

}

void MyApp::createBoard(Ogre::SceneManager* _sceneManager,Ogre::SceneNode* board, unsigned int size){
	
	float relativeXPos;
	float relativeZPos;
	float relativeSize = 1.0/((float)(size+1));

	for (unsigned int i = 0; i < size; i += 1){
		for (unsigned int j = 0; j < size; j += 1){

			Ogre::Entity* cube = _sceneManager->createEntity("cube.mesh");
			cube->setQueryFlags(i?CUBE1:CUBE2);
			std::ostringstream stringStream;
			stringStream << "SquareNode_" << i << "_" << j;
			std::string name = stringStream.str();
			Ogre::SceneNode* node = _sceneManager->createSceneNode(name);
			node->attachObject(cube);
			board->addChild(node);
			cube->setMaterialName("cube_2");

			
			
			node->setScale(relativeSize*0.8, 1 , relativeSize*0.8);
			
			relativeXPos = 1 - (relativeSize)*(2*i) - (relativeSize*2);
			relativeZPos = 1 - (relativeSize)*(2*j) - (relativeSize*2);
			
			node->setPosition(relativeXPos, 1, relativeZPos);
		}
	}
}

void MyApp::createMenu(){
	//CEGUI

	renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("stringFormat.layout");

	//Setting Text!
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text1")->setText(" [vert-alignment='centre']Minesweeper 3D");

	//Game Window
	CEGUI::Window* gameButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/GameButton");
	gameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::startGame, _framelistener));

	//Record Window
	CEGUI::Window* recordButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/RecordButton");
	recordButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::watchScores, _framelistener));

	//Credit Window
	CEGUI::Window* creditButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/CreditButton");
	creditButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::watchCredits, _framelistener));

	//Exit Window
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::quit, _framelistener));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}
/*
void MyApp::createVictoryLayout(){
	//CEGUI

	renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("victoryWin.layout");

	//Setting Text!
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text1")->setText(" [vert-alignment='centre']¡Enhorabuena, has ganado!");

	//Game Window
	CEGUI::Window* gameButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/GameButton");
	gameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::startGame, _framelistener));

	//Record Window
	CEGUI::Window* recordButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/RecordButton");
	recordButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::watchScores, _framelistener));

	//Exit Window
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::quit, _framelistener));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}

void MyApp::createDefeatLayout(){
	//CEGUI

	renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("defeatWin.layout");

	//Setting Text!
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text1")->setText(" [vert-alignment='centre']¡Has perdido!");

	//Game Window
	CEGUI::Window* gameButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/GameButton");
	gameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::startGame, _framelistener));

	//Exit Window
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MyFrameListener::quit, _framelistener));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}*/
