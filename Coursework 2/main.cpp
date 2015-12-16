#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one

#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cBkGround.h"
#include "cSprite.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "asteroidsGame.h"
#include "cButton.h"
#include "cFileHandler.h"
int score = 0;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;

	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
	windowOGL theOGLWnd;

	//Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	//Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If it fails

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}
	int score = 0;
	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);


	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\spaceShip_001.png", "Images\\spaceShip_002.png", "Images\\spaceShip_003.png", "Images\\spaceShip_004.png", "Images\\Blaster.png" };

	for (int tCount = 0; tCount < 5; tCount++)
		{
			theGameTextures.push_back(new cTexture());
			theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
		}

	// Create vector array of background textures
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/spacePlay.png", "Images/spaceStart.png", "Images/spaceEnd.png" };


	for (int tCount = 0; tCount < 3; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround spriteStartBkgd;
	spriteStartBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteStartBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteStartBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cBkGround spriteEndBkgd;
	spriteEndBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteEndBkgd.setTexture(textureBkgList[2]->getTexture());
	spriteEndBkgd.setTextureDimensions(textureBkgList[2]->getTWidth(), textureBkgList[2]->getTHeight());
	cTexture textureBkgd;

	// Create vector array of button textures
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitBtn.png", "Images/Buttons/instructionsBtn.png", "Images/Buttons/loadBtn.png", "Images/Buttons/playBtn.png", "Images/Buttons/saveBtn.png", "Images/Buttons/settingsBtn.png" };
	for (int tCount = 0; tCount < 6; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	cButton saveButton;
	saveButton.attachInputMgr(theInputMgr);
	saveButton.setTexture(btnTextureList[4]->getTexture());
	saveButton.setTextureDimensions(btnTextureList[4]->getTWidth(), btnTextureList[4]->getTHeight());

	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[3]->getTexture());
	playButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/House Loop.wav", "Audio/blaster.wav", "Audio/Explosion.wav" };

	// Creating vector array for sounds
	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);


	// load game fonts
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/Lumberjack.otf", "Fonts/A-Space.otf" };

	theFontMgr->addFont("Lumber", gameFonts[0], 24);
	theFontMgr->addFont("A-Space", gameFonts[1], 24);

	// Create vector for spaceship objects.

	for (int astro = 0; astro < 30; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(800.0f / (rand() % 5 + 1), 600.0f / (rand() % 5 + 2)));   
		theAsteroids[astro]->setSpriteTranslation(glm::vec2(30.0f, 0.0f));  
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
		glm::vec2 moveDown(theAsteroids[astro]->getSpritePos().x,(theAsteroids[astro]->getSpritePos().y) - 15.0f);
		//When the ships hit the boarder, move down
		if (theAsteroids[astro]->getSpritePos().x >= 750)
		{
			theAsteroids[astro]->setSpritePos(moveDown);
			//theAsteroids[astro]->setSpritePo(glm::vec2)theAsteroids[astro]->getSpritePos().x, (glm::vec2)theAsteroids[astro]->getSpritePos().y - 3.0f);
			//theAsteroids[astro]->setSpritePos().x -= 0.0f;

		}

		//if (theAsteroids[astro]->getSpritePos().x <= 0)
		//{
			//theAsteroids[astro]->setSpritePos().y - 3.0f;
			//theAsteroids[astro]->setSpritePos().x += 0.0f;

		//}


	}

	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\Spaceship.png");
	cRocket rocketSprite;

	//Assign the InputMgr to ship
	rocketSprite.attachInputMgr(theInputMgr);
	rocketSprite.setSpritePos(glm::vec2(512.0f, 700.0f));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);


	// include an exit button

	cFileHandler theFile("Data/usermap.dat");
	string mapData;

	string outputMsg;

	string strMsg[] = { "A = Strafe Left, D = Strafe Right, Space = Blaster, S = Play Music","Blast the ships", "Missions Control: Alien Invasion", "You stopped the alien invasion!","Well Done!" };

	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;

	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{
		//Process any window events
		pgmWNDMgr->processWNDEvents(); 

		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (theGameState) //switch statement to control game state
			{
				case MENU:
	
				// create menu game state (menu screen)
				spriteStartBkgd.render();

			
				//place and render buttons
				playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
				exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
				playButton.render();
				exitButton.render();

				//If play button is clicked update the gamestate to PLAYING
				theGameState = playButton.update(theGameState, PLAYING);
				exitButton.update(elapsedTime);

				// Display text
				outputMsg = strMsg[0];
				theFontMgr->getFont("Lumber")->printText(outputMsg.c_str(), FTPoint(350, 600, 0.0f));
				outputMsg = strMsg[1];
				theFontMgr->getFont("Lumber")->printText(outputMsg.c_str(), FTPoint(350, 650, 0.0f));
				outputMsg = strMsg[2];
				theFontMgr->getFont("Lumber")->printText(outputMsg.c_str(), FTPoint(380, 50, 0.0f));

					if (exitButton.getClicked())
						{
							SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
						}
			break;

		case PLAYING:

			// create playing state (main game)
			spriteBkgd.render();
			{
				//create enemy ships
				vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
				while (asteroidIterator != theAsteroids.end())
				{
					if ((*asteroidIterator)->isActive() == false)
					{
						//destroy enemy ship and increase score by 1
						asteroidIterator = theAsteroids.erase(asteroidIterator);
						score++;
					}
					else
					{
						(*asteroidIterator)->update(elapsedTime);
						(*asteroidIterator)->render();
						++asteroidIterator;
					}
				}
			}

			
			rocketSprite.update(elapsedTime);
			rocketSprite.update(elapsedTime);
			rocketSprite.render();

			// Place text and player score
			score = rocketSprite.updater(score);
			theFontMgr->getFont("A-Space")->printText("Space is being invaded!", FTPoint(600.0f, 750.0f, 0.0f));
			theFontMgr->getFont("A-Space")->printText("Score", FTPoint(200.0f, 750.0f, 0.0f));
			outputMsg = to_string(score);
			theFontMgr->getFont("A-Space")->printText(outputMsg.c_str(), FTPoint(350.0f, 750.0, 0.0f));

			// Render exit button
			exitButton.setSpritePos(glm::vec2(780.0f, 20.0f));
			exitButton.render();
			theGameState = exitButton.update(theGameState, END);
			outputMsg = strMsg[2];
			theFontMgr->getFont("A-Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));

			break;


		case END:

			//end game state (end screen)
			spriteEndBkgd.render();

			//Sets buttons to false
			playButton.setClicked(false);
			exitButton.setClicked(false);

			//Render play & exit buttons
			playButton.setSpritePos(glm::vec2(400.0f, 450.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 525.0f));
			playButton.render();
			exitButton.render();

			//Place text and player's final score
			score = rocketSprite.updater(score);
			theFontMgr->getFont("Lumber")->printText("Space is safe, for now..", FTPoint(600.0f, 750.0f, 0.0f));
			theFontMgr->getFont("Lumber")->printText("Score:", FTPoint(350.0f, 300.0f, 0.0f));
			outputMsg = to_string(score);
			theFontMgr->getFont("Lumber")->printText(outputMsg.c_str(), FTPoint(425.0f, 300.0, 0.0f));

			// if play button is pressed, return to play screen
			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update(theGameState, END);

			//display end text
			outputMsg = strMsg[2];
			theFontMgr->getFont("Lumber")->printText(outputMsg.c_str(), FTPoint(30, 30, 0.0f));

			outputMsg = strMsg[3];
			theFontMgr->getFont("Lumber")->printText(outputMsg.c_str(), FTPoint(350, 150, 0.0f));

			outputMsg = strMsg[4];
			theFontMgr->getFont("Lumber")->printText(outputMsg.c_str(), FTPoint(350, 200, 0.0f));

				if (exitButton.getClicked())
					{
						SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
					}
			break;

		}

		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	}

	//Free any resources
	theOGLWnd.shutdown(); 

	//Destroy the program
	pgmWNDMgr->destroyWND(); 

	//Return
	return 0; 

}