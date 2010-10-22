/**
OBJ Viewer
**/

//STD libraries
#include <iostream>
#include <string>

//OpenGL libraries
#include <GL/glew.h>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/glut.h>
#endif

//Engine libraries
#include "OBJModel.h"
#include "MyBitmap.h"
#include "Node.h"
#include "TransformNode.h"
#include "PolyMeshNode.h"
#include "Texture.h"
#include "Vector3.h"
#include "CameraNode.h"

#include "PolarCamera.h"

using namespace std;

//Default Screen Size
const int WIDTH = 640;
const int HEIGHT = 480;

/**
Global Variables
**/

//Temporary screen width and height
int width = WIDTH;
int height = HEIGHT;

//Mouse position
int mouse_x;
int mouse_y;
int mouse_down_x;	//Mouse position when a user clicks
int mouse_down_y;

//Mouse clicks states
bool left_mouse_down = false;
bool middle_mouse_down = false;
bool right_mouse_down = false;

//Model type
int model = 0;

//Show Wireframe
bool show_wireframe = false;
bool shaded_mode = true;

//Camera - Polar view
PolarCamera* camera = new PolarCamera();

//Scene Graph Camera
CameraNode* sgCamera;

//Temporary variables to facilitate Polar camera
float azimuth_change = 0;
float twist_change = 0;
float elevation_change = 0;
float cam_distance_change = 0.0;

//Panning temporary variables
float pan_x_down = 0;	//When the pan button is pushed, record this
float pan_y_down = 0;
float pan_z_down = 0;
float pan_x_change = 0;
float pan_y_change = 0;

//Orthographic zooming temporary variable
float ortho_zoom_change = 0;



//Texture
MyBitmap* texture;
static GLuint texName;

//Scene Graph
Node* rootNode;

//Calculating fps
int frame = 0, time, timebase=0;
float fps=0.0f;
int font=(int)GLUT_BITMAP_8_BY_13;
char s[30];


//For testing
PolyMeshNode* humveeNode;

/**
Draw a 24x24 grid
**/
static void grid(int scale = 1)
{
	//Turn off lighting for grid
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);

	//Start drawing lines
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
		for (int x=-12; x<=12; x++){
			if (x != 0){
				glVertex3f((GLfloat)(x*scale), (GLfloat)0.0, (GLfloat)(-12*scale));
				glVertex3f((GLfloat)(x*scale), (GLfloat)0.0, (GLfloat)(12*scale));
			}
		}
		for (int z=-12; z<=12; z++){
			if (z != 0){
				glVertex3f((GLfloat)(-12*scale), (GLfloat)0.0, (GLfloat)(z*scale));
				glVertex3f((GLfloat)(12*scale), (GLfloat)0.0, (GLfloat)(z*scale));
			}
		}
	glEnd();

	//Make sure the lines at the origin are nice and different
	/*
	glBegin(GL_LINES);
		glColor3f(0.75, 0.75, 0.75);
		glVertex3f(0, 0, (GLfloat)(-12*scale));
		glVertex3f(0, 0, (GLfloat)(12*scale));
		glVertex3f((GLfloat)(-12*scale), 0, 0);
		glVertex3f((GLfloat)(12*scale), 0, 0);
	glEnd();
	*/

	//Draw axis
	glBegin(GL_LINES);
		//x axis should be red and pointing to positive x
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(12*scale, 0.0, 0.0);
	glEnd();

	//Draw cone
	glPushMatrix();
		glTranslatef(12*scale, 0.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glutSolidCone(0.2, 0.75, 8, 3);
	glPopMatrix();

	glBegin(GL_LINES);
		//y axis
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 12*scale, 0.0);
	glEnd();

	//Draw cone
	glPushMatrix();
		glTranslatef(0.0, 12*scale, 0.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		glutSolidCone(0.2, 0.75, 8, 3);
	glPopMatrix();

	glBegin(GL_LINES);
		//z axis
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 12*scale);
	glEnd();	

	//Draw cone
	glPushMatrix();
		glTranslatef(0.0, 0.0, 12*scale);
		glutSolidCone(0.2, 0.75, 8, 3);
	glPopMatrix();

	glFlush();

	//Turn lighting back on
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

}

static void draw_models()
{
	//Draw using scene graph
	rootNode->render();
	
}

void drawText(char* string)
{
	char* p;
	for (p=string; *p; p++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	}
}

/**
Calculate and display FPS
**/
static void profiler()
{
	glDisable(GL_TEXTURE_2D);

	//Calculate FPS
	frame++;	//Increment number of frames
	time = glutGet(GLUT_ELAPSED_TIME);	//Get current time

	//Time is current time, timebase is last time since we took a benchmark
	if ((time - timebase) > 500) {	//Do this every x milliseconds
		
		float fps = 1000* (float)frame / (float)(time-timebase);
		sprintf(s,"FPS: %4.2f", fps );
	 	
		timebase = time;	//Set timebase to current time
		frame = 0;	//Reset number of frames
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glWindowPos2i(5, 5);
	
	drawText(s);
	glEnable(GL_TEXTURE_2D);

}


/**
Display function
**/
static void display()
{
	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Call scene graph camera
	sgCamera->viewTransform();

	/**
	Camera setup
	**/
	
	//Translate & Rotate Polar Views
	/*
	glTranslatef(-(GLfloat)(camera->point_of_interest->x + pan_x_change), (GLfloat)(camera->point_of_interest->y + pan_y_change), -((GLfloat)camera->distance + (GLfloat)cam_distance_change));		//Translate along the z axis away from camera
	glRotatef((GLfloat)camera->twist + (GLfloat)twist_change, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);							//Rotate around z axis (usually by 0)
	glRotatef((GLfloat)camera->elevation + (GLfloat)elevation_change, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);					//Rotate around x axis
	glRotatef((GLfloat)camera->azimuth + (GLfloat)azimuth_change, (GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);						//Rotate around y axis
	*/

	//Draw grid
	grid();

	//Wireframe switching
	//If shaded mode, draw the model with shading, else only draw the wireframe
	if (shaded_mode){

		//Setup polygon offset for drawing wire on shaded
		glPolygonOffset(1.0, 1.0);
		glEnable(GL_POLYGON_OFFSET_FILL);
	
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
		glColor3f(1.0, 1.0, 1.0);

		//Draw the obj model
		draw_models();

		glDisable(GL_POLYGON_OFFSET_FILL);

		//If show wireframe, render the wireframe on top of shaded object
		if (show_wireframe){
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glColor3f(0.0, 0.0, 0.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
			//Draw the obj model
			draw_models();

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}
	} else {
		//Wireframe only
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glColor3f(1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		//Draw the obj model
		draw_models();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
	}

	profiler();

	glutSwapBuffers();

}

/**
Sets the projection
**/
static void set_projection()
{
	sgCamera->setProjection(width, height);
	/*
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Perspective mode or ortho
	float aspect_ratio = (float)width / (float)height;
	if (camera->perspective_mode){
		gluPerspective(camera->fov, GLfloat(aspect_ratio), camera->near_plane, camera->far_plane);
	} else {
		//Keep aspect ratio
		float left, right, bottom, top, temp_ortho_zoom;
		temp_ortho_zoom = camera->ortho_zoom + ortho_zoom_change;

		if (aspect_ratio < 1.0){
			left = -temp_ortho_zoom;
			right = temp_ortho_zoom;
			bottom = -temp_ortho_zoom * ( 1.0f / aspect_ratio );
			top = temp_ortho_zoom * ( 1.0f / aspect_ratio );
		} else {
			left = -temp_ortho_zoom * aspect_ratio;
			right = temp_ortho_zoom * aspect_ratio;
			bottom = -temp_ortho_zoom;
			top = temp_ortho_zoom;
		}
		glOrtho(left, right, bottom, top, -camera->far_plane, camera->far_plane);
	}
	*/
}

/**
Reshape function
**/
static void reshape(int new_width, int new_height)
{
	//Set Global Variables
	width = new_width;
	height = new_height;

	set_projection();
	//sgCamera->setProjection(width, height);
}

/** 
Keyboard Input
**/
static void keyboard_func(unsigned char key, int mx, int my)
{
	switch (key)
	{
	//Camera Motion for Assignment
	case 'u':
		camera->point_of_interest->y -= 0.02f;
		glutPostRedisplay();
		break;
	case 'd':
		camera->point_of_interest->y += 0.02f;
		glutPostRedisplay();
		break;
	case 'l':
		camera->point_of_interest->x -= 0.02f;
		glutPostRedisplay();
		break;
	case 'r':
		camera->point_of_interest->x += 0.02f;
		glutPostRedisplay();
		break;
	case 'f':
		camera->distance -= 0.2f;
		glutPostRedisplay();
		break;
	case 'b':
		camera->distance += 0.2f;
		glutPostRedisplay();
		break;
	
	//Views
	case 'c':	//Reset views
		//Destroy and reinitialize camera
		/*
		delete camera;
		camera = new PolarCamera();
		set_projection();
		*/
		break;

	case 'w':	//Show Wireframe
		show_wireframe = show_wireframe == false ? true : false;
		break;
	case 's':	//Shaded Model
		shaded_mode = shaded_mode == false ? true : false;
		break;
	case 'p':	//Perspective
		camera->perspective_mode = true;
		set_projection();
		break;
	case 'o':	//Ortho
		camera->perspective_mode = false;
		set_projection();
		break;
	case 'z':	//Zoom forward
		if (camera->perspective_mode){
			camera->fov -= 1.5f;
		} else {
			camera->ortho_zoom -= 0.1f;
		}
		set_projection();
		break;
	case 'Z':	//Zoom back
		if (camera->perspective_mode){
			camera->fov += 1.5f;
		} else {
			camera->ortho_zoom += 0.1f;
		}
		set_projection();
		break;

	//Alternative zoom
	case '+':	//Zoom forward
		if (camera->perspective_mode){
			camera->fov -= 1.5f;
		} else {
			camera->ortho_zoom -= 0.1f;
		}
		set_projection();
		break;
	case '-':	//Zoom back
		if (camera->perspective_mode){
			camera->fov += 1.5f;
		} else {
			camera->ortho_zoom += 0.1f;
		}
		set_projection();
		break;

	//Models
	case '1':
		model = 0;
		break;
	case '2':
		model = 1;
		break;
	case '3':
		model = 2;
		break;
	case '4':
		model = 3;
		break;
	case '5':
		model = 4;
		break;
	case '6':
		model = 5;
		break;
	case '7':
		model = 6;
		break;
	case '8':
		model = 7;
		break;

	//Exit
	case 27:
		exit(0);
	}

	glutPostRedisplay();
}

/**
Function Keys
**/
static void functionKeys(int key, int mx, int my)
{

}

/**
Mouse Clicks
**/
static void mouse_clicks(int button, int state, int mx, int my)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			left_mouse_down = true;		//Set mouse down state to true
			mouse_down_x = mx;			//Record screen position of mouse when user clicked left button
			mouse_down_y = my;
			//cout << "Left mouse button clicked at:" << mx << ", " << my << endl;
		} else {
			left_mouse_down = false;

			//Add changes to camera
			sgCamera->azimuth += sgCamera->azimuth_offset;
			sgCamera->elevation += sgCamera->elevation_offset;
			sgCamera->azimuth_offset = 0.0f;
			sgCamera->elevation_offset = 0.0f;
			
		}
		break;
	

	case GLUT_RIGHT_BUTTON:
		//Move camera forward and back
		if (state == GLUT_DOWN) {
			right_mouse_down = true;
			mouse_down_x = mx;
			mouse_down_y = my;

		} else {
			right_mouse_down = false;
			
			//Reset camera distance offset
			sgCamera->distance += sgCamera->distance_offset;
			sgCamera->distance_offset = 0.0f;

			//Reset distance deltas
			camera->distance += cam_distance_change;
			cam_distance_change = 0;
			
			//Ortho
			camera->ortho_zoom += ortho_zoom_change;
			ortho_zoom_change = 0;
			
		}
		break;

	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			middle_mouse_down = true;

			mouse_down_x = mx;
			mouse_down_y = my;

		} else {
			middle_mouse_down = false;

			sgCamera->panX += sgCamera->panX_offset; sgCamera->panX_offset = 0.0f;
			sgCamera->panY += sgCamera->panY_offset; sgCamera->panY_offset = 0.0f;
			

			//Reset panning deltas
			camera->point_of_interest->x += pan_x_change;
			camera->point_of_interest->y += pan_y_change;
			pan_x_change = 0;
			pan_y_change = 0;
			
		}
		break;

	}


}

/**
Mouse Movement
**/
static void mouse_move(int mx, int my)
{
	//Only bother processing if mouse button is pressed

	//Left Mouse Button - Orbit
	if (left_mouse_down){

		sgCamera->azimuth_offset = -(float(mouse_down_x) - float(mx)) / float(width) * 90;
		sgCamera->elevation_offset = -(float(mouse_down_y) - float(my)) / float(height) * 90;

	}

	//Right Mouse Button - Dolly
	if (right_mouse_down){
		
		if (sgCamera->perspectiveMode)
		{
			sgCamera->distance_offset = (float(mouse_down_x) - float(mx)) / float(width) * 10;
		} else {
			sgCamera->distance_offset = (float(mouse_down_x) - float(mx)) / float(width);
			sgCamera->setProjection(width, height);
		}

		if (camera->perspective_mode){
			cam_distance_change = (float(mouse_down_x) - float(mx)) / float(width) * 10;
		} else {
			ortho_zoom_change = (float(mouse_down_x) - float(mx)) / float(width);
			set_projection();
		}
	}

	//Middle Mouse Button - Panning
	if (middle_mouse_down){

		sgCamera->panX_offset = (float(mouse_down_x) - float(mx)) / float(width) * 5;
		sgCamera->panY_offset = -(float(mouse_down_y) - float(my)) / float(height) * 5;
		
	}

	//Force redisplay
	if (left_mouse_down || middle_mouse_down || right_mouse_down){
		glutPostRedisplay();
	}

}


/**
Enable lighting & materials
**/
static void lighting()
{
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	//Setup Material
	GLfloat mat_ambient[] = {(GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0};
	GLfloat mat_diffuse[] = {(GLfloat)0.9, (GLfloat)0.9, (GLfloat)0.9, (GLfloat)1.0};
	GLfloat mat_specular[] = {(GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0};
	GLfloat mat_shininess[] = {(GLfloat)50};

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//Setup Light(s)
	GLfloat light_position[] = {30.0, 30.0, 30.0, 0.0};
	GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

	//Setup Ambient Light
	//GLfloat lmodel_ambient[] = {(GLfloat)0.2, (GLfloat)0.2, (GLfloat)0.2, (GLfloat)1.0};

	//Turn on lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}


/**
Clear out the screen
**/
static void init()
{
	//Backface culling
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//Load models
	/*
	uhtiger = new OBJModel("models/uhtiger.obj");
	PolyMeshNode* uhTigerNode = new PolyMeshNode();
	uhTigerNode->attachModel(uhtiger);
	Texture* uhTigerTexture = new Texture("textures/uhtiger.bmp");
	uhTigerNode->attachTexture(uhTigerTexture);
	uhTigerNode->translate->z = 5.0f;

	m1abrams = new OBJModel("models/m1abrams.obj");
	PolyMeshNode* m1AbramsNode = new PolyMeshNode();
	m1AbramsNode->attachModel(m1abrams);
	Texture* m1AbramsTexture = new Texture("textures/M1_ABRAM.bmp");
	m1AbramsNode->attachTexture(m1AbramsTexture);
	m1AbramsNode->translate->z = -5.0f;
	m1AbramsNode->translate->x = -15.0f;
	*/

	OBJModel* humveehardtop = new OBJModel("models/humveehardtop.obj");
	humveeNode = new PolyMeshNode();
	humveeNode->attachModel(humveehardtop);
	Texture* humveeTexture = new Texture("textures/humveehardtop.bmp");
	humveeNode->attachTexture(humveeTexture);
	humveeNode->rotate->y = -90;

	PolyMeshNode* humveeNode2 = new PolyMeshNode();
	humveeNode2->attachModel(humveehardtop);
	humveeNode2->attachTexture(humveeTexture);
	humveeNode2->translate->x = -20;
	humveeNode2->rotate->y = 90;

	PolyMeshNode* humveeNode3 = new PolyMeshNode();
	humveeNode3->attachModel(humveehardtop);
	humveeNode3->attachTexture(humveeTexture);
	humveeNode3->translate->x = -20;

	//Scene Graph
	rootNode = new Node();
	rootNode->addChild(humveeNode);
	humveeNode->addChild(humveeNode2);
	humveeNode2->addChild(humveeNode3);

	//10 tanks
	/*
	for (int x=-10; x<=10; x++)
	{
		for (int z=-10; z<=10; z++)
		{
			TransformNode* transform = new TransformNode(TRANSLATE);
			transform->translate->x = x*20;
			transform->translate->z = z*20;
			transform->addChild(m1AbramsNode);
			rootNode->addChild(transform);
		}
	}
	*/

	//Create a camera and attach to root node
	sgCamera = new CameraNode(FIRSTPERSON);
	sgCamera->translate->x = -10.0f;
	sgCamera->translate->z = 10.0f;
	sgCamera->translate->y = 2.0f;
	sgCamera->rotate->y = 45;
	//rootNode->addChild(sgCamera);

	sgCamera = new CameraNode(POLAR);
	sgCamera->distance = 30.0f;
	sgCamera->azimuth = -180.0f;
	sgCamera->elevation = 40.0f;
	humveeNode3->addChild(sgCamera);

	//Set up lighting
	lighting();

	//Anti-Aliasing
	/*
	glEnable (GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glLineWidth (1);
	*/



}

/**
Main function
**/
int main(int argc, char** argv)
{
	
	//Get current directory
	cout << "Firing up OBJViewer" << endl;
	cout << "===================" << endl << endl;
	
	cout << "Camera Motion " << endl;
	cout << "Orbit: Left click and drag mouse" << endl;
	cout << "Panning: Middle click and drag mouse" << endl;
	cout << "Dolly: Right click and drag mouse" << endl << endl;

	cout << "Viewport Controls " << endl;
	cout << "w: Toggle wireframe" << endl;
	cout << "s: Toggle shaded mode" << endl;
	cout << "p: Perspective mode" << endl;
	cout << "o: Orthographic mode" << endl;
	cout << "+ or z: Zoom in" << endl;
	cout << "- or Z(shift-z): Zoom out" << endl;
	cout << "c: Reset view" << endl << endl;

	cout << "Escape: Quit" << endl;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OBJViewer Test - Leonard Teo");

	//Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard_func);
	glutSpecialFunc(functionKeys);
	glutMouseFunc(mouse_clicks);
	glutMotionFunc(mouse_move);
	glutIdleFunc(display);	//Set idle function to force refresh

	glutMainLoop();

	//Cleanup

	delete camera;

	exit(0);

}