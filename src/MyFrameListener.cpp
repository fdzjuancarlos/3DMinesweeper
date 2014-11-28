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
#include <stdlib.h>

MyFrameListener::MyFrameListener(RenderWindow* win, Camera* cam, 
OverlayManager *om,SceneManager *sm, mines::Box** n_board){
  OIS::ParamList param; size_t windowHandle;  ostringstream wHandleStr;

  _camera = cam;  _overlayManager = om;
  _sceneManager = sm; _win = win;

  srand((unsigned)time(NULL));   // Semilla aleatorios
  _win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(make_pair("WINDOW", wHandleStr.str()));

  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*>
    (_inputManager->createInputObject(OIS::OISKeyboard, false));
  _mouse = static_cast<OIS::Mouse*>
    (_inputManager->createInputObject(OIS::OISMouse, false));
    
      _keyboard->setEventCallback(this);
  _mouse->setEventCallback(this);
  _mouse->getMouseState().width = _win->getWidth();
  _mouse->getMouseState().height = _win->getHeight();

  _raySceneQuery = _sceneManager->createRayQuery(Ray());
  _selectedNode = NULL;
  s_previousMaterial = new std::string("");
  s_previousCube = new std::string("");
  executionBox = new mines::Box();

  //Game Logic Variables
  initialized=false;
  board=n_board;
  seconds=0;
  flags=0;
  _quit = false;
  mines = 15;
  boardSize=10;
  emptyBoard=false;

}

MyFrameListener::~MyFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  _sceneManager->destroyQuery(_raySceneQuery);
  OIS::InputManager::destroyInputSystem(_inputManager);
}

Ray MyFrameListener::setRayQuery(int posx, int posy, uint32 mask) {
  Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  _raySceneQuery->setQueryMask(mask);
  return (rayMouse);
}

bool MyFrameListener::frameStarted(const FrameEvent& evt) {
  Vector3 vt(0,0,0);    
  Real deltaT = evt.timeSinceLastFrame;
  bool mbleft, mbmiddle, mbright; // Botones del raton pulsados
  
  _timeSinceLastFrame = evt.timeSinceLastFrame;
  
  CEGUI::System::getSingleton().injectTimePulse(_timeSinceLastFrame);

  _keyboard->capture();  _mouse->capture();   // Captura eventos



  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.
  
  checkMatrix();
  
  //Update Time
	seconds = timer.getMilliseconds()/1000;
  
  //CEGUI Non-Callback
  //CEGUI::System::getSingleton().injectMouseMove(_mouse->getMouseState().X.rel, _mouse->getMouseState().Y.rel);
  //CEGUI::System::getSingleton().injectKeyUp(evt.key); 

  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;   // Exit!

  // Operaciones de rotacion para Board -------------------
	std::ostringstream stringStream;
	stringStream << "BoardNode";
	std::string name = stringStream.str();
	SceneNode* boardEntity = _sceneManager->getSceneNode(name);
  Real deltaTaux = deltaT;
  if(_keyboard->isKeyDown(OIS::KC_A)) 
    boardEntity->yaw(Degree(90)*deltaTaux);
  if(_keyboard->isKeyDown(OIS::KC_D)) 
    boardEntity->yaw(Degree(-90)*deltaTaux);
  if(_keyboard->isKeyDown(OIS::KC_RIGHT)) 
    boardEntity->yaw(Degree(90)*deltaTaux);
  if(_keyboard->isKeyDown(OIS::KC_LEFT)) 
    boardEntity->yaw(Degree(-90)*deltaTaux);	


  // Botones del raton pulsados? -------------------------------------
  mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
  mbmiddle = _mouse->getMouseState().buttonDown(OIS::MB_Middle);
  mbright = _mouse->getMouseState().buttonDown(OIS::MB_Right);
  
  if (mbmiddle) { // Con boton medio pulsado, rotamos camara ---------
    float rotx = _mouse->getMouseState().X.rel * deltaT * -1;
    float roty = _mouse->getMouseState().Y.rel * deltaT * -1;
    _camera->yaw(Radian(rotx));
    _camera->pitch(Radian(roty));
    cout << "Boton Medio" << endl;
  }
  
  uint32 mask;
  
  if (mbleft || mbright) {  // Boton izquierdo o derecho -------------
    if (mbleft) { // Variables y codigo especifico si es izquierdo
      mask = CUBE1 | CUBE2;
    }
    if (mbright) { // Variables y codigo especifico si es derecho
      mask = CUBE1 | CUBE2;
    }

    if (_selectedNode != NULL) {  // Si habia alguno seleccionado...
      _selectedNode->showBoundingBox(false);  _selectedNode = NULL;  
    }

    Ray r = setRayQuery(posx, posy, mask);
    RaySceneQueryResult &result = _raySceneQuery->execute();
    RaySceneQueryResult::iterator it;
    it = result.begin();

    if (it != result.end()) {
      if (mbleft && !emptyBoard) {
  		  _selectedNode = it->movable->getParentSceneNode();
  		  std::string str2 = _selectedNode->getName().substr(0,10);

    		if (str2.compare("SquareNode")==0){

      		std::string stri = std::string(_selectedNode->getName().substr(11,2));
      		std::string strj = std::string(_selectedNode->getName().substr(13,2));
      		

         	int i = atoi(stri.c_str());
      		int j = atoi(strj.c_str());
      		
      		if(!initialized){
      			initialized=true;
      			executionBox->firstTouch(board, boardSize, i, j);
      			executionBox->insertMine(board, boardSize, mines);
      			executionBox->insertNumber(board,boardSize);
      			executionBox->openEmptyBox(board,i,j,boardSize);
      			executionBox->openEmptyBox(board,i-1,j-1,boardSize);
      			executionBox->openEmptyBox(board,i-1,j,boardSize);
      			executionBox->openEmptyBox(board,i-1,j+1,boardSize);
      			executionBox->openEmptyBox(board,i,j-1,boardSize);
      			executionBox->openEmptyBox(board,i,j+1,boardSize);
      			executionBox->openEmptyBox(board,i+1,j-1,boardSize);
      			executionBox->openEmptyBox(board,i+1,j,boardSize);
      			executionBox->openEmptyBox(board,i+1,j+1,boardSize);
      			timer = Ogre::Timer();
      			
      		}else if(board[i][j].getState() == 0){
      			//mine::Box exec = mine::Box();
            if(board[i][j].getValue() == -1){
              executionBox->gameOver(board, i, j, boardSize);
              //restartGame();  //Descomentar para probar
            }else{
  				executionBox->openEmptyBox(board, i, j, boardSize);
            }
  				checkMatrix();
  			}			
      	}
      }
      if (mbright && !emptyBoard) {
          _selectedNode = it->movable->getParentSceneNode();
          std::string str2 = _selectedNode->getName().substr(0,10);
          
          if (str2.compare("SquareNode")==0){

            std::string stri = std::string(_selectedNode->getName().substr(11,2));
            std::string strj = std::string(_selectedNode->getName().substr(13,2));
        
            int i = atoi(stri.c_str());
            int j = atoi(strj.c_str());

            std::cout << "Cube " << i << " " << j << std::endl;
            if(board[i][j].getState() == 0){
              if(!executionBox->putFlag(board, i, j))
              	flags++;
              else
              	flags--;
              checkMatrix();
            }
          }     
        } 
       
    }
  }

  mask = CUBE1 | CUBE2; //STAGE |
  Ray r = setRayQuery(posx, posy, mask);
  RaySceneQueryResult &result = _raySceneQuery->execute();
  RaySceneQueryResult::iterator it;
  it = result.begin();
    

  if (it != result.end()) {
    _selectedNode = it->movable->getParentSceneNode();
    Entity* mEntity = static_cast<Entity*>(_selectedNode->getAttachedObject(0));

    if(s_previousCube->compare(_selectedNode->getName())){
    	
		if(s_previousCube->compare(std::string(""))){
			std::cout << *s_previousMaterial << std::endl;
	    	Entity* entity = static_cast<Entity*>(_sceneManager->getSceneNode(*s_previousCube)->getAttachedObject(0));
	    	entity->getSubEntity(0)->setMaterialName(*s_previousMaterial);
	    }
	    delete s_previousCube;
	    s_previousCube= new std::string(_selectedNode->getName());
	    delete s_previousMaterial;
	    s_previousMaterial = new std::string(mEntity->getSubEntity(0)->getMaterialName());
    }
    
    mEntity->setMaterialName("cube_selected");  
  }
  
  // Gestion del overlay ---------------------------------------------
  OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("segundos");
  std::ostringstream string;
  string << seconds;
  oe->setCaption(string.str());
  oe = _overlayManager->getOverlayElement("puntuacion");
  std::ostringstream stringFlags;
  stringFlags << flags;
  oe->setCaption(stringFlags.str());
  oe = _overlayManager->getOverlayElement("minas");
  std::ostringstream stringMines;
  stringMines << mines;
  oe->setCaption(stringMines.str());


  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);

  return true;
}

void MyFrameListener::checkMatrix(){
	
	int matrixToWin = (boardSize*boardSize) - mines;
	if(!emptyBoard){
		for ( int i = 0; i < boardSize; i += 1){
			for ( int j = 0; j < boardSize; j += 1){
			  	std::ostringstream stringStream;
				stringStream << "SquareNode_" << i << "_" << j;
				std::string name = stringStream.str();
				Entity* entity = static_cast<Entity*>(_sceneManager->getSceneNode(name)->getAttachedObject(0)); 
			
				std::ostringstream stringStream2;
			if(board[i][j].getState() == 0){
				if(board[i][j].getFlag() == true){
					stringStream2 << "flag";
		    	}else{
		    		stringStream2 << "undiscovered";
		    	}
			}else if(board[i][j].getValue() >=0){
				stringStream2 << "cube_" << board[i][j].getValue();
				matrixToWin--;
		  	}else{
				stringStream2 << "mine";
			}
			if(board[i][j].getNoFlag() == true){
		    	stringStream2 << "noflag";
			}
			std::string materialName = stringStream2.str();
			entity->getSubEntity(0)->setMaterialName(materialName);
			}	
		}
	}
	
	if(matrixToWin == 0 )
		std::cout << "tenemos ganador" << std::endl;
		//FIXME condicion de victoria a implementar
}

void MyFrameListener::restartGame(){
	
	//Delete previous board
	for ( int i = 0; i < boardSize; i += 1){
		for ( int j = 0; j < boardSize; j += 1){
			std::ostringstream stringStream;
			stringStream << "SquareNode_" << i << "_" << j;
			std::string name = stringStream.str();
			std::cout << name << std::endl;
			_sceneManager->destroySceneNode(name); 
		}
	}
	std::ostringstream stringStream;
	stringStream << "BoardNode";
	std::string name = stringStream.str();
	emptyBoard=true;
	
	_selectedNode=NULL;
	
	board = executionBox->createMatrix(boardSize);
	createBoard(_sceneManager, 	_sceneManager->getSceneNode(name), boardSize);
	initialized=false;
	
	emptyBoard=false;
	
}

void MyFrameListener::createBoard(Ogre::SceneManager* _sceneManager,Ogre::SceneNode* board, unsigned int size){
	
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

bool MyFrameListener::keyPressed(const OIS::KeyEvent& evt)
{
  if(evt.key==OIS::KC_ESCAPE) _quit=true;
 
  CEGUI::System::getSingleton().injectKeyDown(evt.key);
  CEGUI::System::getSingleton().injectChar(evt.text);

  return true;
}

bool MyFrameListener::keyReleased(const OIS::KeyEvent& evt)
{
  CEGUI::System::getSingleton().injectKeyUp(evt.key);
  return true;
}

bool MyFrameListener::mouseMoved(const OIS::MouseEvent& evt)
{
	std::cout << "se llama" << std::endl;
  CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
  return true;
}

bool MyFrameListener::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
  return true;
}

bool MyFrameListener::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
  return true;
}

CEGUI::MouseButton MyFrameListener::convertMouseButton(OIS::MouseButtonID id)
{
  CEGUI::MouseButton ceguiId;
  switch(id)
    {
    case OIS::MB_Left:
      ceguiId = CEGUI::LeftButton;
      break;
    case OIS::MB_Right:
      ceguiId = CEGUI::RightButton;
      break;
    case OIS::MB_Middle:
      ceguiId = CEGUI::MiddleButton;
      break;
    default:
      ceguiId = CEGUI::LeftButton;
    }
  return ceguiId;
}


bool MyFrameListener::quit(const CEGUI::EventArgs &e)
{
  _quit = true;
  return true;
}
