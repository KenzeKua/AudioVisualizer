#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include <windows.h>

#include <fmod.hpp>
#include <fmod_errors.h>

#include "demo_base.h"
#include "bitmap.h"

#define TEXTURE_COUNT 1

// Must be power of 2 number
#define SPECTRUM_SIZE 512 //1024

//#define RECT_VERTICE_W 50
//#define RECT_VERTICE_H 50
//
//// Every quad has 6 vertices
//const int RECT_VERTEX_COUNT = (RECT_VERTICE_W - 1) * (RECT_VERTICE_H - 1) * 6;
//
//// Every vertex has 3 components(x, y, z)
//const int RECT_VERTEX_ARRAY_SIZE = RECT_VERTEX_COUNT * 3;
//
//const int RECT_UV_ARRAY_SIZE = RECT_VERTEX_COUNT * 2;

const int TOTAL_ARRAYS = (200 * 200) * 6 * 3;
const int TOTAL_ARRAYS_UV = (200 * 200) * 6 * 2;

// FMODerror checking function
void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) &s\n", result, FMOD_ErrorString(result));
	}
}

class TriangleDemo : public DemoBase
{
private:
	GLuint mTextureID[TEXTURE_COUNT];

	FMOD::System* m_fmodSystem;
	FMOD::Sound* m_music;
	FMOD::Channel* m_musicChannel;

	float m_rotation;
	float m_translate;
	float m_scale;

	// For storing the octaves (average amplitude)
	float octavesArray[18] = {};

	float m_spectrumLeft[SPECTRUM_SIZE];
	float m_spectrumRight[SPECTRUM_SIZE];

	void loadTexture(const char* path, GLuint textureID)
	{
		CBitmap bitmap(path);

		// Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // apply texture wrapping along horizontal
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // apply texture wrapping along vertical

		// Old school (minecraft_ filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // near filtering/ (For when texture needs to scale up on screen)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // far filtering/ (For when texture needs to scale down on screen)
		
		// bilinear filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // near filtering. (For when texture needs to scale up on screen)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // far filtering. (For when texture needs to scale down on screen)

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 
			0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());
	}

public:
	void initFmod()
	{
		FMOD_RESULT result;

		unsigned int version;

		result = FMOD::System_Create(&m_fmodSystem);
		ERRCHECK(result);

		result = m_fmodSystem->getVersion(&version);
		ERRCHECK(result);

		if (version < FMOD_VERSION)
			printf("FMOD Error! You are using an old version of FMOD.", version, FMOD_VERSION);

		// Initialize FMOD system
		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);

		// Load and set up music
		result = m_fmodSystem->createStream("../media/supapao - HEAVYLIGHT.mp3", FMOD_SOFTWARE, 0, &m_music);
		// 20Hz-20kHz
		// Joji - WORL$TAR MONEY
		// Trippie Redd - Taking A Walk (6th)
		// Twenty One Pilots - Legend (2nd)
		// Post Malone - Circles (Noise)
		ERRCHECK(result);

		// Play the loaded mp3 music
		result = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music, false, &m_musicChannel);
		result = m_musicChannel->setVolume(0.5);
		ERRCHECK(result);

		// Set sound channel loop count
		//m_musicChannel->setLoopCount(10);
	}

	void init()
	{
		glGenTextures(TEXTURE_COUNT, mTextureID);
		loadTexture("../media/glass.bmp", mTextureID[0]);

		initFmod();
	}

	void deinit()
	{
	}

	void drawAxis(const Matrix& viewMatrix)
	{
	// ============draw axis.
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.3f, 0.3f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);

			glColor3f(0.3f, 1.0f, 0.3f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);

			glColor3f(0.3f, 0.3f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
		// ============================
	}

	void drawTriangle(void)
	{
		glBegin(GL_TRIANGLES);	// Drawing Using Triangles
			
			glVertex3f( 0.0f, 1.0f, 0.0f);				
			glVertex3f( 0.5f, 0.0f, 0.0f);					
			glVertex3f( -0.5f, 0.0f, 0.0f);		

		glEnd();	
	}

	void drawRectangle(void)
	{
		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 1.0f, 0.0f);

		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(2.0f, -1.0f, 0.0f);

		glVertex3f(2.0f, -1.0f, 0.0f);
		glVertex3f(2.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);

		glEnd();
	}

	void drawSquare(float width, float height, float depth)
	{
		glBegin(GL_TRIANGLES);

		glVertex3f(-(width * 0.5), -(height * 0.5), 0.0f);
		glVertex3f(width * 0.5, -(height * 0.5), 0.0f);
		glVertex3f(width * 0.5, height * 0.5, 0.0f);

		glVertex3f(width * 0.5, height * 0.5, 0.0f);
		glVertex3f(-(width * 0.5), height * 0.5, 0.0f);
		glVertex3f(-(width * 0.5), -(height * 0.5), 0.0f);

		glEnd();
	}

	void drawCircle(float radius, int segment)
	{
		glBegin(GL_TRIANGLES);

		double angle = 2 * M_PI / segment;

		for (int i = 0; i <= segment; i++)
		{
			glVertex3f(0, 0, 0);
			glVertex3f(radius * cos(angle * i), 0.0f, radius * sin(angle * i));
			glVertex3f(radius * cos(angle * (i + 1)), 0.0f, radius * sin(angle * (i + 1)));
		}
		glEnd();
	}

	void drawCube(float width, float height, float depth)
	{
		static GLfloat vertices[TOTAL_ARRAYS] = {};
		static GLfloat textures[TOTAL_ARRAYS_UV] = {};
		static GLubyte colors[TOTAL_ARRAYS] = {};

		// Vertices
		for (int i = 0; i < 6; i++)
		{
			if (i == 0 || i == 1)
			{
				// Left & right
				vertices[i * 54] = -0.5f + (width * i);
				vertices[1 + (i * 54)] = -0.5f;
				vertices[2 + (i * 54)] = -0.5f;
				vertices[3 + (i * 54)] = -0.5f + (width * i);
				vertices[4 + (i * 54)] = -0.5f;
				vertices[5 + (i * 54)] = -0.5f + depth;
				vertices[6 + (i * 54)] = -0.5f + (width * i);
				vertices[7 + (i * 54)] = -0.5f + height;
				vertices[8 + (i * 54)] = -0.5f + depth;

				vertices[9 + (i * 54)] = -0.5f + (width * i);
				vertices[10 + (i * 54)] = -0.5f + height;
				vertices[11 + (i * 54)] = -0.5f + depth;
				vertices[12 + (i * 54)] = -0.5f + (width * i);
				vertices[13 + (i * 54)] = -0.5f + height;
				vertices[14 + (i * 54)] = -0.5f;
				vertices[15 + (i * 54)] = -0.5f + (width * i);
				vertices[16 + (i * 54)] = -0.5f;
				vertices[17 + (i * 54)] = -0.5f;

				// Back & front
				vertices[18 + (i * 54)] = -0.5f;
				vertices[19 + (i * 54)] = -0.5f;
				vertices[20 + (i * 54)] = -0.5f + (depth * i);
				vertices[21 + (i * 54)] = -0.5f + width;
				vertices[22 + (i * 54)] = -0.5f;
				vertices[23 + (i * 54)] = -0.5f + (depth * i);
				vertices[24 + (i * 54)] = -0.5f + width;
				vertices[25 + (i * 54)] = -0.5f + height;
				vertices[26 + (i * 54)] = -0.5f + (depth * i);

				vertices[27 + (i * 54)] = -0.5f + width;
				vertices[28 + (i * 54)] = -0.5f + height;
				vertices[29 + (i * 54)] = -0.5f + (depth * i);
				vertices[30 + (i * 54)] = -0.5f;
				vertices[31 + (i * 54)] = -0.5f + height;
				vertices[32 + (i * 54)] = -0.5f + (depth * i);
				vertices[33 + (i * 54)] = -0.5f;
				vertices[34 + (i * 54)] = -0.5f;
				vertices[35 + (i * 54)] = -0.5f + (depth * i);

				// Bottom & top
				vertices[36 + (i * 54)] = -0.5f;
				vertices[37 + (i * 54)] = -0.5f + (height * i);
				vertices[38 + (i * 54)] = -0.5f;
				vertices[39 + (i * 54)] = -0.5f + width;
				vertices[40 + (i * 54)] = -0.5f + (height * i);
				vertices[41 + (i * 54)] = -0.5f;
				vertices[42 + (i * 54)] = -0.5f + width;
				vertices[43 + (i * 54)] = -0.5f + (height * i);
				vertices[44 + (i * 54)] = -0.5f + depth;

				vertices[45 + (i * 54)] = -0.5f + width;
				vertices[46 + (i * 54)] = -0.5f + (height * i);
				vertices[47 + (i * 54)] = -0.5f + depth;
				vertices[48 + (i * 54)] = -0.5f;
				vertices[49 + (i * 54)] = -0.5f + (height * i);
				vertices[50 + (i * 54)] = -0.5f + depth;
				vertices[51 + (i * 54)] = -0.5f;
				vertices[52 + (i * 54)] = -0.5f + (height * i);
				vertices[53 + (i * 54)] = -0.5f;
			}

			// Texture
			/*textures[i * 12] = 0.0f;
			textures[1 + (i * 12)] = 0.0f;
			textures[2 + (i * 12)] = 1.0f;
			textures[3 + (i * 12)] = 0.0f;
			textures[4 + (i * 12)] = 1.0f;
			textures[5 + (i * 12)] = 1.0f;

			textures[6 + (i * 12)] = 1.0f;
			textures[7 + (i * 12)] = 1.0f;
			textures[8 + (i * 12)] = 0.0f;
			textures[9 + (i * 12)] = 1.0f;
			textures[10 + (i * 12)] = 0.0f;
			textures[11 + (i * 12)] = 0.0f;*/

			// Colors
			colors[i * 18] = 42.5 * i;
			colors[1 + (i * 18)] = 0;
			colors[2 + (i * 18)] = 255;
			colors[3 + (i * 18)] = 42.5 * i;
			colors[4 + (i * 18)] = 0;
			colors[5 + (i * 18)] = 255;
			colors[6 + (i * 18)] = 42.5 * i;
			colors[7 + (i * 18)] = 0;
			colors[8 + (i * 18)] = 255;

			colors[9 + (i * 18)] = 42.5 * i;
			colors[10 + (i * 18)] = 0;
			colors[11 + (i * 18)] = 255;
			colors[12 + (i * 18)] = 42.5 * i;
			colors[13 + (i * 18)] = 0;
			colors[14 + (i * 18)] = 255;
			colors[15 + (i * 18)] = 42.5 * i;
			colors[16 + (i * 18)] = 0;
			colors[17 + (i * 18)] = 255;
		}

		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, mTextureID[0]);

		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		//glTexCoordPointer(2, GL_FLOAT, 0, textures);
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);

		// Draw cube
		glDrawArrays(GL_TRIANGLES, 0, 108);

		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		//glDisable(GL_TEXTURE_2D);
	}

	void drawPyramid(void)
	{
		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 1.0f, 1.0f);

		// Base
		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);

		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		// Face 1
		glColor3f(0.0f, 0.5f, 1.0f);

		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(0.0f, 0.5f, 0.0f);

		// Face 2
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.0f, 0.5f, 0.0f);

		// Face 3
		glColor3f(1.0f, 0.0f, 0.5f);

		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.0f, 0.5f, 0.0f);

		// Face 4
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.0f, 0.5f, 0.0f);

		glEnd();
	}

	void drawCylinder(float radius, float height, int segment)
	{
		glBegin(GL_TRIANGLES);

		double pi = 3.141592654;
		double angle =  2 * M_PI / segment;
		double originX = 0.0f, originY = 0.0f, originZ = 0.0f;

		for (int i = 1; i <= segment; i++)
		{
			// Top circle
			glVertex3f(originX, originY + height / 2, originZ);
			glVertex3f(originX + (radius * cos(i * angle)), originY + height / 2, originZ + (radius * sin(i * angle)));
			glVertex3f(originX + (radius * cos((i + 1) * angle)), originY + height / 2, originZ + (radius * sin((i + 1) * angle)));

			// Bottom circle
			glVertex3f(originX, originY - height / 2, originZ);
			glVertex3f(originX + (radius * cos(i * angle)), originY - height / 2, originZ + (radius * sin(i * angle)));
			glVertex3f(originX + (radius * cos((i + 1) * angle)), originY - height / 2, originZ + (radius * sin((i + 1) * angle)));

			// Rectangle
			glVertex3f(originX + (radius * cos((i + 1) * angle)), originY - height / 2, originZ + (radius * sin((i + 1) * angle)));
			glVertex3f(originX + (radius * cos(i * angle)), originY - height / 2, originZ + (radius * sin(i * angle)));
			glVertex3f(originX + (radius * cos(i * angle)), originY + height / 2, originZ + (radius * sin(i * angle)));

			glVertex3f(originX + (radius * cos(i * angle)), originY + height / 2, originZ + (radius * sin(i * angle)));
			glVertex3f(originX + (radius * cos((i + 1) * angle)), originY + height / 2, originZ + (radius * sin((i + 1) * angle)));
			glVertex3f(originX + (radius * cos((i + 1) * angle)), originY - height / 2, originZ + (radius * sin((i + 1) * angle)));
		}

		glEnd();
	}

	void drawCone(float radius, float height, int segment)
	{
		glBegin(GL_TRIANGLES);

		double pi = 3.141592654;
		double angle = 2 * M_PI / segment;
		double originX = 0.0f, originY = 0.0f, originZ = 0.0f;

		for (int i = 1; i <= segment; i++)
		{
			// Top circle
			glVertex3f(originX, originY + height / 2, originZ);
			glVertex3f(originX + (radius * cos(i * angle)), originY - height / 2, originZ + (radius * sin(i * angle)));
			glVertex3f(originX + (radius * cos((i + 1) * angle)), originY - height / 2, originZ + (radius * sin((i + 1) * angle)));

			// Bottom circle
			glVertex3f(originX, originY - height / 2, originZ);
			glVertex3f(originX + (radius * cos(i * angle)), originY - height / 2, originZ + (radius * sin(i * angle)));
			glVertex3f(originX + (radius * cos((i + 1) * angle)), originY - height / 2, originZ + (radius * sin((i + 1) * angle)));
		}

		glEnd();
	}

	void drawBrokenCone(float radius, float height, int segment)
	{
		glBegin(GL_TRIANGLES);

		double pi = 3.141592654;
		double angle = 2 * M_PI / segment;
		double originX = 0.0f, originY = 0.0f, originZ = 0.0f;

		for (int i = 1; i <= segment; i++)
		{
			// Top circle
			glVertex3f(originX, originY + height / 2, originZ);
			glVertex3f(originX + (radius * cos(i * angle)), originY - height / 2, originZ + (radius * sin(i * angle)));
			glVertex3f(originX + (radius * cos((i + 1) * angle)), originY - height / 2, originZ + (radius * sin((i + 1) * angle)));
		}

		glEnd();
	}

	void drawSphere(float radius, int latSeg, int longSeg)
	{
		// theta = latitude
		// phi = longitude / azimuth
		// x = r sin theta cos phi
		// y = r sin theta sin phi
		// z = r cos theta

		glBegin(GL_TRIANGLES);

		double x, y, z;
		double pi = 3.141592654;
		float latAngle = M_PI / latSeg;
		float longAngle = 2 * M_PI / longSeg;

		for (int i = 0; i < longSeg; i++)
		{
			//glColor3f(1.0f / longSeg * i, 0.0f, 1.0f);

			for (int j = 0; j < latSeg; j++)
			{
				x = radius * sin(latAngle * j) * cos(longAngle * i);
				y = radius * sin(latAngle * j) * sin(longAngle * i);
				z = radius * cos(latAngle * j);

				glVertex3f(x, y , z);

				x = radius * sin(latAngle * (j + 1)) * cos(longAngle * i);
				y = radius * sin(latAngle * (j + 1)) * sin(longAngle * i);
				z = radius * cos(latAngle * (j + 1));

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * (j + 1)) * cos(longAngle * (i + 1));
				y = radius * sin(latAngle * (j + 1)) * sin(longAngle * (i + 1));
				z = radius * cos(latAngle * (j + 1));

				glVertex3f(x, y, z);
				glVertex3f(x, y, z);

				x = radius * sin(latAngle * j) * cos(longAngle * (i + 1));
				y = radius * sin(latAngle * j) * sin(longAngle * (i + 1));
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * j) * cos(longAngle * i);
				y = radius * sin(latAngle * j) * sin(longAngle * i);
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}

	void drawIcosahedron(float radius, int latSeg, int longSeg)
	{
		// (0, ±1, ±phi)
		// (±1, ±phi, 0)
		// (±phi, 0, ±1)
		// phi = (1 + sqrt(5)) / 2

		float phi = (1.0f + sqrt(5)) / 2;

		glBegin(GL_TRIANGLES);
		
		glEnd();
	}

	void drawHemisphere(float radius, int latSeg, int longSeg)
	{
		glBegin(GL_TRIANGLES);

		double x, y, z;
		double pi = 3.141592654;
		float latAngle = M_PI / latSeg;
		float longAngle = 2 * M_PI / longSeg / 2;

		for (int i = 0; i < longSeg; i++)
		{
			//glColor3f(1.0f / longSeg * i, 0.0f, 1.0f);

			for (int j = 0; j < latSeg; j++)
			{
				x = radius * sin(latAngle * j) * cos(longAngle * i);
				y = radius * sin(latAngle * j) * sin(longAngle * i);
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * (j + 1)) * cos(longAngle * i);
				y = radius * sin(latAngle * (j + 1)) * sin(longAngle * i);
				z = radius * cos(latAngle * (j + 1));

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * (j + 1)) * cos(longAngle * (i + 1));
				y = radius * sin(latAngle * (j + 1)) * sin(longAngle * (i + 1));
				z = radius * cos(latAngle * (j + 1));

				glVertex3f(x, y, z);
				glVertex3f(x, y, z);

				x = radius * sin(latAngle * j) * cos(longAngle * (i + 1));
				y = radius * sin(latAngle * j) * sin(longAngle * (i + 1));
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * j) * cos(longAngle * i);
				y = radius * sin(latAngle * j) * sin(longAngle * i);
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);
			}
		}

		for (int i = 0; i < latSeg * 2; i++)
		{
			glVertex3f(0, 0, 0);
			glVertex3f(radius * cos(latAngle / 2 + latAngle * i), 0.0f, radius * sin(latAngle / 2 + latAngle * i));
			glVertex3f(radius * cos(latAngle / 2 + latAngle * (i + 1)), 0.0f, radius * sin(latAngle / 2 + latAngle * (i + 1)));
		}
		glEnd();
	}

	void drawBrokenHemi(float radius, int latSeg, int longSeg)
	{
		glBegin(GL_TRIANGLES);

		double x, y, z;
		float latAngle = M_PI / latSeg;
		float longAngle = 2 * M_PI / longSeg / 2;

		for (int i = 0; i < longSeg; i++)
		{
			//glColor3f(1.0f / longSeg * i, 0.0f, 1.0f);

			for (int j = 0; j < latSeg; j++)
			{
				x = radius * sin(latAngle * j) * cos(longAngle * i);
				y = radius * sin(latAngle * j) * sin(longAngle * i);
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * (j + 1)) * cos(longAngle * i);
				y = radius * sin(latAngle * (j + 1)) * sin(longAngle * i);
				z = radius * cos(latAngle * (j + 1));

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * (j + 1)) * cos(longAngle * (i + 1));
				y = radius * sin(latAngle * (j + 1)) * sin(longAngle * (i + 1));
				z = radius * cos(latAngle * (j + 1));

				glVertex3f(x, y, z);
				glVertex3f(x, y, z);

				x = radius * sin(latAngle * j) * cos(longAngle * (i + 1));
				y = radius * sin(latAngle * j) * sin(longAngle * (i + 1));
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * j) * cos(longAngle * i);
				y = radius * sin(latAngle * j) * sin(longAngle * i);
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);
			}
		}

		glEnd();
	}

	void drawEigthSphere(float radius, int latSeg, int longSeg)
	{
		glBegin(GL_TRIANGLES);

		double x, y, z;
		double pi = 3.141592654;
		float latAngle = M_PI / latSeg / 2;
		float longAngle = 2 * M_PI / longSeg / 2;

		for (int i = 0; i < longSeg; i++)
		{
			//glColor3f(1.0f / longSeg * i, 0.0f, 1.0f);

			for (int j = 0; j < latSeg; j++)
			{
				x = radius * sin(latAngle * j) * cos(longAngle * i);
				y = radius * sin(latAngle * j) * sin(longAngle * i);
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * (j + 1)) * cos(longAngle * i);
				y = radius * sin(latAngle * (j + 1)) * sin(longAngle * i);
				z = radius * cos(latAngle * (j + 1));

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * (j + 1)) * cos(longAngle * (i + 1));
				y = radius * sin(latAngle * (j + 1)) * sin(longAngle * (i + 1));
				z = radius * cos(latAngle * (j + 1));

				glVertex3f(x, y, z);
				glVertex3f(x, y, z);

				x = radius * sin(latAngle * j) * cos(longAngle * (i + 1));
				y = radius * sin(latAngle * j) * sin(longAngle * (i + 1));
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latAngle * j) * cos(longAngle * i);
				y = radius * sin(latAngle * j) * sin(longAngle * i);
				z = radius * cos(latAngle * j);

				glVertex3f(x, y, z);
			}
		}

		for (int i = 0; i < longSeg; i++)
		{
			glVertex3f(0, 0, 0);
			glVertex3f(radius * cos(longAngle * i), radius * sin(longAngle * i), 0.0f);
			glVertex3f(radius * cos(longAngle * (i + 1)), radius * sin(longAngle * (i + 1)), 0.0f);
		}
		for (int i = 0; i < latSeg * 2; i++)
		{
			glVertex3f(0, 0, 0);
			glVertex3f(radius * cos(latAngle * i), 0.0f, radius * sin(latAngle * i));
			glVertex3f(radius * cos(latAngle * (i + 1)), 0.0f, radius * sin(latAngle * (i + 1)));
		}
		glEnd();
	}

	void drawSpherePattern(float radius, int latitudeSegment, int longitudeSegment)
	{
		glBegin(GL_TRIANGLES);

		double x, y, z;
		float latitudeAngle = 2 * M_PI / latitudeSegment;
		float longitudeAngle = M_PI / longitudeSegment;

		for (int i = 0; i < longitudeSegment; i++)
		{
			glColor3f(1.0f, 0.0f, 1.0f / longitudeSegment * i);

			for (int j = 0; j < latitudeSegment; j++)
			{
				x = radius * sin(latitudeAngle * j) * cos(longitudeAngle * (j + i));
				y = radius * sin(latitudeAngle * j) * sin(longitudeAngle * (j + i));
				z = radius * cos(latitudeAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latitudeAngle * (j + 1)) * cos(longitudeAngle * (j + i));
				y = radius * sin(latitudeAngle * (j + 1)) * sin(longitudeAngle * (j + i));
				z = radius * cos(latitudeAngle * (j + 1));

				glVertex3f(x, y, z);

				x = radius * sin(latitudeAngle * (j + 1)) * cos(longitudeAngle * (j + i + 1));
				y = radius * sin(latitudeAngle * (j + 1)) * sin(longitudeAngle * (j + i + 1));
				z = radius * cos(latitudeAngle * (j + 1));

				glVertex3f(x, y, z);
				glVertex3f(x, y, z);

				x = radius * sin(latitudeAngle * j) * cos(longitudeAngle * (j + i + 1));
				y = radius * sin(latitudeAngle * j) * sin(longitudeAngle * (j + i + 1));
				z = radius * cos(latitudeAngle * j);

				glVertex3f(x, y, z);

				x = radius * sin(latitudeAngle * j) * cos(longitudeAngle * (j + i));
				y = radius * sin(latitudeAngle * j) * sin(longitudeAngle * (j + i));
				z = radius * cos(latitudeAngle * j);

				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}

	void drawPokemon(const Matrix& viewMatrix)
	{
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);

		// •	Right drill
		Matrix translate = Matrix::makeTranslationMatrix(-0.4f, -0.95f, 1.10f);
		Matrix animTrans = Matrix::makeTranslationMatrix(0.0f, sin(m_translate) * 0.05f, 0.0);
		Matrix rotate = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		Matrix rotateA = Matrix::makeRotateMatrix(90, Vector(0.0f, 0.0f, 1.0f));
		Matrix rightDrillModel = translate * rotate * rotateA * animTrans;
		Matrix viewSpaceMatrix = viewMatrix * rightDrillModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.84f, 0.88f, 0.88f);
		drawCone(0.15f, 0.7f, 20);

		// 11	Right arm
		Matrix translate11 = Matrix::makeTranslationMatrix(-0.25f, -0.9f, 0.49f);
		Matrix rotate11 = Matrix::makeRotateMatrix(90, Vector(0.9f, 1.0f, 1.0f));
		Matrix animRotate = Matrix::makeRotateMatrix(sin(m_rotation) * -15, Vector(0.0f, 0.1f, 0.1f));
		Matrix scale11 = Matrix::makeScaleMatrix(0.03f, 0.316f, 0.03f);
		Matrix rightArmModel = translate11 * rotate11 * animRotate * scale11;
		Matrix viewSpaceMatrix11 = viewMatrix * rightArmModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix11.mVal);
		glColor3f(0.59f, 0.6f, 0.68f);
		drawCube(1.0f, 1.0f, 1.0f);
		// 13	Right forearm
		Matrix translate13 = Matrix::makeTranslationMatrix(-0.4f, -0.95f, 0.66f);
		Matrix animTransA = Matrix::makeTranslationMatrix(0.0f, sin(m_translate) * -0.1f, 0.0);
		Matrix rotate13 = Matrix::makeRotateMatrix(90, Vector(1.0f, 0.0f, 0.0f));
		Matrix scale13 = Matrix::makeScaleMatrix(0.02f, 0.3f, 0.02f);
		Matrix rightForearmModel = translate13 * rotate13 * scale13 * animTransA;
		Matrix viewSpaceMatrix13 = viewMatrix * rightForearmModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix13.mVal);
		drawCube(1.0f, 1.0f, 1.0f);

		// 15	Right thigh
		Matrix translate15 = Matrix::makeTranslationMatrix(-0.29f, -1.18f, 0.5f);
		Matrix rotate15 = Matrix::makeRotateMatrix(220, Vector(1.0f, -0.2f, 1.0f));
		Matrix scale15 = Matrix::makeScaleMatrix(0.035f, 0.5f, 0.035f);
		Matrix rightThighModel = translate15 * rotate15 * scale15;
		Matrix viewSpaceMatrix15 = viewMatrix * rightThighModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix15.mVal);
		drawCube(1.0f, 1.0f, 1.0f);
		// 17	Right leg
		Matrix translate17 = Matrix::makeTranslationMatrix(-0.44f, -1.57f, 0.57f);
		Matrix scale17 = Matrix::makeScaleMatrix(0.03f, 0.45f, 0.03f);
		Matrix rightLegModel = translate17 * scale17;
		Matrix viewSpaceMatrix17 = viewMatrix * rightLegModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix17.mVal);
		drawCube(1.0f, 1.0f, 1.0f);
		// 19	Right foot
		Matrix translate19 = Matrix::makeTranslationMatrix(-0.44f, -1.8f, 0.63f);
		Matrix rotate19 = Matrix::makeRotateMatrix(90, Vector(1.0f, 0.0f, 0.0f));
		Matrix scale19 = Matrix::makeScaleMatrix(0.029f, 0.15f, 0.029f);
		Matrix rightFootModel = translate19 * rotate19 * scale19;
		Matrix viewSpaceMatrix19 = viewMatrix * rightFootModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix19.mVal);
		drawCube(1.0f, 1.0f, 1.0f);

		// 1	Left drill
		Matrix translate1 = Matrix::makeTranslationMatrix(0.4f, -0.95f, 1.10f);
		Matrix animTrans1 = Matrix::makeTranslationMatrix(0.0f, sin(m_translate) * -0.05f, 0.0f);
		Matrix leftDrillModel = translate1 * rotate * rotateA * animTrans1;
		Matrix viewSpaceMatrix1 = viewMatrix * leftDrillModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix1.mVal);
		glColor3f(0.84f, 0.88f, 0.88f);
		drawCone(0.15f, 0.7f, 20);

		// 12	Left arm
		Matrix translate12 = Matrix::makeTranslationMatrix(0.25f, -0.9f, 0.49f);
		Matrix rotate12 = Matrix::makeRotateMatrix(90, Vector(0.9f, -1.0f, -1.0f));
		Matrix animRotate1 = Matrix::makeRotateMatrix(sin(m_rotation) * -15, Vector(0.1f, 0.0f, 0.1f));
		Matrix leftArmModel = translate12 * rotate12 * animRotate1 * scale11;
		Matrix viewSpaceMatrix12 = viewMatrix * leftArmModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix12.mVal);
		glColor3f(0.59f, 0.6f, 0.68f);
		drawCube(1.0f, 1.0f, 1.0f);
		// 14	Left forearm
		Matrix translate14 = Matrix::makeTranslationMatrix(0.4f, -0.95f, 0.66f);
		Matrix animTrans1A = Matrix::makeTranslationMatrix(0.0f, sin(m_translate) * 0.1f, 0.0f);
		Matrix leftForearmModel = translate14 * rotate13 * scale13 * animTrans1A;
		Matrix viewSpaceMatrix14 = viewMatrix * leftForearmModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix14.mVal);
		drawCube(1.0f, 1.0f, 1.0f);

		// 16	Left thigh
		Matrix translate16 = Matrix::makeTranslationMatrix(0.29f, -1.18f, 0.5f);
		Matrix rotate16 = Matrix::makeRotateMatrix(220, Vector(1.0f, 0.2f, -1.0f));
		Matrix leftThighModel = translate16 * rotate16 * scale15;
		Matrix viewSpaceMatrix16 = viewMatrix * leftThighModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix16.mVal);
		drawCube(1.0f, 1.0f, 1.0f);
		// 18	Left leg
		Matrix translate18 = Matrix::makeTranslationMatrix(0.44f, -1.57f, 0.57f);
		Matrix leftLegModel = translate18 * scale17;
		Matrix viewSpaceMatrix18 = viewMatrix * leftLegModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix18.mVal);
		drawCube(1.0f, 1.0f, 1.0f);
		// 20	Left foot
		Matrix translate20 = Matrix::makeTranslationMatrix(0.44f, -1.8f, 0.63f);
		Matrix rotate20 = Matrix::makeRotateMatrix(90, Vector(1.0f, 0.0f, 0.0f));
		Matrix leftFootModel = translate20 * rotate20 * scale19;
		Matrix viewSpaceMatrix20 = viewMatrix * leftFootModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix20.mVal);
		drawCube(1.0f, 1.0f, 1.0f);

		// 2	Needle
		Matrix translate2 = Matrix::makeTranslationMatrix(0.0f, -1.8f, 0.95f);
		Matrix rotate2 = Matrix::makeRotateMatrix(180, Vector(0.0f, 0.0f, 1.0f));
		Matrix rotate2a = Matrix::makeRotateMatrix(50, Vector(-1.0f, 0.0f, 0.0f));
		Matrix needleModel = translate2 * rotate2 * rotate2a;
		Matrix viewSpaceMatrix2 = viewMatrix * needleModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix2.mVal);
		glColor3f(0.93f, 0.82f, 0.38f);
		drawCone(0.1f, 0.4f, 20);

		// 3	Abdomen
		Matrix translate3 = Matrix::makeTranslationMatrix(0.0f, -1.45f, 0.55f);
		Matrix rotate3 = Matrix::makeRotateMatrix(-65, Vector(1.0f, 0.0f, 0.0f));
		Matrix scale3 = Matrix::makeScaleMatrix(0.8f, 0.8f, 1.0f);
		Matrix abdomenModel = translate3 * rotate3 * scale3;
		Matrix viewSpaceMatrix3 = viewMatrix * abdomenModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix3.mVal);
		drawSphere(0.4f, 20, 20);

		// 21	Abdomen stripe 1
		Matrix translate21 = Matrix::makeTranslationMatrix(0.0f, -1.35f, 0.5f);
		Matrix rotate21 = Matrix::makeRotateMatrix(28, Vector(1.0f, 0.0f, 0.0f));
		Matrix blackStripe1 = translate21 * rotate21;
		Matrix viewSpaceMatrix21 = viewMatrix * blackStripe1;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix21.mVal);
		glColor3f(0.34f, 0.34f, 0.34f);
		drawCylinder(0.31f, 0.1f, 20);
		// 22	Abdomen stripe 2
		Matrix translate22 = Matrix::makeTranslationMatrix(0.0f, -1.55f, 0.6f);
		Matrix rotate22 = Matrix::makeRotateMatrix(28, Vector(1.0f, 0.0f, 0.0f));
		Matrix blackStripe2 = translate22 * rotate22;
		Matrix viewSpaceMatrix22 = viewMatrix * blackStripe2;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix22.mVal);
		drawCylinder(0.31f, 0.1f, 20);

		// 4	Thorax
		Matrix translate4 = Matrix::makeTranslationMatrix(0.0f, -0.9f, 0.45f);
		Matrix rotate4 = Matrix::makeRotateMatrix(65, Vector(1.0f, 0.0f, 0.0f));
		Matrix scale4 = Matrix::makeScaleMatrix(0.8f, 0.8f, 1.2f);
		Matrix thoraxModel = translate4 * rotate4 * scale4;
		Matrix viewSpaceMatrix4 = viewMatrix * thoraxModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix4.mVal);
		glColor3f(0.93f, 0.82f, 0.38f);
		drawSphere(0.2f, 20, 20);

		// 5	Head
		Matrix translate5 = Matrix::makeTranslationMatrix(0.0f, -0.6f, 0.5f);
		Matrix headModel = translate5;
		Matrix viewSpaceMatrix5 = viewMatrix * headModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix5.mVal);
		drawSphere(0.2f, 20, 20);

		// 6	Nose
		Matrix translate6 = Matrix::makeTranslationMatrix(0.0f, -0.6f, 0.55f);
		Matrix rotate6 = Matrix::makeRotateMatrix(-15, Vector(1.0f, 0.0f, 0.0f));
		Matrix scale6 = Matrix::makeScaleMatrix(0.7f, 0.9f, 1.2f);
		Matrix noseModel = translate6 * rotate6 * scale6;
		Matrix viewSpaceMatrix6 = viewMatrix * noseModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix6.mVal);
		drawEigthSphere(0.2f, 20, 20);
		// 6A	Jaw
		Matrix translate6A = Matrix::makeTranslationMatrix(0.0f, -0.65f, 0.68f);
		Matrix rotate6A = Matrix::makeRotateMatrix(-96, Vector(1.0f, 0.0f, 0.0f));
		Matrix scale6A = Matrix::makeScaleMatrix(0.9f, 1.0f, 0.5f);
		Matrix jawModel = translate6A * rotate6A * scale6A;
		Matrix viewSpaceMatrix6A = viewMatrix * jawModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix6A.mVal);
		drawCone(0.2f, 0.2f, 20);
		// End head

		//Eyes
		// 23	Right eye
		Matrix translate23 = Matrix::makeTranslationMatrix(-0.08f, -0.55f, 0.6f);
		Matrix rotate23 = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		Matrix rotate23A = Matrix::makeRotateMatrix(-25, Vector(0.0f, 0.0f, 1.0f));
		Matrix rightEyeModel = translate23 * rotate23A * rotate23;
		Matrix viewSpaceMatrix23 = viewMatrix * rightEyeModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix23.mVal);
		glColor3f(0.76f, 0.3f, 0.36f);
		drawEigthSphere(0.1f, 20, 20);
		// 24	Left eye
		Matrix translate24 = Matrix::makeTranslationMatrix(0.08f, -0.55f, 0.6f);
		Matrix rotate24 = Matrix::makeRotateMatrix(-90, Vector(0.0f, 1.0f, 0.0f));
		Matrix rotate24A = Matrix::makeRotateMatrix(25, Vector(0.0f, 0.0f, 1.0f));
		Matrix leftEyeModel = translate24 * rotate24A * rotate24;
		Matrix viewSpaceMatrix24 = viewMatrix * leftEyeModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix24.mVal);
		drawEigthSphere(0.1f, 20, 20);

		// WINGS
		// 25	Right
		Matrix translate25 = Matrix::makeTranslationMatrix(-0.4f, -0.9f, 0.05f);
		Matrix animTrans2 = Matrix::makeTranslationMatrix(0.0f, 0.0f, sin(m_translate) * -0.04f);
		Matrix rotate25 = Matrix::makeRotateMatrix(45, Vector(0.0f, 1.0f, 0.0f));
		Matrix animRotate2 = Matrix::makeRotateMatrix(sin(m_rotation) * -10, Vector(0.0f, 0.1f, 0.0f));
		Matrix scale25 = Matrix::makeScaleMatrix(1.0f, 0.75f, 1.0f);
		Matrix rightWing = animTrans2 * animRotate2 * translate25 * rotate25 * scale25;
		Matrix viewSpaceMatrix25 = viewMatrix * rightWing;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix25.mVal);
		glColor3f(0.91f, 0.94f, 0.95f);
		drawTriangle();
		// 26	Left
		Matrix translate26 = Matrix::makeTranslationMatrix(0.4f, -0.9f, 0.05f);
		Matrix rotate26 = Matrix::makeRotateMatrix(-45, Vector(0.0f, 1.0f, 0.0f));
		Matrix animRotate3 = Matrix::makeRotateMatrix(sin(m_rotation) * 10, Vector(0.0f, 0.1f, 0.0f));
		Matrix leftWing = animTrans2 * animRotate3 * translate26 * rotate26 * scale25;
		Matrix viewSpaceMatrix26 = viewMatrix * leftWing;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix26.mVal);
		drawTriangle();

		// 7	Right antenna
		Matrix translate7 = Matrix::makeTranslationMatrix(-0.08f, -0.35f, 0.5f);
		Matrix scale7 = Matrix::makeScaleMatrix(0.01f, 0.2f, 0.01f);
		Matrix rightAntennaModel = translate7 * scale7;
		Matrix viewSpaceMatrix7 = viewMatrix * rightAntennaModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix7.mVal);
		glColor3f(0.5f, 0.5f, 0.6f);
		drawCube(1.0f, 1.0f, 1.0f);
		// 8	Left antenna
		Matrix translate8 = Matrix::makeTranslationMatrix(0.08f, -0.35f, 0.5f);
		Matrix leftAntennaModel = translate8 * scale7;
		Matrix viewSpaceMatrix8 = viewMatrix * leftAntennaModel;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix8.mVal);
		drawCube(1.0f, 1.0f, 1.0f);

		// 9	Right antenna end
		Matrix translate9 = Matrix::makeTranslationMatrix(-0.08f, -0.25f, 0.65f);
		Matrix rotate9 = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		Matrix rotate9A = Matrix::makeRotateMatrix(90, Vector(0.0f, 0.0f, 1.0f));
		Matrix scale9 = Matrix::makeScaleMatrix(0.01f, 0.3f, 0.01f);
		Matrix rightAntennaModelA = translate9 * rotate9 * rotate9A * scale9;
		Matrix viewSpaceMatrix9 = viewMatrix * rightAntennaModelA;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix9.mVal);
		drawCube(1.0f, 1.0f, 1.0f);
		// 10	Left antenna end
		Matrix translate10 = Matrix::makeTranslationMatrix(0.08f, -0.25f, 0.65f);
		Matrix leftAntennaModelA = translate10 * rotate9 * rotate9A * scale9;
		Matrix viewSpaceMatrix10 = viewMatrix * leftAntennaModelA;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix10.mVal);
		drawCube(1.0f, 1.0f, 1.0f);
	}

	void drawTexture(void)
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		glEnable(GL_TEXTURE_2D); // Enable texturing
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]); // Bind panel texture

		glBegin(GL_TRIANGLES);

		static float r = 0.0f; // Persistent in the memory; will only be deleted once the programme is closed
		r += 0.1f;
		float mul = (sinf(r) + 1.0f) * 0.5f;

		// Face 1
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

		// Face 2
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);

		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

		// Face 3
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

		// Face 4
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

		// Face 5
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		// Face 6
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	void arrayPlane(float width, float height, float depth, int columns, int rows, float speed, float amplitude, float frequency)
	{
		m_translate += speed;

		static float totalArrays = (columns * rows) * 6 * 3;
		static float totalUV = (columns * rows) * 6 * 2;
		static float totalVertices = (columns * rows) * 6;

		// Create vertices data
		static GLfloat vertices[TOTAL_ARRAYS] =
		{
			//-1.0f, -1.0f, 1.0f,
			//1.0f, -1.0f, 1.0f,
			//1.0f, 1.0f, 1.0f,
			//1.0f, 1.0f, 1.0f,
			//-1.0f, 1.0f, 1.0f,
			//-1.0f, -1.0f, 1.0f,

			//1.0, -1.0, -1.0,
			//1.0, 1.0, -1.0,
			//1.0, 1.0, 1.0,
			//1.0, 1.0, 1.0,
			//1.0, -1.0, 1.0,
			//1.0, -1.0, -1.0,

			//-1.0, 1.0, -1.0,
			//1.0, 1.0, -1.0,
			//1.0, 1.0, 1.0,
			//1.0, 1.0, 1.0,
			//-1.0, 1.0, 1.0,
			//-1.0, 1.0, -1.0,
			////	*	*	*
			//-1.0f, -1.0f, -1.0f,
			//1.0f, -1.0f, -1.0f,
			//1.0f, 1.0f, -1.0f,
			//1.0f, 1.0f, -1.0f,
			//-1.0f, 1.0f, -1.0f,
			//-1.0f, -1.0f, -1.0f,

			//-1.0, -1.0, -1.0,
			//-1.0, 1.0, -1.0,
			//-1.0, 1.0, 1.0,
			//-1.0, 1.0, 1.0,
			//-1.0, -1.0, 1.0,
			//-1.0, -1.0, -1.0,

			//-1.0, -1.0, -1.0,
			//1.0, -1.0, -1.0,
			//1.0, -1.0, 1.0,
			//1.0, -1.0, 1.0,
			//-1.0, -1.0, 1.0,
			//-1.0, -1.0, -1.0,
		};

		static float widthInc = width / (float)columns;
		static float depthInc = depth / (float)rows;
		int k = 0;
		int columnsChecker = 0;
		int rowsChecker = 0;
		for (float i = (((float)width * 0.5) * -1); i < width * 0.5; i += widthInc)
		{
			rowsChecker = 0;
			for (float j = (((float)depth * 0.5) * -1); j < depth * 0.5; j += depthInc)
			{
				vertices[k] = i;
				//vertices[k + 1] = sin(m_translate + (i * frequency)) * amplitude + height;
				vertices[k + 2] = j;

				vertices[k + 3] = i + widthInc;
				//vertices[k + 4] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height;
				vertices[k + 5] = j;

				vertices[k + 6] = i + widthInc;
				//vertices[k + 7] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height;
				vertices[k + 8] = j + depthInc;
				//	*	*	*
				vertices[k + 9] = i + widthInc;
				//vertices[k + 10] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height;
				vertices[k + 11] = j + depthInc;

				vertices[k + 12] = i;
				//vertices[k + 13] = sin(m_translate + (i * frequency)) * amplitude + height;
				vertices[k + 14] = j + depthInc;

				vertices[k + 15] = i;
				//vertices[k + 16] = sin(m_translate + (i * frequency)) * amplitude + height;
				vertices[k + 17] = j;

				if (rowsChecker % 2 == 0)
				{
					if (columnsChecker % 2 == 0)
					{
						// |
						// |
						// -----
						vertices[k + 1] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3] + octavesArray[4];
						vertices[k + 4] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3];
						vertices[k + 7] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height - octavesArray[3] - octavesArray[1];
						vertices[k + 10] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height - octavesArray[3] - octavesArray[1];
						vertices[k + 13] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3];
						vertices[k + 16] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3] + octavesArray[4];
					}
					else if (columnsChecker % 2 == 1)
					{
						//     |
						//     |
						// -----

						vertices[k + 1] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3];
						vertices[k + 4] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3] + octavesArray[4];
						vertices[k + 7] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3];
						vertices[k + 10] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3];
						vertices[k + 13] = sin(m_translate + (i * frequency)) * amplitude + height - octavesArray[3] - octavesArray[1];
						vertices[k + 16] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3];
					}
				}
				else if (rowsChecker % 2 == 1)
				{
					if (columnsChecker % 2 == 0)
					{
						// -----
						// |
						// |
						vertices[k + 1] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3];
						vertices[k + 4] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height - octavesArray[3] - octavesArray[1];
						vertices[k + 7] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3];
						vertices[k + 10] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3];
						vertices[k + 13] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3] + octavesArray[4];
						vertices[k + 16] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3];
					}
					else if (columnsChecker % 2 == 1)
					{
						// -----
						//     |
						//     |
						vertices[k + 1] = sin(m_translate + (i * frequency)) * amplitude + height - octavesArray[3] - octavesArray[1];
						vertices[k + 4] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3];
						vertices[k + 7] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3] + octavesArray[4];
						vertices[k + 10] = sin(m_translate + (i * frequency) + widthInc * frequency) * amplitude + height + octavesArray[3] + octavesArray[4];
						vertices[k + 13] = sin(m_translate + (i * frequency)) * amplitude + height + octavesArray[3];
						vertices[k + 16] = sin(m_translate + (i * frequency)) * amplitude + height - octavesArray[3] - octavesArray[1];
					}
				}
				k += 18;
				rowsChecker++;
			}
			columnsChecker++;
		}

		// Create texture coordinates data
		static GLfloat texCoords[TOTAL_ARRAYS_UV] = 
		{
			/*0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,*/
		};

		static float textureUmapInc = widthInc / width;
		static float textureVmapInc = depthInc / depth;
		int UVArrayIndex = 0;
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				texCoords[UVArrayIndex] = textureUmapInc * i;
				texCoords[UVArrayIndex + 1] = 1 - (textureVmapInc * j);
				texCoords[UVArrayIndex + 2] = textureUmapInc * (i + 1);
				texCoords[UVArrayIndex + 3] = 1 - (textureVmapInc * j);
				texCoords[UVArrayIndex + 4] = textureUmapInc * (i + 1);
				texCoords[UVArrayIndex + 5] = 1 - (textureVmapInc * (j + 1));

				texCoords[UVArrayIndex + 6] = textureUmapInc * (i + 1);
				texCoords[UVArrayIndex + 7] = 1 - (textureVmapInc * (j + 1));
				texCoords[UVArrayIndex + 8] = textureUmapInc * i;
				texCoords[UVArrayIndex + 9] = 1 - (textureVmapInc * (j + 1));
				texCoords[UVArrayIndex + 10] = textureUmapInc * i;
				texCoords[UVArrayIndex + 11] = 1 - (textureVmapInc * j);

				UVArrayIndex += 12;
			}
		}

		// Create color data
		static GLubyte colors[TOTAL_ARRAYS] =
		{
			/*255, 255, 255,
			255, 0, 0,
			0, 255, 0,
			0, 255, 0,
			255, 0, 0,
			255, 255, 255,*/
		};

		static float colorIncrement = 255 / totalArrays;
		for (int i = 0; i < totalArrays; i += 18)
		{
			colors[i] = 76.5 + (i * colorIncrement * 0.7);
			colors[i + 1] = i * (colorIncrement * 0.5);
			colors[i + 2] = 255 - (i * colorIncrement);
			colors[i + 3] = 76.5 + (i * colorIncrement * 0.7);
			colors[i + 4] = i * (colorIncrement * 0.5);
			colors[i + 5] = 255 - (i * colorIncrement);
			colors[i + 6] = 76.5 + (i * colorIncrement * 0.7);
			colors[i + 7] = i * (colorIncrement * 0.5);
			colors[i + 8] = 255 - (i * colorIncrement);

			colors[i + 9] = 76.5 + (i * colorIncrement * 0.7);
			colors[i + 10] = i * (colorIncrement * 0.5);
			colors[i + 11] = 255 - (i * colorIncrement);
			colors[i + 12] = 76.5 + (i * colorIncrement * 0.7);
			colors[i + 13] = i * (colorIncrement * 0.5);
			colors[i + 14] = 255 - (i * colorIncrement);
			colors[i + 15] = 76.5 + (i * colorIncrement * 0.7);
			colors[i + 16] = i * (colorIncrement * 0.5);
			colors[i + 17] = 255 - (i * colorIncrement);
		}

		//glEnable(GL_BLEND);
		/*glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);*/

		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		//glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
		glColorPointer(3, GL_UNSIGNED_BYTE, 0 , colors);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA); // Alpha blend
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Add blend
		//glBlendFunc(GL_DST_COLOR, GL_ZERO); // Multiply blend

		// Draw rectangle
		glDrawArrays(GL_TRIANGLES, 0 , totalVertices);

		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		//glDisable(GL_TEXTURE_2D);
	}

	void updateFmod()
	{
		m_fmodSystem->update();

		// Get spectrum for left and right stereo channels
		m_musicChannel->getSpectrum(m_spectrumLeft, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		m_musicChannel->getSpectrum(m_spectrumRight, SPECTRUM_SIZE, 1, FMOD_DSP_FFT_WINDOW_RECT);

		//std::cout << m_spectrumLeft[0] << "\t,\t" << m_spectrumRight[0] << std::endl;
		std::cout << m_spectrumLeft[0] + m_spectrumRight[0] * 0.5f << std::endl;
	}

	void shapesAudioVisualiser(const Matrix& viewMatrix)
	{
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);

		m_rotation += 0.35f;

		Matrix rotate = Matrix::makeRotateMatrix(90, Vector(1.0f, 0.0f, 0.0f));
		Matrix translatePenta = Matrix::makeTranslationMatrix(-12.0f, 5.0f, 8.5f);
		Matrix rotation8 = Matrix::makeRotateMatrix(m_rotation + (octavesArray[8] * 36), Vector(0.0, -1.0f, 0.0f));
		Matrix pentagon = rotate * translatePenta * rotation8;
		Matrix viewPenta = viewMatrix * pentagon;
		glLoadMatrixf((GLfloat*)viewPenta.mVal);
		if (octavesArray[1] > 0.1f || octavesArray[8] > 0.8f)
		{
			glColor3f(1.0f, 1.0f, 0.0f);
			drawBrokenCone(2.0f + (octavesArray[8] * 1.5f), 1, 5);
		}
		Matrix rotateHemi = Matrix::makeRotateMatrix(90, Vector(-1.0f, 0.0f, 0.0f));
		Matrix translateHemi = Matrix::makeTranslationMatrix(-12.0f, -5.0f, -8.5f);
		Matrix rotationHemi = Matrix::makeRotateMatrix(m_rotation + (octavesArray[8] * 36), Vector(0.0, 1.0f, 0.0f));
		Matrix hemi = rotateHemi * translateHemi * rotationHemi;
		Matrix viewHemi = viewMatrix * hemi;
		glLoadMatrixf((GLfloat*)viewHemi.mVal);
		glColor3f(0.0f, 1.0f, 0.25f);
		drawBrokenHemi(1.0f, 3, 3);

		Matrix translateQuad = Matrix::makeTranslationMatrix(12.0f, 5.0f, 8.5f);
		Matrix rotation6 = Matrix::makeRotateMatrix(m_rotation * (octavesArray[6] - 2.0f), Vector(0.0f, -1.0f, 0.0f));
		Matrix square = rotate * translateQuad * rotation6;
		Matrix viewQuad = viewMatrix * square;
		glLoadMatrixf((GLfloat*)viewQuad.mVal);
		if (octavesArray[3] > 0.4f || octavesArray[2] > 0.3f || octavesArray[6] > 0.2f)
		{
			glColor3f(1.0f, 0.0f, 1.0f);
			drawBrokenCone(2.0f + octavesArray[2] + (octavesArray[6] * 1.5f), 1, 4);
		}
		Matrix translateCone = Matrix::makeTranslationMatrix(12.0f, -4.5f, -8.5f);
		Matrix rotationCone = Matrix::makeRotateMatrix(m_rotation * (octavesArray[6] - 2.0f), Vector(0.0f, 1.0f, 0.0f));
		Matrix cone = rotateHemi * translateCone * rotationCone;
		Matrix viewCone = viewMatrix * cone;
		glLoadMatrixf((GLfloat*)viewCone.mVal);
		glColor3f(1.0f, 0.0f, 0.0f);
		drawBrokenCone(1.0f, 1.0f, 4);

		Matrix translateTri = Matrix::makeTranslationMatrix(0.0f, 5.0f, 8.5f);
		Matrix rotation4 = Matrix::makeRotateMatrix(m_rotation + octavesArray[4] + octavesArray[5], Vector(0.0, -1.0f, 0.0f));
		Matrix triangle = rotate * translateTri * rotation4;
		Matrix viewTri = viewMatrix * triangle;
		glLoadMatrixf((GLfloat*)viewTri.mVal);
		if (octavesArray[4] > 0.09f || octavesArray[5] > 0.09f)
		{
			glColor3f(0.0f, 1.0f, 1.0f);
			drawBrokenCone(2.0f + (octavesArray[4] * 1.5f), 1, 3);
		}
		Matrix rotateSphere = Matrix::makeRotateMatrix(m_rotation + (octavesArray[3] * 60), Vector(0.0f, 0.0f, 1.0f));
		Matrix translateSphere = Matrix::makeTranslationMatrix(0.0f, 4.5f, 8.5f);
		Matrix sphere = rotate * translateSphere * rotation4 * rotateSphere;
		Matrix viewSphere = viewMatrix * sphere;
		glLoadMatrixf((GLfloat*)viewSphere.mVal);
		glColor3f(1.0f, 1.0f, 1.0f);
		drawSphere(1.0f, 3 + (octavesArray[3] * 3), 3 + (octavesArray[3] * 5));
	}

	void basicAudioVisualiser(const Matrix& viewMatrix)
	{
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);

		m_rotation += 0.3f;

		m_fmodSystem->update();
		m_musicChannel->getSpectrum(m_spectrumLeft, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		m_musicChannel->getSpectrum(m_spectrumRight, SPECTRUM_SIZE, 1, FMOD_DSP_FFT_WINDOW_RECT);
		//std::cout << " " << m_spectrumLeft[6] << "\t,\t" << m_spectrumRight[6] << std::endl;

		// Bars
		static int bars = 18;
		static float gap = 0.5f;
		static float length = 1.0f;
		static float height = 0.25f;
		static float depth = 1.0f;
		static float amplify = 3.0f;
		static float displaceForward = 0.3f;
		static float totalLength = (bars * length) + (gap * (bars - 1));

		// Stars
		static float starHeight = 0.0f;
		static int starSegment = 4;

		static float tempSpectrumTotal = 0.0f;
		static float tempSpectrum1 = 0.0f;
		static float tempSpectrum2 = 0.0f;

		std::cout << " " << octavesArray[4] << std::endl;

		for (int i = 0; i < bars; i++)
		{
			static float tempSkipForward = 0;
			if (i < bars * 0.5)
			{
				tempSkipForward = i * displaceForward;
			}
			else if (i > bars * 0.5)
			{
				tempSkipForward = (bars * displaceForward) - (i * displaceForward);
			}
			Matrix translate = Matrix::makeTranslationMatrix((-1 * (totalLength * 0.5f)) + (i * (length + gap)) + (length * 0.5f), 0.5f, 1.5f + tempSkipForward);
			Matrix model = translate;
			Matrix view = viewMatrix * model;
			glLoadMatrixf((GLfloat*)view.mVal);
			glColor3f(i * (1.0f / (float)bars), 0.0f, 1.0f);

			// Stars
			Matrix rotate = Matrix::makeRotateMatrix(90, Vector(1.0f, 0.0f, 0.0f));
			Matrix translateStar = Matrix::makeTranslationMatrix((-1 * (totalLength * 0.5f)) + (i * (length + gap)) + (length * 0.5f), 10.0f, 13.5f + tempSkipForward);
			Matrix rotationStar = Matrix::makeRotateMatrix(m_rotation + (i * 2), Vector(0.0f, -1.0f, 0.0f));
			Matrix stars = rotate * translateStar * rotationStar;
			Matrix viewStar = viewMatrix * stars;

			/*1 array = 43.06640625 Hz (22 050 Hz / 512 sample size)
			Note of B in each octave in brackets
			Octave 0 (30.86771) = [0]
			Octave 1 (61.73541) = [1]
			Octave 2 (123.4708) = [2]
			Octave 3 (246.9417) = [3] - [5]
			Octave 4 (493.8833) = [6] - [13]
			Octave 5 (987.7666) = [14] - [24]
			Octave 6 (1975.533) = [25] - [47]
			Octave 7 (3951.066) = [48] - [91]
			Octave 8 (7902.133) = [92] - [184]*/

			// Octave 0
			if (i == 0)
			{
				drawCube(length, ((m_spectrumLeft[i] + m_spectrumRight[i]) * 0.5) * amplify + height, depth);
				octavesArray[0] = (m_spectrumLeft[i] + m_spectrumRight[i]) * 0.5;

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, octavesArray[0]);
				drawBrokenCone(octavesArray[0], starHeight, starSegment);
			}
			else if (i == 1)
			{
				drawCube(length, ((m_spectrumLeft[i] + m_spectrumRight[i] + m_spectrumLeft[i - 1] + m_spectrumRight[i - 1]) / 4.0f) * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, (m_spectrumLeft[i] + m_spectrumRight[i] + m_spectrumLeft[i - 1] + m_spectrumRight[i - 1]) / 4.0f);
				drawBrokenCone((m_spectrumLeft[i] + m_spectrumRight[i] + m_spectrumLeft[i - 1] + m_spectrumRight[i - 1]) / 4, starHeight, starSegment);
			}
			// Octave 1
			else if (i == 2)
			{
				drawCube(length, ((m_spectrumLeft[i - 1] + m_spectrumRight[i - 1]) * 0.5) * amplify + height, depth);
				octavesArray[1] = (m_spectrumLeft[i - 1] + m_spectrumRight[i - 1]) * 0.5;

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, octavesArray[1]);
				drawBrokenCone(octavesArray[1], starHeight, starSegment);
			}
			else if (i == 3)
			{
				drawCube(length, ((m_spectrumLeft[i - 2] + m_spectrumRight[i - 2] + m_spectrumLeft[i - 1] + m_spectrumRight[i - 1]) / 4.0f) * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, ((m_spectrumLeft[i - 2] + m_spectrumRight[i - 2] + m_spectrumLeft[i - 1] + m_spectrumRight[i - 1]) / 4));
				drawBrokenCone(((m_spectrumLeft[i - 2] + m_spectrumRight[i - 2] + m_spectrumLeft[i - 1] + m_spectrumRight[i - 1]) / 4), starHeight, starSegment);
			}
			// Octave 2
			else if (i == 4)
			{
				drawCube(length, ((m_spectrumLeft[i - 2] + m_spectrumRight[i - 2]) * 0.5) * amplify + height, depth);
				octavesArray[2] = (m_spectrumLeft[i - 2] + m_spectrumRight[i - 2]) * 0.5;

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, octavesArray[2]);
				drawBrokenCone(octavesArray[2], starHeight, starSegment);
			}
			else if (i == 5)
			{
				tempSpectrumTotal = 0;
				tempSpectrum1 = 0;
				tempSpectrum1 = (m_spectrumLeft[i - 3] + m_spectrumRight[i - 3]) * 0.5;
				for (int j = 3; j <= 5; j++)
				{
					tempSpectrumTotal += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrumTotal = tempSpectrumTotal / 3;
				tempSpectrumTotal = (tempSpectrumTotal + tempSpectrum1) * 0.5;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal / 4, starHeight, starSegment);
			}
			// Octave 3
			else if (i == 6)
			{
				tempSpectrumTotal = 0;
				for (int j = 3; j <= 5; j++)
				{
					tempSpectrumTotal += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				octavesArray[3] = tempSpectrumTotal * 0.5;
				tempSpectrumTotal = tempSpectrumTotal / 3;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal, starHeight, starSegment);
			}
			else if (i == 7)
			{
				tempSpectrumTotal = 0;
				tempSpectrum1 = 0;
				tempSpectrum2 = 0;
				for (int j = 3; j <= 5; j++)
				{
					tempSpectrum1 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum1 = tempSpectrum1 / 3;

				for (int j = 6; j <= 13; j++)
				{
					tempSpectrum2 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum2 = tempSpectrum2 / 2;
				tempSpectrumTotal = (tempSpectrum1 + tempSpectrum2) * 0.5;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal / 4, starHeight, starSegment);
			}
			// Octave 4
			else if (i == 8)
			{
				tempSpectrumTotal = 0;
				for (int j = 6; j <= 13; j++)
				{
					tempSpectrumTotal += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				octavesArray[4] = tempSpectrumTotal * 0.5;
				tempSpectrumTotal = tempSpectrumTotal / 2;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal, starHeight, starSegment);
			}
			else if (i == 9)
			{
				tempSpectrumTotal = 0;
				tempSpectrum1 = 0;
				tempSpectrum2 = 0;
				for (int j = 6; j <= 13; j++)
				{
					tempSpectrum1 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum1 = tempSpectrum1 / 2;

				for (int j = 14; j <= 24; j++)
				{
					tempSpectrum2 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum2 = tempSpectrum2 / 2;
				tempSpectrumTotal = (tempSpectrum1 + tempSpectrum2) * 0.5;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal / 4, starHeight, starSegment);
			}
			// Octave 5
			else if (i == 10)
			{
				tempSpectrumTotal = 0;
				for (int j = 14; j <= 24; j++)
				{
					tempSpectrumTotal += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				octavesArray[5] = tempSpectrumTotal * 0.5;
				tempSpectrumTotal = tempSpectrumTotal / 2;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal, starHeight, starSegment);
			}
			else if (i == 11)
			{
				tempSpectrumTotal = 0;
				tempSpectrum1 = 0;
				tempSpectrum2 = 0;
				for (int j = 14; j <= 24; j++)
				{
					tempSpectrum1 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum1 = tempSpectrum1 / 2;

				for (int j = 25; j <= 47; j++)
				{
					tempSpectrum2 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum2 = tempSpectrum2 / 2;
				tempSpectrumTotal = (tempSpectrum1 + tempSpectrum2) / 2;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal / 4, starHeight, starSegment);
			}
			// Octave 6
			else if (i == 12)
			{
				tempSpectrumTotal = 0;
				for (int j = 25; j <= 47; j++)
				{
					tempSpectrumTotal += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				octavesArray[6] = tempSpectrumTotal * 0.5;
				tempSpectrumTotal = tempSpectrumTotal / 2;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal, starHeight, starSegment);
			}
			else if (i == 13)
			{
				tempSpectrumTotal = 0;
				tempSpectrum1 = 0;
				tempSpectrum2 = 0;
				for (int j = 25; j <= 47; j++)
				{
					tempSpectrum1 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum1 = tempSpectrum1 / 2;

				for (int j = 48; j <= 91; j++)
				{
					tempSpectrum2 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum2 = tempSpectrum2 / 2;
				tempSpectrumTotal = (tempSpectrum1 + tempSpectrum2) / 2;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal / 4, starHeight, starSegment);
			}
			// Octave 7
			else if (i == 14)
			{
				tempSpectrumTotal = 0;
				for (int j = 48; j <= 91; j++)
				{
					tempSpectrumTotal += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				octavesArray[7] = tempSpectrumTotal * 0.5;
				tempSpectrumTotal = tempSpectrumTotal / 2;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal, starHeight, starSegment);
			}
			else if (i == 15)
			{
				tempSpectrumTotal = 0;
				tempSpectrum1 = 0;
				tempSpectrum2 = 0;
				for (int j = 48; j <= 91; j++)
				{
					tempSpectrum1 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum1 = tempSpectrum1 / 2;
				for (int j = 92; j <= 184; j++)
				{
					tempSpectrum2 += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrum2 = tempSpectrum2 / 2;
				tempSpectrumTotal = (tempSpectrum1 + tempSpectrum2) / 2;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal / 4, starHeight, starSegment);
			}
			// Octave 8
			else if (i == 16)
			{
				tempSpectrumTotal = 0;
				for (int j = 92; j <= 184; j++)
				{
					tempSpectrumTotal += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				octavesArray[8] = tempSpectrumTotal * 0.5;
				tempSpectrumTotal = tempSpectrumTotal / 2; // 93
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal, starHeight, starSegment);
			}
			else if (i == 17)
			{
				tempSpectrumTotal = 0;
				tempSpectrum1 = 0;
				for (int j = 92; j <= 184; j++)
				{
					tempSpectrumTotal += m_spectrumLeft[j] + m_spectrumRight[j];
				}
				tempSpectrumTotal = tempSpectrumTotal / 2;
				tempSpectrum1 = (m_spectrumLeft[i + 168] + m_spectrumRight[i + 168]) * 0.5;
				tempSpectrumTotal = (tempSpectrumTotal + tempSpectrum1) * 0.5f;
				drawCube(length, tempSpectrumTotal * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, tempSpectrumTotal);
				drawBrokenCone(tempSpectrumTotal / 4, starHeight, starSegment);
			}
			else
			{
				drawCube(length, ((m_spectrumLeft[i + 167] + m_spectrumRight[i + 167]) * 0.5) * amplify + height, depth);

				glLoadMatrixf((GLfloat*)viewStar.mVal);
				glColor3f(1.0f, 1.0f, ((m_spectrumLeft[i + 167] + m_spectrumRight[i + 167]) * 0.5));
				drawBrokenCone(((m_spectrumLeft[i + 167] + m_spectrumRight[i + 167]) * 0.5), starHeight, starSegment);
			}
		}
	}

	void draw(const Matrix& viewMatrix)
	{
		//updateFmod();

		//drawAxis(viewMatrix); // Reenable this if you need to see the origin

		glLoadMatrixf((GLfloat*)viewMatrix.mVal);

		/*m_translate += 0.05f;
		m_rotation += 0.05f;*/

		//Matrix rotate1 = Matrix::makeRotateMatrix(m_rotation, Vector(0.0f, m_rotation, 0.0f));
		//Matrix translate1 = Matrix::makeTranslationMatrix(sin(m_translate) * 4.0, 0.0f, cos(m_translate) * 2.0f);
		/*Matrix translate1 = Matrix::makeTranslationMatrix(3.0f, 0.0f, 0.0f);
		Matrix scale1 = Matrix::makeScaleMatrix(1.0f, 1.0f, 1.0f);*/
		//Matrix scale1 = Matrix::makeScaleMatrix(1.0f, 2.0f, 1.0f);

		// NOTE on OpenGL Matrix model:
		// Screen = Proj * View * Model
		// Model = Transform(3rd) * Transform(2nd) * Transform(1st) (Transform could be Rotate, Scale, Translate, etc)

		// Perform model transformation
		//Matrix modelMatrix = translate1 * rotate1;
		//Matrix modelMatrix = scale1;

		// Convert model space to view space
		//Matrix viewSpaceMatrix = viewMatrix * modelMatrix;

		//glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

		// Enable wireframe mode
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		//drawTriangle();
		//drawRectangle();
		//drawSquare(2, 2, 0);
		//drawCircle(1.0f, 20);
		//drawCube(1, 1, 1);
		//drawPyramid();
		//drawCylinder(1, 1, 41);
		//drawCone(1, 1, 41);
		//drawSphere(1, 20, 20);
		//drawHemisphere(1, 7, 9);
		//drawEigthSphere(1.0f, 3, 5);
		//drawPokemon(viewMatrix);
		//drawTexture();

		//glColor3f(0.7f, 0.7f, 0.8f);
		arrayPlane(180, -10, 180, 90, 90, 0.05f, 0.5, 10);
		basicAudioVisualiser(viewMatrix);
		shapesAudioVisualiser(viewMatrix);

		// Steps to render:
		// Draw all opaque objects
		// Draw Additive/Multiply blend objects
		// Enable depth writing

		// Additive blending
		//glDisable(GL_BLEND);
		//glColor3f(0.5f, 0.0f, 0.0f);
		//drawCube(1, 1, 1);
		//glColor3f(0.0f, 0.0f, 0.5f);
		//drawCube(2, 2, 2);
		//glDepthMask(false);
		//Matrix translate = Matrix::makeTranslationMatrix(0.0f, 0.0f, 3.0f);
		//Matrix model = translate;
		//Matrix view = viewMatrix * model;
		//glLoadMatrixf((GLfloat*)view.mVal);
		//glEnable(GL_BLEND);
		////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA); // Alpha blend
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Add blend
		////glBlendFunc(GL_DST_COLOR, GL_ZERO); // Multiply blend
		//drawTexture();
		//glDepthMask(true);

		// Multiple disks flying in spiral
		/*for (int i = 0; i < 10; i++)
		{
		Matrix rotate = Matrix::makeRotateMatrix(m_rotation, Vector(0.0f, 2.0f, 0.0f));
		Matrix translate = Matrix::makeTranslationMatrix(sin(m_translate + i), 0.0f + i, cos(m_translate + i));
		Matrix modelMatrix = translate * rotate;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCylinder();
		}*/

		// Spinning cone
		/*Matrix modelMatrix2 = rotate1;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix2;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCone();*/
	}
};

#endif