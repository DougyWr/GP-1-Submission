/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cAsteroid.h"

void cAsteroid::render()
{


	glPushMatrix();

	glTranslatef(spritePos2D.x, spritePos2D.y, 0.0f);
	glRotatef(spriteRotation, 0.0f, 0.0f, 1.0f);
	glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	glBegin(GL_QUADS);
	glColor3f(255.0f, 255.0f, 255.0f);
	glTexCoord2f(spriteTexCoordData[0].x, spriteTexCoordData[0].y);
	glVertex2f(-(textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[1].x, spriteTexCoordData[1].y);
	glVertex2f((textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[2].x, spriteTexCoordData[2].y);
	glVertex2f((textureWidth / 2), (textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[3].x, spriteTexCoordData[3].y);
	glVertex2f(-(textureWidth / 2), (textureHeight / 2));

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
//Update sprite position
void cAsteroid::update(float deltaTime)

// Prevents enemies from leaving the screen from the sides.
{
	spritePos2D += spriteTranslation * deltaTime;

	//if sprite is at point 0 on x axis allow them to move as far as point 1024.
	if (spritePos2D.x < 0 || (spritePos2D.x + textureWidth) > 1024)

		//Once sprite has reached point 1024 or 0, reverse direction and fall down
	{
		spriteTranslation *= -1;
		spritePos2D.y = spritePos2D.y + 0.25;
	}

	setBoundingRect(&boundingRect);
}
/*
=================================================================
Sets the velocity for the Asteroid
=================================================================
*/
void cAsteroid::setAsteroidVelocity(glm::vec2 AsteroidVel)
{
	asteroidVelocity = AsteroidVel;
}
/*
=================================================================
Gets the Asteroid velocity
=================================================================
*/
glm::vec2 cAsteroid::getAsteroidVelocity()
{
	return asteroidVelocity;
}
/*
==========================================================================
Use this method to show the collision box.
==========================================================================
*/
void cAsteroid::renderCollisionBox()
{
	glPushMatrix();

	glTranslatef(boundingRect.left, boundingRect.top, 0.0f);
	glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glColor3f(255.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-((boundingRect.right - boundingRect.left) / 2), -((boundingRect.bottom - boundingRect.top) / 2));
	glVertex2f(((boundingRect.right - boundingRect.left) / 2), -((boundingRect.bottom - boundingRect.top) / 2));
	glVertex2f(((boundingRect.right - boundingRect.left) / 2), ((boundingRect.bottom - boundingRect.top) / 2));
	glVertex2f(-((boundingRect.right - boundingRect.left) / 2), ((boundingRect.bottom - boundingRect.top) / 2));

	glEnd();

	glPopMatrix();
}