/**
OBJ Viewer
**/

#include <iostream>
#include <GL/glut.h>
#include "PolarCamera.h"
#include "OBJModel.h"
#include "MyBitmap.h"

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

//OBJ Models
//OBJModel* objmodel;	//Any objmodel for testing

OBJModel* m1abrams;
OBJModel* humveehardtop;
OBJModel* uhtiger;

//Texture
MyBitmap* texture;
static GLuint texName;

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
	glBegin(GL_LINES);
		glColor3f(0.75, 0.75, 0.75);
		glVertex3f(0, 0, (GLfloat)(-12*scale));
		glVertex3f(0, 0, (GLfloat)(12*scale));
		glVertex3f((GLfloat)(-12*scale), 0, 0);
		glVertex3f((GLfloat)(12*scale), 0, 0);
	glEnd();

	glFlush();

	//Turn lighting back on
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

}

static void draw_models()
{
	//Draw all three models (let's be awesome)
	glPushMatrix();
		glTranslatef(-15, 0, 0);
		m1abrams->draw();
	glPopMatrix();

	uhtiger->draw();

	glPushMatrix();
		glTranslatef(15,0,0);
		humveehardtop->draw();
	glPopMatrix();

	//Switch model
	/*
	switch (model)
	{
	case 0:
		m1abrams->draw();
		break;
	case 1:
		humveehardtop->draw();
		break;
	case 2:
		uhtiger->draw();
		break;
	}
	*/
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

	/**
	Camera setup
	**/
	//Translate & Rotate Polar Views
	glTranslatef(-(GLfloat)(camera->point_of_interest->x + pan_x_change), (GLfloat)(camera->point_of_interest->y + pan_y_change), -((GLfloat)camera->distance + (GLfloat)cam_distance_change));		//Translate along the z axis away from camera
	glRotatef((GLfloat)camera->twist + (GLfloat)twist_change, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);							//Rotate around z axis (usually by 0)
	glRotatef((GLfloat)camera->elevation + (GLfloat)elevation_change, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);					//Rotate around x axis
	glRotatef((GLfloat)camera->azimuth + (GLfloat)azimuth_change, (GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);						//Rotate around y axis

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

	glutSwapBuffers();

}

/**
Sets the projection
**/
static void set_projection()
{
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
		delete camera;
		camera = new PolarCamera();
		set_projection();
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
			camera->azimuth += azimuth_change;
			camera->elevation += elevation_change;
			
			//Reset deltas
			azimuth_change = 0;
			elevation_change = 0;
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
		azimuth_change = - (float(mouse_down_x) - float(mx)) / float(width) * 90;
		elevation_change = - (float(mouse_down_y) - float(my)) / float(height) * 90;
	}

	//Right Mouse Button - Dolly
	if (right_mouse_down){
		
		if (camera->perspective_mode){
			cam_distance_change = (float(mouse_down_x) - float(mx)) / float(width) * 10;
		} else {
			ortho_zoom_change = (float(mouse_down_x) - float(mx)) / float(width);
			set_projection();
		}
	}

	//Middle Mouse Button - Panning
	if (middle_mouse_down){

		pan_x_change = (float(mouse_down_x) - float(mx)) / float(width) * 5;
		pan_y_change = (float(mouse_down_y) - float(my)) / float(height) * 5;
		
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
	GLfloat lmodel_ambient[] = {(GLfloat)0.2, (GLfloat)0.2, (GLfloat)0.2, (GLfloat)1.0};

	//Turn on lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

//Load Texture
static void load_texture(char* filename)
{
	texture = new MyBitmap(filename);
	if (!texture->loaded){
		cout << "Could not load texture." << endl;
		exit(0);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);				//Allocate Texture Name
	glBindTexture(GL_TEXTURE_2D, texName);	//Select current texture

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
	glEnable( GL_TEXTURE_2D );
	

	delete texture;
}

/**
Clear out the screen
**/
static void init()
{
	//Load model
	m1abrams = new OBJModel("C:\\mayaprojects\\GameEngineTests\\scenes\\m1abrams.obj");
	m1abrams->loadTexture("C:\\mayaprojects\\GameEngineTests\\sourceimages\\M1_ABRAM.bmp");

	humveehardtop = new OBJModel("C:\\mayaprojects\\GameEngineTests\\scenes\\humveehardtop.obj");
	humveehardtop->loadTexture("C:\\mayaprojects\\GameEngineTests\\sourceimages\\humveehardtop.bmp");

	uhtiger = new OBJModel("C:\\mayaprojects\\GameEngineTests\\scenes\\uhtiger.obj");
	uhtiger->loadTexture("C:\\mayaprojects\\GameEngineTests\\sourceimages\\uhtiger.bmp");

	//objmodel = new OBJModel("C:\\mayaprojects\\GameEngineTests\\scenes\\quakeplasma.obj");
	//objmodel = new OBJModel("C:\\mayaprojects\\GameEngineTests\\scenes\\uhtiger.obj");
	//objmodel = new OBJModel("C:\\mayaprojects\\GameEngineTests\\scenes\\humveehardtop.obj");
	//objmodel = new OBJModel("C:\\mayaprojects\\GameEngineTests\\scenes\\ussoldier1.obj");

	//load_texture("C:\\mayaprojects\\GameEngineTests\\sourceimages\\M1_ABRAM.bmp");
	//load_texture("C:\\mayaprojects\\GameEngineTests\\sourceimages\\plasma.bmp");
	//load_texture("C:\\mayaprojects\\GameEngineTests\\sourceimages\\uhtiger.bmp");
	//load_texture("C:\\mayaprojects\\GameEngineTests\\sourceimages\\humveehardtop.bmp");
	//load_texture("C:\\mayaprojects\\GameEngineTests\\sourceimages\\ussoldier01.bmp");

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//Set up lighting
	lighting();

	//Anti-Aliasing
	/*
	glEnable (GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glLineWidth (1);
	*/

	//Backface culling
	glEnable(GL_CULL_FACE);

}

/**
Main function
**/
int main(int argc, char** argv)
{

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

	cout << "Deluxe Light Cycle Controls " << endl;
	cout << "left/right arrow keys: Turn front wheels" << endl;
	cout << "[ and ]: Open/close canopy" << endl << endl;

	cout << "Escape: Quit" << endl;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OBJViewer Test - Leonard Teo");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard_func);
	glutSpecialFunc(functionKeys);
	glutMouseFunc(mouse_clicks);
	glutMotionFunc(mouse_move);

	glutMainLoop();

	//Cleanup
	/*
	delete lightcycle_chassis;
	delete lightcycle_canopy;
	delete lightcycle_axel_front_right;
	delete lightcycle_axel_front_left;
	delete lightcycle_wheel_front_right;
	delete lightcycle_wheel_front_left;
	delete lightcycle_wheel_rear_right;
	delete lightcycle_wheel_rear_left;
	*/
	delete camera;

	exit(0);

}