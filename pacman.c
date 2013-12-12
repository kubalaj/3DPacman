/*
 *  3D Pacman
 *  By Paul Kubala 
 *  'p' to switch projections
 *  's' to start/stop light
 *  'a' to toggle axes
 *  '0' snaps angles to 0,0
 *  arrows to rotate the world
 *  PgUp/PgDn zooms in/out
 *  -/+    change light elevation
 */
#include "CSCIx229.h"
#include <stdbool.h>
#include <math.h>
//#include "SDL/SDL.h"
//#include "SDL/SDL_mixer.h"

int axes=1;       //  Display axes
int mode=0;       //  Shader mode
int move=1;       //  Move light
int proj=2;       //  Projection type
int th=5;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=100;       //  Field of view (for perspective)
double spc=1;     //  Specular intensity
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
double scale=1; //  Image scale
int zh=90;        //  Light azimuth
float Ylight=2;   //  Light elevation
int cherry;          //  Object display list
int pacman;
double horizon = 0;
double vertical = 0; // The player moves
int map;
int title;
double title_rot = -5;
bool increase = true;
double east_line = 0;
double collider_y_a = 1; 
double collider_y_b = 0;
// Collison
// Distance between Objects
double d = 0;
// Position of our first point
double one_x;
double one_y;
double one_z;
// Position of our second point 
double two_x = 0;
double two_y = 0;
double two_z = 0;
bool map_hit = false;

// Hold the radius of points
const RADIUS = 10;




float RGBA[4] = {1,1,1,1};  //  Colors
double speed = .25;
bool north = false;
bool south = false;
bool east = false;
bool west = false;
bool camera_switch = false;
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */




static bool is_not_legal(double x, double y, double bx1, double by1, double bx2, double by2, 
                                 double bx3, double by3, double bx4, double by4)
{
   if((x < bx2 && x > bx1 && x > bx3 && x < bx4) && (y > by2 && y > by1 && y < by3 && y<by4)){
      return true;
   } 
   else{ 
      return false;}
    
}





void pacman_loader(int obj)
{  
   //bool is_collision = collision(horizon-3, vertical);
   glColor3f(1,0,0);
   


      if(camera_switch==false)
         glTranslated(horizon,vertical,0);
      else
         glTranslated(vertical,horizon,0);
   
 

   glScalef(.055,.055,.055);
   //Rotate the Pacman
   if(north == true)
      glRotatef(90,0,0,1);
   if(south == true)
      glRotatef(-90,0,0,1);
   if(west == true)
      glRotatef(180,0,1,0);
   if(camera_switch == true){
      glRotatef(90,1,0,0);
      glRotatef(90,0,0,1);
   }
   glCallList(obj);


}
void cherry_loader(int obj)
{
   glTranslated(-3,0,0);
   glScaled(.1,.1,.1);
   glCallList(obj);
}
void map_loader(int obj)
{
   glTranslated(-3,0,0);
   glScaled(1.73,1.3,1);
   if(map_hit == true){
      for(int i = 0; i < 10; i += .01){
   glRotatef(90+i,1,0,0);
   }
   }
   else
      glRotatef(90,1,0,0);
   glCallList(obj);
}
void title_loader(int obj)
{
   glTranslated(5,3,0);
   glScaled(.25,.25,.25);
   //glScaled(.8,.8,.8);
 
   glRotatef(title_rot,0,1,0);
   if(increase == true){
     title_rot += .03;
   }
   else{
      title_rot -= .03;
   }
   //Upper Bound and lower Bound for 
   // Title animation
   if(title_rot >= -5)
      increase = false;
   if(title_rot <= -10)
      increase = true;

   glCallList(obj);

}

void display()
{
   const double len=2.0;  //  Length of axes
   //  Light position and colors
   float Emission[]  = {0.0,0.0,0.0,1.0};
   float Ambient[]   = {1,1,1,1.0};
   float Diffuse[]   = {1.0,1.0,1.0,1.0};
   float Specular[]  = {spc,spc,spc,1.0};
   float Position[]  = {-3,0,15,1.0};
   float Position_1[]  = {-3,10,0,1.0};
   //float Position[]  = {90,0,0,1.0};
   float Shinyness[] = {14};

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (proj%2 == 0)
   {

      camera_switch = false;
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Pacman View
   else
   {
     // if(north == true){
      
      camera_switch = true;
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      //  gluLookAt(horizon-4,vertical, 2, horizon,vertical,0 , 90,Cos(ph),0); 
      gluLookAt(vertical-4,horizon, 2, vertical,horizon,0 , 90,-Cos(ph),0);     
     // }
     /* else if(west == true){
      
      camera_switch = true;
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      //  gluLookAt(horizon-4,vertical, 2, horizon,vertical,0 , 90,Cos(ph),0); 
      gluLookAt(horizon,vertical-4, 2, vertical,horizon,0 , 90,-Cos(ph),0);

      }
      else if(east == true){
      
      camera_switch = true;
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      //  gluLookAt(horizon-4,vertical, 2, horizon,vertical,0 , 90,Cos(ph),0); 
      gluLookAt(vertical-4,horizon, 2, vertical,horizon,0 , 90,-Cos(ph),0);

      }
      else{

      camera_switch = true;
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      //  gluLookAt(horizon-4,vertical, 2, horizon,vertical,0 , 90,Cos(ph),0); 
      gluLookAt(vertical-4,horizon, 2, vertical,horizon,0 , 90,-Cos(ph),0);
      
      }*/
   }

   //  Draw light position as sphere (still no lighting here)
   glColor3f(1,1,1);
   glPushMatrix();
   glTranslated(Position[0],Position[1],Position[2]);
   glutSolidSphere(0.03,10,10);
   glPopMatrix();

   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //glEnable(GL_LIGHT1);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);
   //glLightfv(GL_LIGHT1,GL_POSITION_1,Position);

   //  Set materials
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,RGBA);
   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,RGBA);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   //Break1
   //  Draw the models
   //Load Map
   glPushMatrix();
   map_loader(map);
   glPopMatrix();

   glPushMatrix();
   title_loader(title);
   glPopMatrix();

   //Load Cherry
   glPushMatrix();
   cherry_loader(cherry);
   glPopMatrix();


   //Load Pacman
   glPushMatrix();
   glTranslated(0,0,0);
   pacman_loader(pacman);
   glPopMatrix();
   //printf("X position =  %d\n",horizon);
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s",
     th,ph,dim,proj?"Perpective":"Pacman");
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Control Movement
   // West
   else if (ch == 'a' || ch == 'A')
      {
         if(is_not_legal(horizon - .25, vertical, -1,.35, .5, .35, -1,3.25,.5,3.25) == false &&
            is_not_legal(horizon - .25, vertical, -5, -1, -1.25, -1, -5,1.25,-1.25,1.25) == false){          
            horizon -= speed;
           
      }
      else{
            map_hit = true;
      }
        // if(is_not_legal(horizon-3 - .25, vertical, -1.75,1.5,0,1.5,-1.75,1.55,0,1.55) == false)
         //   horizon -= speed; 
         north = false;
         south = false;
         west = true;
      }
   //East
   else if (ch == 'd' || ch == 'D')
      {
        

         if(is_not_legal(horizon + .25, vertical, -1,.35, .5, .35, -1,3.25,.5,3.25) == false &&
            is_not_legal(horizon + .25, vertical, -2.25,1.25,0,1.25,-2.25,2,0,2) == false &&
            is_not_legal(horizon + .25, vertical, -5, -1, -1.25, -1, -5,1.25,-1.25,1.25) == false &&
            is_not_legal(horizon + .25, vertical, .5, .25, 3.75, .25, .5, 2.25, 3.75, 2.25) == false &&
            is_not_legal(horizon + .25, vertical, 3,2, 4,2,3,10,4,10) == false){        
            horizon += speed;
      }
       else{
            map_hit = true;
         
      }
         
         north = false;
         south = false;
         west = false;
      }  
   //North 
   else if (ch == 'w' || ch == 'W')
      {
         if(is_not_legal(horizon, vertical + .25, -1, .35, .5, .35, -1,3.25,.5,3.25) == false &&
            is_not_legal(horizon, vertical + .25, -2.25,1.25,0,1.25,-2.25,2,0,2) == false &&
            is_not_legal(horizon, vertical + .25, -5, -1, -1.25, -1, -5,1.25,-1.25,1.25) == false &&
            is_not_legal(horizon, vertical + .25, .5, .25, 3.75, .25, .5, 2.25, 3.75, 2.25) == false &&
            is_not_legal(horizon, vertical + .25, -20, 4.75, 20,4.75, -10, 10, 20, 10) == false){ 
            vertical += speed;
      }
            else{
            map_hit = true;
         
      }
         north = true;
         south = false;
         west = false;
    
      }
   //Down
   else if (ch == 's' || ch == 'S')
      {
        if(is_not_legal(horizon, vertical - .25, -1, .35, .5, .35, -1,3.25,.5,3.25) == false &&
         is_not_legal(horizon, vertical - .25, -2.25,1,0,1,-2.25,2.15,0,2.15) == false &&
         is_not_legal(horizon, vertical -.25, -5, -1, -1.25, -1, -5,1.25,-1.25,1.25) == false &&
         is_not_legal(horizon, vertical - .25, .5, .25, 3.75, .25, .5, 2.25, 3.75, 2.25) == false){
            vertical -= speed;
      }
       else{
            map_hit = true;
      }
         north = false;
         south = true;
         west = false;
      }
   //  Toggle projection type
   else if (ch == 'p' || ch == 'P')
         proj = 1 + proj;
   //  Toggle specular light
   else if (ch == 'l' || ch == 'L')
      spc = 1-spc;
   //  Toggle light movement
   else if (ch == 'u' || ch == 'U')
      move = 1-move;
   //  Move light
   else if (ch == '[')
      zh -= 1;
   else if (ch == ']')
      zh += 1;
   //  Light elevation
   else if (ch == '+')
      Ylight += 0.1;
   else if (ch == '-')
      Ylight -= 0.1;
   else if (ch == 'z')
      horizon += 1;
   //  Reproject
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(proj?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //if (argc!=2 && argc!=3 && argc!=6) Fatal("Usage: %s <obj> [scale [R G B]]\n",argv[0]);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(1000,600);
   glutCreateWindow("3D PACMAN");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Set scale
   if (argc>=3) scale = strtod(argv[2],NULL);
   if (scale<=0) scale = 1;
   //  Set color
   if (argc>=6) RGBA[0] = strtod(argv[3],NULL);
   if (argc>=6) RGBA[1] = strtod(argv[4],NULL);
   if (argc>=6) RGBA[2] = strtod(argv[5],NULL);
   //  Load object
   cherry = LoadOBJ(argv[1]);
   pacman = LoadOBJ(argv[2]);
   map = LoadOBJ(argv[3]);
   title = LoadOBJ(argv[4]);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
