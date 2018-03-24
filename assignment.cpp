//################BOUNCING OF BALL  in 3d####code showing 3d ball moving
#include<iostream>
#include<random>
#include<stdio.h>
#include<GL/glut.h>
#include<cmath>
#include<pthread.h>
#include<unistd.h>

#include <stdlib.h>
#include "imageloader.h"
//#include "obstacle.h"

/*#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif*/



using namespace std;
#define pi 3.142857
pthread_mutex_t lock;
bool stop = 1;
void stop_function(int k);
bool stop_ob = 0;
///##########image loader, code is taken from opensource
/*void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 27: //Escape key
            exit(0);
    }
}*/
int some_sphere=0;
int some_cube=0;
int some_cyl=0;
const int sphere = 7;
const int cube = 6;
const int cyl = 5;
GLuint _textureId;
//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                   //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

///############image loader exit here

//variable defined globally
/*double trans_y=0.0f;
double trans_x=0.0f;
double trans_z = 0.0f;
double roty=0.0f;
double rotx=0.0f;*/


int refreshmillis=30;
float ballXmax=1,ballYmax=1,ballYmin=-1,ballXmin=-1,ballZmin=-1,ballZmax=1;
GLdouble clipxleft, clipxright,clipytop,clipybottom,clipzin,clipzout;
void initialise_obs();

void InitGL()
{
    glClearColor(0.0, 0.0, 0.0,1.0);
    //glClearDepth(1.0f);
    
    //glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_LIGHT0);
    //glColorMaterial;
    
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
    Image* image = loadBMP("vtr.bmp");
    _textureId = loadTexture(image);
    delete image;
}

///############defination of ball class
class Ball{
  
public:   
        //constructor to seed intial values
        float ballradius;
        float ballX;
        float ballY;
        float ballZ;
        float speedx;
        float speedy;
        float speedz;
        float red;
        float green;
        float blue;
        Ball()
        {   
            //using random function to specify intial position
            //not able to generate ball from random position
         
            mt19937 generator((random_device())());         
            uniform_real_distribution<float> distribution(-1.0,1.0);
            ballradius=abs((distribution(generator))/4)+0.07;
            ballX=distribution(generator);
            ballY=distribution(generator);
            ballZ=distribution(generator);
            speedx=distribution(generator);
            speedy=distribution(generator);
            speedz=distribution(generator);
            red=abs(distribution(generator));
            green=abs(distribution(generator));
            blue=abs(distribution(generator));
            }
 
    float GetRadius(){return ballradius;}
    void SetRadius(float r){ballradius = r;}
 
    float GetxCenter(){return ballX;}
    void SetxCenter(float a){ballX = a;}
 
    float GetyCenter(){return ballY;}
    void SetyCenter(float b){ballY = b;}

    float GetzCenter(){return ballZ;}
    void SetzCenter(float b){ballZ = b;}
 
    float GetxSpeed(){return speedx;}
    void SetxSpeed(float c){speedx = c;}
 
    float GetySpeed(){return speedy;}
    void SetySpeed(float d){speedy = d;}

    float GetzSpeed(){return speedz;}
    void SetzSpeed(float d){speedz = d;}

      float GetrColour(){return red;}
    void SetrColour(float d){red = d;}

	   float GetgColour(){return green;}
    void SetgColour(float d){green = d;}

	   float GetbColour(){return blue;}
    void SetbColour(float d){blue = d;}
};
int no_of_ball;
Ball* ball;//=new Ball[no_of_ball];
//##########ball.h end here

//cube* Cube =new cube[1];
//sphere* Sphere=new sphere[1];
//cylider* CYL=new cylider[1];
//#######all the obstacles
double trans_y=0.0f; 
double trans_x=0.0f;
double trans_z = 0.0f;
double roty=0.0f; 
double rotx=0.0f;

//void obstacles1();  //sphere

GLfloat Ob1_rad[sphere];//={0.1,0.4,0.2,0.3,0.13};
GLfloat Ob1c_x[sphere];//={0.5,-0.9,0.8};
GLfloat Ob1c_y[sphere];//={};
GLfloat Ob1c_z[sphere];//={0.5,-0.9,0.8};
GLfloat Ob1c_red[sphere];//={1.0,0.0,0.0};
GLfloat Ob1c_green[sphere];//={0.0,0.9,0.0};
GLfloat Ob1c_blue[sphere];//={0.5,0.5,0.1};
//void obstacles2();  //cube
GLfloat Ob2_side[cube];//={0.2,0.1,0.25};
GLfloat Ob2c_x[cube];//={0.5,0.9,0.8};
GLfloat Ob2c_y[cube];//={0.5,0.9,0.8};
GLfloat Ob2c_z[cube];//={0.5,0.9,0.8};
//void obstacles4();  //cylinder
GLfloat Ob4_radius[cyl];//={0.15,0.09,0.13};
GLfloat Ob4_height[cyl];//={0.5,0.9,0.8};
GLfloat Ob4c_x[cyl];//={-0.5,-0.9,-0.8};
GLfloat Ob4c_y[cyl];//={-0.5,-0.9,-0.8};
GLfloat Ob4c_z[cyl];//={-0.5,-0.9,-0.8};
GLfloat Ob4c_red[cyl];//={0.5,0.9,0.8};
GLfloat Ob4c_green[cyl];//={0.5,0.9,0.8};
GLfloat Ob4c_blue[cyl];//={0.5,0.9,0.8};

void initialise_obs()
{	
		
	for(int a=0; a<sphere; a++)
	{	  mt19937 generator((random_device())());         
            uniform_real_distribution<float> distribution(-3.0,3.0);

		Ob1_rad[a]=abs((distribution(generator))/8.0)+0.07;
            Ob1c_x[a]=distribution(generator);
            Ob1c_y[a]=distribution(generator)/2.0;
            Ob1c_z[a]=distribution(generator);
            Ob1c_red[a]=abs(distribution(generator));
            Ob1c_green[a]=abs(distribution(generator));
            Ob1c_blue[a]=abs(distribution(generator));
	}
	for(int b=0; b<cube; b++)
	{	 mt19937 generator((random_device())());         
            uniform_real_distribution<float> distribution(-3.0,3.0); 

		Ob2_side[b]=abs((distribution(generator))/8)+0.07;
            Ob2c_x[b]=distribution(generator);
            Ob2c_y[b]=distribution(generator)/2.0;
            Ob2c_z[b]=distribution(generator);
	}
	for(int c=0; c<cyl; c++)
	{	 mt19937 generator((random_device())());         
            uniform_real_distribution<float> distribution(-3.0,3.0); 

		Ob4_radius[c]=abs((distribution(generator))/8)+0.07;
		Ob4_height[c]=abs((distribution(generator))/2.0);
            Ob4c_x[c]=distribution(generator);
            Ob4c_y[c]=abs(distribution(generator))-1.5;
            Ob4c_z[c]=distribution(generator);
            Ob4c_red[c]=abs(distribution(generator));
            Ob4c_green[c]=abs(distribution(generator));
            Ob4c_blue[c]=abs(distribution(generator));
	}
}
void obstacles1(int i)
{
    
     glMatrixMode(GL_MODELVIEW);     // To operate on model-view ma


   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
  // Rotate when user changes rotate_x and rotate_y
      glRotatef( rotx, 0.0, 1.0, 0.0 );
   glRotatef( roty, 1.0, 0.0, 0.0 );
   glTranslatef( trans_x, 0.0, 0.0 );
  glTranslatef( 0.0, trans_y, 0.0 );
  glTranslatef( 0.0, 0.0, trans_z);
   glTranslatef( 0.0f, 0.0f, -6.0f ); 
   glTranslatef( Ob1c_x[i], Ob1c_y[i], Ob1c_z[i]);
   
   glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _textureId);
    glColor3f(Ob1c_red[i], Ob1c_green[i], Ob1c_blue[i]); //coloring of the circle
    glScalef(1.0,1.0,1.0);
    // built-in (glut library) function , draw you a sphere.
    glTexCoord2f(1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glutSolidSphere( Ob1_rad[i],100,100);
    glDisable(GL_TEXTURE_2D);

    glEnd();

 }

void obstacles2(int i)
{
    
     glMatrixMode(GL_MODELVIEW);     // To operate on model-view ma


   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
  // Rotate when user changes rotate_x and rotate_y
     glRotatef( rotx, 0.0, 1.0, 0.0 );
   glRotatef( roty, 1.0, 0.0, 0.0 );
   glTranslatef( trans_x, 0.0, 0.0 );
  glTranslatef( 0.0, trans_y, 0.0 );
  glTranslatef( 0.0, 0.0, trans_z);
  glTranslatef( 0.0f, 0.0f, -6.0f ); 
   //glTranslatef( Ob2c_x[i], Ob2c_y[i], Ob2c_z[i]);
   glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _textureId);
    glScalef(1.0,1.0,1.0);
  //Multi-colored side - FRONT
  glBegin(GL_POLYGON);
 
  glColor3f( 1.0, 0.0, 0.0 );  glTexCoord2f(1.0f, 0.0f);   glVertex3f(  (Ob2c_x[i]+(Ob2_side[i]/2)) , (Ob2c_y[i]-(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );      // P1 is red
  glColor3f( 0.0, 1.0, 0.0 );   glTexCoord2f(1.0f, 1.0f);  glVertex3f(  (Ob2c_x[i]+(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );      // P2 is green
  glColor3f( 0.0, 0.0, 1.0 );   glTexCoord2f(0.0f, 1.0f);  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );      // P3 is blue
  glColor3f( 1.0, 0.0, 1.0 );   glTexCoord2f(0.0f, 0.0f);  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );      // P4 is purple
 
  glEnd();
 
  // White side - BACK
  glBegin(GL_POLYGON);
  glColor4f(   1.0,  1.0, 0.3, 0.3 );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(  (Ob2c_x[i]+(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)), (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(  (Ob2c_x[i]+(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)), (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)), (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)), (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glEnd();
 
  // Purple side - RIGHT
  glBegin(GL_POLYGON);
  glColor4f(  1.0,  0.0,  1.0, 1.0 );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( (Ob2c_x[i]+(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f( (Ob2c_x[i]+(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( (Ob2c_x[i]+(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)),  (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f( (Ob2c_x[i]+(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)),  (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glEnd();
 
  // Green side - LEFT
  glBegin(GL_POLYGON);
  glColor4f(   0.0,  1.0,  0.0, 1.0 );
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)),  (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)),  (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 0.0f);
  glEnd();
 
  // Blue side - TOP
  glBegin(GL_POLYGON);
  glColor4f(   0.0,  0.0,  1.0, 1.0 );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( (Ob2c_x[i]+(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)),  (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(  (Ob2c_x[i]+(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)),  (Ob2c_y[i]+(Ob2_side[i]/2)),  (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glEnd();
 
  // Red side - BOTTOM
  glBegin(GL_POLYGON);
  glColor4f(   1.0,  0.0,  0.0, 0.0 );
  glVertex3f(  (Ob2c_x[i]+(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(  (Ob2c_x[i]+(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)),  (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)),  (Ob2c_z[i]+(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( (Ob2c_x[i]-(Ob2_side[i]/2)), (Ob2c_y[i]-(Ob2_side[i]/2)), (Ob2c_z[i]-(Ob2_side[i]/2)) );
  glTexCoord2f(0.0f, 0.0f);
  glEnd();
    glEnd();
    glDisable(GL_TEXTURE_2D);

 }

void obstacles4(int i)
{
    
     glMatrixMode(GL_MODELVIEW);     // To operate on model-view ma


   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
  // Rotate when user changes rotate_x and rotate_y
      glRotatef( rotx, 0.0, 1.0, 0.0 );
   glRotatef( roty, 1.0, 0.0, 0.0 );
   glTranslatef( trans_x, 0.0, 0.0 );
  glTranslatef( 0.0, trans_y, 0.0 );
  glTranslatef( 0.0, 0.0, trans_z);
  glTranslatef( 0.0f, 0.0f, -6.0f ); 
  glRotatef(90.0,0.0,0.0,1.0);
  glRotatef(90.0,0.0,1.0,0.0);

   glTranslatef (-1*Ob4c_z[i], -1*Ob4c_x[i], Ob4c_y[i]);
  
    glColor3f(Ob4c_red[i], Ob4c_green[i], Ob4c_blue[i]);
    glScalef(1.0,1.0,1.0);
    // built-in (glut library) function , draw you a sphere.
    glBegin(GL_POLYGON);
    GLUquadricObj *obj = gluNewQuadric();
    gluCylinder(obj, Ob4_radius[i], Ob4_radius[i], Ob4_height[i], 100, 100);


    glEnd();

 }
void move_obs()
{
	
		 mt19937 generator((random_device())());         
            uniform_real_distribution<float> distribution(-3.0,3.0); 
	for(int h=0; h<cyl; h++)
	{	
		if(Ob4c_x[h]<ballXmax)
		Ob4c_x[h] = Ob4c_x[h] + 0.0025;
		else
		 {	Ob4c_x[h] = ballXmin;
			Ob4_radius[h]=abs((distribution(generator))/8)+0.07;
			Ob4_height[h]=abs((distribution(generator))/2.0);
                	Ob4c_y[h]=abs(distribution(generator))-1.5;
		        Ob4c_z[h]=distribution(generator);
		        Ob4c_red[h]=abs(distribution(generator));
		        Ob4c_green[h]=abs(distribution(generator));
		        Ob4c_blue[h]=abs(distribution(generator));
		  }
	}
	for(int h=0; h<cube; h++){
		if(Ob2c_x[h]<ballXmax)
		Ob2c_x[h] = Ob2c_x[h] + 0.0025;
		else
		 {	Ob2c_x[h] = ballXmin;
			
                	Ob2_side[h]=abs((distribution(generator))/8)+0.07;
            		Ob2c_y[h]=distribution(generator)/2.0;
            		Ob2c_z[h]=distribution(generator);
		  }
	}
	for(int l=0;l<sphere; l++){
		if(Ob1c_x[l]<ballXmax)
		Ob1c_x[l] = Ob1c_x[l] + 0.0025;
		else
		 {	Ob1c_x[l] = ballXmin;
			Ob1_rad[l]=abs((distribution(generator))/8)+0.07;
			Ob1c_y[l]=abs(distribution(generator))-1.5;
		        Ob1c_z[l]=distribution(generator);
		        Ob1c_red[l]=abs(distribution(generator));
		        Ob1c_green[l]=abs(distribution(generator));
		        Ob1c_blue[l]=abs(distribution(generator));
		  }}
}
//#######physics of ball-ball collision,ball wall collision and ball-obstacle.

//void obstacles2();  //cube


//void obstacles3();  //cylinder

 void physics_ball_obs_sphere(int i,int j)
{
    //---------Obs-1(SPHERE)----------

   GLfloat xdiff = ( Ob1c_x[j] - ball[i].GetxCenter() );
    GLfloat ydiff = ( Ob1c_y[j] - ball[i].GetyCenter());
    GLfloat zdiff = ( Ob1c_z[j] - ball[i].GetzCenter() );
       //cout<<xdiff<<"\t"<<ydiff<<"\t"<<zdiff<<"\n";
    GLfloat diff = pow((pow(xdiff, 2.0)+pow(ydiff, 2.0)+pow(zdiff, 2.0)),0.5);
    if(diff<=Ob1_rad[j]+ball[i].GetRadius())
    {
        GLfloat dot_product = (xdiff*(ball[i].GetxSpeed()) + ydiff*(ball[i].GetySpeed()) + zdiff*(ball[i].GetzSpeed()))/diff;       //  Dot prod * magnitude of diff        vel along loc = (dot_product/diff)(xdiff i + ydiff j + zdiff k)

        if (dot_product != 0)
        {
            float speedx = ball[i].GetxSpeed() - 2*((xdiff*dot_product)/diff);
            ball[i].SetxSpeed(speedx);
            float speedy = ball[i].GetySpeed() - 2*((ydiff*dot_product)/diff);
            ball[i].SetySpeed(speedy);
            float speedz = ball[i].GetzSpeed() - 2*((zdiff*dot_product)/diff);
            ball[i].SetzSpeed(speedz);
            ball[i].ballX = Ob1c_x[j]-(((Ob1_rad[j]+ball[i].GetRadius())*(xdiff))/diff);
            ball[i].ballY = Ob1c_y[j]-(((Ob1_rad[j]+ball[i].GetRadius())*(ydiff))/diff);
            ball[i].ballZ = Ob1c_z[j]-(((Ob1_rad[j]+ball[i].GetRadius())*(zdiff))/diff);
        }
    }
}
 void physics_ball_obs_cube(int i,int j)
{
    //---------Obs-2 (CUBE)------------

    GLfloat x2diff = (Ob2c_x[j] - ball[i].GetxCenter() );
    GLfloat y2diff = (Ob2c_y[j] - ball[i].GetyCenter());
    GLfloat z2diff = (Ob2c_z[j] - ball[i].GetzCenter());

    if(abs(x2diff) <=((Ob2_side[j]/2) + ball[i].GetRadius()) && ball[i].GetyCenter()<=(Ob2c_y[j] + (Ob2_side[j]/2) + (ball[i].GetRadius()/2)) && ball[i].GetyCenter()>=(Ob2c_y[j] - (Ob2_side[j]/2) - (ball[i].GetRadius()/2)) && ball[i].GetzCenter()<=(Ob2c_z[j] + (Ob2_side[j]/2) + (ball[i].GetRadius()/2)) && ball[i].GetzCenter()>=(Ob2c_z[j] -( Ob2_side[j]/2) - (ball[i].GetRadius()/2))){      
    float coll2x= Ob2c_x[j] - ((x2diff/abs(x2diff))*((Ob2_side[j]/2) + ball[i].GetRadius()));
    ball[i].SetxCenter(coll2x);
    float speed2x = -ball[i].GetxSpeed();
    ball[i].SetxSpeed(speed2x);   }
    else if(abs(y2diff) <=((Ob2_side[j]/2) + ball[i].GetRadius()) && ball[i].GetxCenter()<=(Ob2c_x[j] + (Ob2_side[j]/2) + (ball[i].GetRadius()/2)) && ball[i].GetxCenter()>=(Ob2c_x[j] - (Ob2_side[j]/2) - (ball[i].GetRadius()/2)) && ball[i].GetzCenter()<=(Ob2c_z[j] + (Ob2_side[j]/2) + (ball[i].GetRadius()/2)) && ball[i].GetzCenter()>=(Ob2c_z[j] - (Ob2_side[j]/2) - (ball[i].GetRadius()/2))){
    float coll2y = Ob2c_y[j] - ((y2diff/abs(y2diff))*((Ob2_side[j]/2) + ball[i].GetRadius()));
    ball[i].SetyCenter(coll2y);
    float speed2y = -ball[i].GetySpeed();
    ball[i].SetySpeed(speed2y);    }
    else if(abs(z2diff) <=((Ob2_side[j]/2) + ball[i].GetRadius()) && ball[i].GetyCenter()<=(Ob2c_y[j] + (Ob2_side[j]/2) + (ball[i].GetRadius()/2)) && ball[i].GetyCenter()>=(Ob2c_y[j] - (Ob2_side[j]/2) - (ball[i].GetRadius()/2)) && ball[i].GetxCenter()<=(Ob2c_x[j] + (Ob2_side[j]/2) + (ball[i].GetRadius()/2)) && ball[i].GetxCenter()>=(Ob2c_x[j] - (Ob2_side[j]/2) - (ball[i].GetRadius()/2))){
    float coll2z = Ob2c_z[j] - ((z2diff/abs(z2diff))*((Ob2_side[j]/2) + ball[i].GetRadius()));
    ball[i].SetzCenter(coll2z);
    float speed2z = -ball[i].GetzSpeed();
    ball[i].SetzSpeed(speed2z); }
}

 void physics_ball_obs_cyl(int i,int j)
{
    //---------Obs-4(CYLINDER)----------

    //cout<<"ndcks";
    GLfloat xdiff4 = ( Ob4c_x[j] - ball[i].GetxCenter());

    GLfloat ydiff4 = ( Ob4c_y[j] - ball[i].GetyCenter());
    GLfloat zdiff4 = ( Ob4c_z[j] - ball[i].GetzCenter());
    
    GLfloat detect = pow((pow(xdiff4, 2.0) + pow( zdiff4, 2.0)), 0.5);
    if(detect <= Ob4_radius[j]+ball[i].GetRadius() && detect >= Ob4_radius[j]&& ball[i].GetyCenter()<= Ob4c_y[j]+Ob4_height[j]+(ball[i].GetRadius()) && ball[i].GetyCenter()>=Ob4c_y[j]-(ball[i].GetRadius()))
    {   //cout<<"collision";
        GLfloat diff4 = pow((pow(xdiff4, 2.0)+pow(zdiff4, 2.0)),0.5);
        GLfloat dot_product4 = (xdiff4*ball[i].GetxSpeed() + zdiff4*ball[i].GetzSpeed())/diff4;
        float speed4x = ball[i].GetxSpeed() - 2*((xdiff4*dot_product4)/diff4);
        ball[i].SetxSpeed(speed4x);
        float speed4z = ball[i].GetzSpeed()- 2*((zdiff4*dot_product4)/diff4);
        ball[i].SetzSpeed(speed4z);
        float newballX1 = Ob4c_x[j]-((xdiff4*(Ob4_radius[j]+ball[i].GetRadius()))/detect);
        ball[i].SetxCenter(newballX1);
        float ball4Z = Ob4c_z[j]-((zdiff4*(Ob4_radius[j]+ball[j].GetRadius()))/detect);
        ball[i].SetzCenter(ball4Z);
    }
    else if(detect < Ob4_radius[j]+ball[i].GetRadius() && ball[i].GetyCenter()<= Ob4c_y[j]+Ob4_height[j]+ball[i].GetRadius() &&  ball[i].GetyCenter() >= Ob4c_y[j]+Ob4_height[j]|| detect < Ob4_radius[j] && ball[i].GetyCenter()>= Ob4c_y[j]-ball[i].GetRadius() &&  ball[i].GetyCenter() <= Ob4c_y[j])
    {   float speed4y = -ball[i].GetySpeed();
        float ball4Y = Ob4c_y[j]-ydiff4;
        ball[i].SetyCenter(ball4Y);
    }

}

/*bool grav = 0;
void gravity(bool c)
{
  if(c==1)
  {for(int i=0;i<no_of_ball;i++)
    ball[i].SetySpeed(ball[i].GetySpeed()-0.01);}
}*/

void *physics(void *j)
{
     
       // gravity(grav);
        int i=(long int)j;
        int flag[no_of_ball];
        while(true)
        {
            if(some_sphere==1)
            {for(int k=0;k<sphere;k++)
            {physics_ball_obs_sphere(i,k);}}
		        if(some_cube==1)
            {for(int h=0; h<cube; h++)
		        {physics_ball_obs_cube(i,h);}}
		        if(some_cyl==1)
            {for(int h=0; h<cyl; h++)
		        {physics_ball_obs_cyl(i,h);}}
       //other calls to physics of other obstacles
        float tempx = ball[i].GetxCenter();
            tempx+=(0.0005f)*ball[i].GetxSpeed();
            ball[i].SetxCenter(tempx);
             float tempy = ball[i].GetyCenter();
            tempy+=(0.0005f)*ball[i].GetySpeed();
        ball[i].SetyCenter(tempy);
             float tempz = ball[i].GetzCenter();
            tempz+=(0.0005f)*ball[i].GetzSpeed();
            ball[i].SetzCenter(tempz);
            if(ball[i].GetxCenter()>ballXmax-ball[i].GetRadius())
            {   ball[i].SetxCenter(ballXmax-ball[i].GetRadius());
                //ball[i].SetxCenter(xcenter[i]);
                //xspeed[i]=-ball[i].GetxSpeed();
                ball[i].SetxSpeed(-1*ball[i].GetxSpeed());
                flag[i]=1;
            }
            else if(ball[i].GetxCenter()<ballXmin+ball[i].GetRadius()){
                ball[i].SetxCenter(ballXmin+ball[i].GetRadius());
                //ball[i].SetxCenter(xcenter[i]);
                //xspeed[i]=-ball[i].GetxSpeed();
                ball[i].SetxSpeed(-1*ball[i].GetxSpeed());
                flag[i]=1;
            }
            if(ball[i].GetyCenter()>ballYmax-ball[i].GetRadius()){
            ball[i].SetyCenter(ballYmax-ball[i].GetRadius());
                //ball[i].SetxCenter(xcenter[i]);
                //xspeed[i]=-ball[i].GetxSpeed();
                ball[i].SetySpeed(-1*ball[i].GetySpeed());
                flag[i]=1;
            }
            else if(ball[i].GetyCenter()<ballYmin+ball[i].GetRadius()){
             ball[i].SetyCenter(ballYmin+ball[i].GetRadius());
                //ball[i].SetxCenter(xcenter[i]);
                //xspeed[i]=-ball[i].GetxSpeed();
                ball[i].SetySpeed(-1*ball[i].GetySpeed());
                flag[i]=1;
                //cout<<"hello";
            }
        if(ball[i].GetzCenter()>ballZmax-ball[i].GetRadius()){
            ball[i].SetzCenter(ballZmax-ball[i].GetRadius());
                //ball[i].SetxCenter(xcenter[i]);
                //xspeed[i]=-ball[i].GetxSpeed();
        //cout<<ball[i].GetzCenter()<<"\t"<<ball[i].GetzSpeed()<<endl;
                ball[i].SetzSpeed(-1*ball[i].GetzSpeed());
        //cout<<ball[i].GetzCenter()<<"\t"<<ball[i].GetzSpeed()<<endl;
                flag[i]=1;
            }
            else if(ball[i].GetzCenter()<ballZmin+ball[i].GetRadius()){
             ball[i].SetzCenter(ballZmin+ball[i].GetRadius());
                //ball[i].SetxCenter(xcenter[i]);
                //xspeed[i]=-ball[i].GetxSpeed();
                ball[i].SetzSpeed(-1*ball[i].GetzSpeed());
                flag[i]=1;
                //cout<<"hello";
            }
            
            //if(flag[0]!=1 && flag[1]!=1)
            {      
                    for(int k=i+1;k<no_of_ball;k++)
                    {
                       
                        float p=((ball[i].ballX-ball[k].ballX)*(ball[i].ballX-ball[k].ballX))+((ball[i].ballY-ball[k].ballY)*(ball[i].ballY-ball[k].ballY))+((ball[i].ballZ-ball[k].ballZ)*(ball[i].ballZ-ball[k].ballZ));
                       
                        pthread_mutex_lock(&lock);
                        //cout<<"hi"<<p<<endl;

                        //cout<<i<<endl<<k<<endl
///////////////##########code physics of ball
                        if(4*(ball[i].GetRadius())*(ball[k].GetRadius())>=p)
                        {
                            //cout<<"collision";
                            GLfloat xdiff = (ball[i].GetxCenter() - ball[k].GetxCenter());
                            GLfloat ydiff = (ball[i].GetyCenter() - ball[k].GetyCenter());
                            GLfloat zdiff = (ball[i].GetzCenter() - ball[k].GetzCenter());
                                //GLfloat zdiff = ( Ob1c_z - ballZ );
                            GLfloat diff = pow((pow(xdiff, 2.0)+pow(ydiff, 2.0)+pow(zdiff, 2.0)),0.5);
                            //float normalizex=(xdiff)/(diff);
                            //float normalizey=(ydiff)/(diff);
                            /*float intialSpeedi= sqrt((pow(ball[i].GetxSpeed(), 2.0)+pow(ball[i].GetySpeed(), 2.0)));
                            float intialSpeedk= sqrt((pow(ball[k].GetxSpeed(), 2.0)+pow(ball[k].GetySpeed(), 2.0)));*/

                            float dot_producti = xdiff*(ball[i].GetxSpeed()) + ydiff*(ball[i].GetySpeed()) + zdiff*(ball[i].GetzSpeed());
                            float dot_productk = xdiff*(ball[k].GetxSpeed()) + ydiff*(ball[k].GetySpeed()) +  zdiff*(ball[k].GetzSpeed());
                            //cout<<dot_producti<<"hello"<<endl;
                            float newvelocityxi=ball[i].GetxSpeed()-(dot_producti*xdiff)/(diff*diff)+(dot_productk*xdiff)/(diff*diff);//(dot_productk*xdiff)/(diff*diff);
                            //cout<<ball[i].GetxCenter()<<endl;
                ball[i].SetxSpeed(newvelocityxi);
                //cout<<ball[i].GetzSpeed()<<endl;
                float newvelocityzi=ball[i].GetzSpeed()-(dot_producti*zdiff)/(diff*diff)+(dot_productk*zdiff)/(diff*diff);//(dot_productk*xdiff)/(diff*diff);
                            //cout<<ball[i].GetxCenter()<<endl;
                ball[i].SetzSpeed(newvelocityzi);
                //cout<<ball[i].GetzSpeed()<<endl;
                float newx1 = ball[k].GetxCenter()+(((ball[i].GetRadius()+ball[k].GetRadius())*xdiff)/diff);
                float newy1 = ball[k].GetyCenter()+(((ball[i].GetRadius()+ball[k].GetRadius())*ydiff)/diff);
                float newz1 = ball[k].GetzCenter()+(((ball[i].GetRadius()+ball[k].GetRadius())*zdiff)/diff);
                ball[i].SetxCenter (newx1 );
                ball[i].SetyCenter (newy1);
                ball[i].SetzCenter (newz1);
                          
                            float newvelocityyi=ball[i].GetySpeed()-(dot_producti*ydiff)/(diff*diff)+(dot_productk*ydiff)/(diff*diff);
                            ball[i].SetySpeed(newvelocityyi);
               
                           
                            float newvelocityxk=ball[k].GetxSpeed()-(dot_productk*xdiff)/(diff*diff)+(dot_producti*xdiff)/(diff*diff);//(dot_productk*ydiff)/(diff*diff);
                            ball[k].SetxSpeed(newvelocityxk);
                  float newx2 = ball[i].GetxCenter()-(((ball[i].GetRadius()+ball[k].GetRadius())*xdiff)/diff);
                float newy2 = ball[i].GetyCenter()-(((ball[i].GetRadius()+ball[k].GetRadius())*ydiff)/diff);
                float newz2 = ball[i].GetzCenter()-(((ball[i].GetRadius()+ball[k].GetRadius())*zdiff)/diff);
                ball[k].SetxCenter (newx2 );
                ball[k].SetyCenter (newy2);
                ball[k].SetzCenter (newz2);
                           
                            float newvelocityyk=ball[k].GetySpeed()-(dot_productk*ydiff)/(diff*diff)+(dot_producti*ydiff)/(diff*diff);//(dot_producti*ydiff)/(diff*diff);
                            ball[k].SetySpeed(newvelocityyk);

                 //cout<<ball[k].GetzSpeed()<<endl;
                 float newvelocityzk=ball[k].GetzSpeed()-(dot_productk*zdiff)/(diff*diff)+(dot_producti*zdiff)/(diff*diff);//(dot_productk*ydiff)/(diff*diff);
                            ball[k].SetzSpeed(newvelocityzk);
                //cout<<ball[k].GetzSpeed()<<endl;
                        }
                        pthread_mutex_unlock(&lock);
                    }
            }
            usleep(100);
        }
}
//###############ball obstacle physics


////////#########physics of ball end here
GLfloat angleX = 0.0f;
//-----------------------------------------------------------
//Obstacles
//-----------------------------------------------------------
/*void add_ball()
{
    mt19937 generator((random_device())());         
    uniform_real_distribution<float> distribution(-1.0,1.0);
    no_of_ball++;
    Ball* ball_new=new Ball[no_of_ball];
    Ball temp;
    
    int i;
    for(i=0;i<no_of_ball-1;i++)
    {
        ball_new[i]=ball[i];
    }
    ball_new[no_of_ball-1]=temp;
    ball=ball_new;
    ball_new=NULL;
    delete ball_new;
    ball[no_of_ball-1].SetxSpeed(distribution(generator));
    ball[no_of_ball-1].SetySpeed(distribution(generator));
    ball[no_of_ball-1].SetzSpeed(distribution(generator));
}

void remove_ball()
{
    //no_of_balls--;
    Ball* ball_new=new Ball[no_of_ball-1];
    int i=0;
    int l=0;
    for(i=0;i<no_of_ball-1;i++)
    {
        ball_new[i]=ball[i];
    }
    ball=ball_new;
    ball_new=NULL;
    delete ball_new;
    no_of_ball--;
}*/


void MouseWheel();
void display(void);
void reshape(int x, int y);
//void physics_ball_wall();
//void physics_ball_obs();
/*void idle(void)
{
 
    xRotated += 0.01;
    yRotated += 0.01;
    zRotated += 0.01;
    display();
}*/
/* Global variables */
//char title[] = "3D Shapes";
 
/* Initialize OpenGL Graphics */
 
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
/*void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glClear(GL_COLOR_BUFFER_BIT);  //clear the color buffer
    //glBegin(GL_POINTS);         
    glMatrixMode(GL_MODELVIEW); //tp operate on modelview
    glLoadIdentity();             //reset model view matrix
    for(int i=0;i<2;i++){
    glPushMatrix();
    glTranslatef(0.0,0.0,-3.0);
    glTranslatef(ball[i].GetxCenter(),ball[i].GetyCenter(),ball[i].GetzCenter()); //shows the intial position of ball
    glColor3f(0.0f,0.0f,1.0f);
    glScalef(1.0,1.0,1.0);
    glutWireSphere(ball[i].GetRadius(),100,100);
    glPopMatrix();}
    glutSwapBuffers(); //Swap the front and back frame buffers(double buffering)
}*/
int sel_ball=0;
bool speed_mode = 0;
GLfloat temp_colour[3];
void selection()
{	temp_colour[0]=ball[sel_ball].GetrColour();
	temp_colour[1]=ball[sel_ball].GetgColour();
	temp_colour[2]=ball[sel_ball].GetbColour();
	ball[sel_ball].SetrColour(0.0);
	ball[sel_ball].SetgColour(0.0);
	ball[sel_ball].SetbColour(0.0);
}
void restore_colour()
{
	ball[sel_ball].SetrColour(temp_colour[0]);
	ball[sel_ball].SetgColour(temp_colour[1]);
	ball[sel_ball].SetbColour(temp_colour[2]);
}
void change_speed(int d)
{
	if(speed_mode==1)
	{	if(d==1)
		{	GLfloat temp_speedx = ball[sel_ball].GetxSpeed()*1.1;
			GLfloat temp_speedy = ball[sel_ball].GetySpeed()*1.1;
			GLfloat temp_speedz = ball[sel_ball].GetzSpeed()*1.1;
			ball[sel_ball].SetxSpeed(temp_speedx);
			ball[sel_ball].SetySpeed(temp_speedy);
			ball[sel_ball].SetzSpeed(temp_speedz);
		}
		else
		if(d==0)
		{	GLfloat temp_speedx = ball[sel_ball].GetxSpeed()*0.9;
			GLfloat temp_speedy = ball[sel_ball].GetySpeed()*0.9;
			GLfloat temp_speedz = ball[sel_ball].GetzSpeed()*0.9;
			ball[sel_ball].SetxSpeed(temp_speedx);
			ball[sel_ball].SetySpeed(temp_speedy);
			ball[sel_ball].SetzSpeed(temp_speedz);
		}
	}
}


// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
 
  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT)
	{if (trans_x>-2.35)   
	{ trans_x -= 0.05;
		cout<<trans_x<<endl;}}
  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
      {if (trans_x<2.35)
	{trans_x += 0.05;
    cout<<trans_x<<endl;}}

  else if (key == GLUT_KEY_UP)
	{if (trans_y>-0.2)    
	{trans_y -= 0.05;
 cout<<trans_y<<endl;}}
  else if (key == GLUT_KEY_DOWN)
	{if (trans_y<0.2)    
	{trans_y += 0.05;
 cout<<trans_y<<endl;} }

 
  //  Request display update
  glutPostRedisplay();
 
}
/*
void processMouse(int button, int state, int x, int y)
{
       else if (key == GLUT_UP)
    trans_z -= 0.05;

    else if (key == GLUT_DOWN)
    trans_z += 0.05;

      //  Request display update
  glutPostRedisplay();
 

}*/


void mouseWheel(int button, int dir, int x, int y)
{
    if (button == 3)
    {
	if (trans_z>-0.5)
       {trans_z -= 0.05;    
       cout<<trans_z<<endl;}     // Zoom in
    }
    if (button == 4)
    {
	if (trans_z<5.1)
         {trans_z+= 0.05;
         cout<<trans_z<<endl;}        // Zoom out
    }

     glutPostRedisplay();
}
int m;
int l;
void processMousePassiveMotion(int x, int y) {
            // User must press the SHIFT key to change the
            // rotation in the X axis
           
            {
                    // setting the angle to be relative to the mouse
                    // position inside the window
                     if(m < x) rotx+=0.5;
                     else if(m > x) rotx-=0.5;
                     else if(l > y) roty-=0.5;
                     else if(l < y) roty+=0.5;

                     m = x;
                     l = y;

            }
            glutPostRedisplay();
}
//#########special key function end here

//void draw_cube();


void handleKeypress(unsigned char key, int x, int y) {
	//if(key == '0')
	//stop_ob = !stop_ob;
	//cout<<"sssssssssssssss";
    switch (key) {
        case 27: 
            exit(0);
		break;
	case 32:
	stop_function( stop);
	break;
	case 111:
	stop_ob = !stop_ob;
	break;
	case 115://s
	{	if(speed_mode==1)
		restore_colour();
		else
		selection();
		speed_mode = !speed_mode;
	 
	}
	break;
	case 110://n
	{
	restore_colour();
	if(sel_ball<no_of_ball-1)	
	sel_ball++;
	else
	sel_ball = 0;
	selection();
    	}
	break;
	case 117://u
	change_speed(1);
	break;
	case 100://d
	change_speed(0);
	break;
	/*case 43://+
	add_ball();
	break;
	case 45://-
	remove_ball();
	break;*/
  case 49:
  some_sphere=!some_sphere;
  break;
  case 50:
  some_cube=!some_cube;
  break;
    case 51:
  some_cyl=!some_cyl;
  break;
  /*case 103:
  {grav = !grav;
  }*/
	}
}
GLfloat stop_arr[2][3];
void stop_function( int k)
{	
		if(k==1)
		{for(int p=0; p<no_of_ball; p++)
		{	stop_arr[p][0] = ball[p].GetxSpeed();
			stop_arr[p][1] = ball[p].GetySpeed();
			stop_arr[p][2] = ball[p].GetzSpeed();
			ball[p].SetxSpeed(0.0);
			ball[p].SetySpeed(0.0);
			ball[p].SetzSpeed(0.0);
		}}
		else if(k==0)
		{	for(int o=0; o<no_of_ball; o++)
		{	ball[o].SetxSpeed(stop_arr[o][0]);
			ball[o].SetySpeed(stop_arr[o][1]);
			ball[o].SetzSpeed(stop_arr[o][2]);
		}}	
		stop = !stop;
		//cout<<stop;
}

/*void obstacles1();  //sphere
GLfloat Ob1_rad = 0.3f;
GLfloat Ob1c_x = 0.0f;
GLfloat Ob1c_y = 0.0f;
GLfloat Ob1c_z = 0.0f;
void obstacles2();  //cube
GLfloat Ob2_side = 0.3f;
GLfloat Ob2c_x = 0.0f;
GLfloat Ob2c_y = 0.0f;
GLfloat Ob2c_z = 0.0f;

void obstacles3();  //cone
GLfloat Ob3_rad = 0.2f;
GLfloat Ob3_height = 0.3f;
GLfloat Ob3c_x = 0.5f;
GLfloat Ob3c_y = 0.5f;
GLfloat Ob3c_z = 0.5f;*/

/////start drawing box 
void draw_cube()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // Reset transformations
  glLoadIdentity();
  glRotatef( rotx, 0.0, 1.0, 0.0 );
  glRotatef( roty, 1.0, 0.0, 0.0 );
  glTranslatef( trans_x, 0.0, 0.0 );
  glTranslatef( 0.0, trans_y, 0.0 );
  glTranslatef( 0.0, 0.0, trans_z);
  // Other Transformations
    glTranslatef( 0.0f, 0.0f, -6.0f );      // Not included
       // Not included
    glRotatef(180.0f,0.0f,1.0f,0.0f);
 
  // Other Transformations
   glScalef( 1.0, 1.0, 1.0 );          // Not included
 
 /* //Multi-colored side - FRONT
  glBegin(GL_POLYGON);
 
  glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
  glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 is green
  glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 is blue
  glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple
 
  glEnd();
 */
  // White side - BACK
 /*  GLfloat mat_color[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_color);
     glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
*/
    
    
    //Bottom
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _textureId);  
  glBegin(GL_POLYGON);
  glColor4f(   255,250,250  , 1.0 );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( clipxright, clipybottom, clipzout );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f( clipxright,  clipytop, clipzout );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( clipxleft,  clipytop, clipzout );
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f( clipxleft, clipybottom, clipzout );
  glEnd();
 
  // Purple side - RIGHT

  glBegin(GL_POLYGON);
  glColor4f( 255,223,0, 1.0 );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( clipxright, clipybottom, clipzin );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f( clipxright,  clipytop, clipzin );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( clipxright,  clipytop,  clipzout );
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f( clipxright, clipybottom,  clipzout );
  glEnd();
 
  // Green side - LEFT
    
  glBegin(GL_POLYGON);
  glColor4f(   255,223,0, 1.0 );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( clipxleft, clipybottom,  clipzout );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f( clipxleft,  clipytop,  clipzout );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( clipxleft,  clipytop, clipzin );
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f( clipxleft, clipybottom, clipzin );
  glEnd();

  // Blue side - TOP
  glBegin(GL_POLYGON);
  glColor4f(   250,250,210, 1.0 );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( clipxright,  clipytop,  clipzout );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(  clipxright,  clipytop, clipzin );
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f( clipxleft,  clipytop, clipzin );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( clipxleft,  clipytop,  clipzout );
  glEnd();
 
  // Red side - BOTTOM
  glBegin(GL_POLYGON);
  glColor4f(   210,105,30, 0.0 );
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( clipxright, clipybottom, clipzin );
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f( clipxright, clipybottom,  clipzout );
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f( clipxleft, clipybottom,  clipzout );
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f( clipxleft, clipybottom, clipzin );
  glEnd();
   glDisable(GL_TEXTURE_2D);
}
//##############finish drawing box

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width,GLsizei height){
    if(height==0)
        height=1;
    float aspect=(float)width/(float)height;
    glViewport(0,0,width,height); //set the viewport to cover new window
    glMatrixMode(GL_PROJECTION);  //to operate on projection matrix
    glLoadIdentity();      //reset the projection matrix
    //to avoid any distortion in shape of ball
    if(width>=height){
        clipxleft=-3.0*aspect;
        clipxright=3.0*aspect;
        clipybottom=-1.5;
        clipytop=1.5;
        clipzin = -3.0;
        clipzout = 3.0;
    }
    else{
        clipxleft=-3.0;
        clipxright=3.0;
        clipybottom=-1.5/aspect;
        clipytop=1.5/aspect;
        clipzin = -3.0;
        clipzout = 3.0;
    }
    //gluOrtho2D(clipxleft,clipxright,clipybottom,clipytop);
    gluPerspective(45.0f, aspect, 1.0f, 50.0f);
    ballXmin=clipxleft+ball[0].GetRadius();
    //cout<<"hi";
    ballXmax=clipxright-ball[0].GetRadius();
    ballYmin=clipybottom+ball[0].GetRadius();
    //cout<<ballXmax<<endl;
    ballYmax=clipytop-ball[0].GetRadius();
    ballZmin=clipzin+ball[0].GetRadius();
    ballZmax=clipzout-ball[0].GetRadius();
}
const GLfloat light_ambient[]  = { 0, 0, 0, 1};
const GLfloat light_diffuse[]  = { 1, 1, 1, 1 };
const GLfloat light_specular[] = { 1, 1, 1, 1 };
const GLfloat light_position[] = { 1, 1, 1, 0 };

const GLfloat mat_ambient[]    = { 0.2, 0.2, 0.2, 1.0 };
const GLfloat mat_diffuse[]    = { 0.2, 0.2, 0.2, 1.0 };
const GLfloat mat_specular[]   = { 0, 0, 0, 1 };
//const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 200 };

//open source

/*float[] ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
float[] diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
float[] specular ={ 1.0f, 1.0f, 1.0f, 1.0f };
float[] position = { 200.0f, 300.0f, 100.0f, 0.0f };*/
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
        draw_cube();
	
        /*for (int i = 0; i < 2; ++i)
        {
            //obstacles1();
            //obstacles2();
            obstacles1(i);
        }*/
        if(some_sphere==1)
        {for (int i = 0; i < sphere; ++i)
        {
            
            obstacles1(i);
        }}
	       if(some_cube==1)
         {for (int i = 0; i < cube; ++i)
        {
            
            obstacles2(i);
        }}
	       if(some_cyl==1)
        {for (int i = 0; i < cyl; ++i)
        {
            
            obstacles4(i);
        }}
	if(stop_ob == 0)	
	{move_obs();}
        /*for (int i = 0; i < 1; ++i)
        {
           
            obstacles4(i);
        }*/
        //obstacles1();
        //obstacles2();
        //obstacles4();
   /* Image* image = loadBMP("planets.bmp");
    _textureId = loadTexture(image);
    delete image;*/
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   glRotatef( rotx, 0.0, 1.0, 0.0 );
   glRotatef( roty, 1.0, 0.0, 0.0 );
  // Rotate when user changes rotate_x and rotate_y
   glTranslatef( trans_x, 0.0, 0.0 );
  glTranslatef( 0.0, trans_y, 0.0 );
  glTranslatef( 0.0, 0.0, trans_z);
   glTranslatef(0.0f, 0.0f, -6.0f);  // Move right and into the screen
   //GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
        //GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };

    glShadeModel(GL_SMOOTH);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   
   //glEnable(GL_DEPTH_TEST);
    //glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
   for(int i=0;i<no_of_ball;i++){
    glPushMatrix();
    
    glTranslatef(0.0,0.0,0.0);
    glTranslatef(ball[i].GetxCenter(),ball[i].GetyCenter(),ball[i].GetzCenter()); //shows the intial position of ball
    glColor3f(ball[i].red,ball[i].green,ball[i].blue);
    
    //glColor3f(1.0f,0.0f,0.0f);
    /*GLfloat mat_color[] = {ball[i].red, ball[i].green, ball[i].blue, 1.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);*/
        
         //GL_QUADRATIC_ATTENUATION;
    GLfloat mat_color[] = {ball[i].red, ball[i].green, ball[i].blue, 1.0};
    GL_LIGHT_MODEL_TWO_SIDE;
    GL_LIGHT_MODEL_LOCAL_VIEWER;
    GL_LIGHT_MODEL_COLOR_CONTROL;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_color);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        
    
//open source 
   /* Gl.glLightfv(Gl.GL_LIGHT0, Gl.GL_AMBIENT, ambient);
    Gl.glLightfv(Gl.GL_LIGHT0, Gl.GL_DIFFUSE, diffuse);
    Gl.glLightfv(Gl.GL_LIGHT0, Gl.GL_SPECULAR, specular);
    Gl.glLightfv(Gl.GL_LIGHT0, Gl.GL_POSITION, position);
    Gl.glEnable(Gl.GL_LIGHT0);
    Gl.glEnable(Gl.GL_LIGHTING);*/
    glScalef(1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glutSolidSphere(ball[i].GetRadius(),100,100);
    glTexCoord2f(1.0f, 0.0f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();}
   
    glutSwapBuffers(); //Swap the front and back frame buffers(double buffering)
    //ballY+=speedy;
    //ballX+=speedx;
    //ballZ+=speedz;

   //glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

    //physics_ball_wall();
    //physics_ball_obs();
}
//obstaclebox end here
void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(refreshmillis,Timer,0);
}
int windowWidth=640;
int windowHeight=480;
int windowPosX=50;
int windowPosY=50;


int main(int argc, char** argv)
{
    //int no_of_ball;
    //cin>>no_of_ball;
    //created object of class ball
    initialise_obs();
    //cout<<no_of_ball<<endl;
    no_of_ball=atoi(argv[1]);
    ball=new Ball[no_of_ball];
    pthread_t my_thread[100];
    for (int id = 0; id <no_of_ball; id++)
    {
        /* code */
        //cout<<"hello"<<endl;
        int ret =  pthread_create(&my_thread[id], NULL,&physics, (void*)id);
                if(ret != 0) {
                        printf("Error: pthread_create() failed\n");
                        exit(EXIT_FAILURE);
                        }
    }
    //cout<<ballXmax<<" "<<ballXmin<<endl;
    glutInit(&argc,argv);   //intialize glut
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);  //enable double buffered mode
    glutInitWindowSize(windowWidth,windowHeight);  //set intial window size
    glutInitWindowPosition(windowPosX,windowPosY);//set intial window position

    glutCreateWindow("Screen Saver");
    glEnable(GL_DEPTH_TEST);
    //creating title of the window
    //setcenter();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST); //register callback handler to repaint the window
    //glutFullScreen();
    //register callback handler to resize the window
    glutTimerFunc(0, Timer ,0); //first timer call immediately
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseWheel);
    glutMotionFunc(processMousePassiveMotion);
    glutKeyboardFunc(handleKeypress);
    //glEnable(GL_DEPTH_TEST);
    InitGL();
    //cout<<ballXmax<<" "<<ballXmin<<endl;
    //printf("hello world\n");        //opengl intialization
    glutMainLoop();   //enter the infinite event processing loop
    return 0;
}


