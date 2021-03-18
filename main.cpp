#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
int red_dot_count;
double angle,canon_rotation_X,canon_rotation_Y_1,canon_rotation_Y_2,canon_rotation_Z;

///////////// data struct //////////

struct point
{
	double x,y,z;
	point(double xx,double yy,double zz) {
        x = xx; y = yy; z = zz;
	}
	point() {
	    x = 0; y = 0; z = 0;
	}
}cam_pos,red_dots[100];


struct vect
{
    double x,y,z;

    vect(double xx,double yy,double zz) {
        x = xx; y = yy; z = zz;
    }

    vect() {
        x = 0; y = 0; z = 0;
    }

}up,right,look;


///////////// functions //////////////

point add_vect_point(struct point p,struct vect v)
{
    p.x = p.x + v.x;
    p.y = p.y + v.y;
    p.z = p.z + v.z;

    return p;
}

vect add_vect_vect(struct vect one,struct vect two) {

    return vect( one.x+two.x, one.y+two.y, one.z+two.z );
}

vect vect_from_two_point(struct point a,struct point b) {
    vect r;
    r.x = b.x-a.x;
    r.y = b.y-a.y;
    r.z = b.z-a.z;
	return r;
}

vect neg_vect(struct vect v) {

    return vect(-v.x,-v.y,-v.z);
}

double dist_two_point(struct point a,struct point b) {
    return sqrt( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z) );
}

double dot_product (struct vect a,struct vect b) {
    return (a.x*b.x+a.y*b.y + a.z*b.z);
}

double dist_from_origin(struct point a) {
    return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}

double value_of_vect(struct vect a) {
    return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}

struct vect cross_product(struct vect p1,struct vect p2)
{
    struct vect ret;

    ret.x = p1.y * p2.z - p1.z * p2.y;
    ret.y = p1.z * p2.x - p1.x * p2.z;
    ret.z = p1.x * p2.y - p1.y * p2.x;

    return ret;

}





void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    // a comment added
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}




void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
               // glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void draw_Cylinder(double radius,point center,double length) {
    struct point points[100];
	int slices = 35
	;



    for(int i=0;i<=slices;i++) {

        points[i].y=radius*cos(((double)i/(double)slices)*2*pi)+center.y;
        points[i].z=radius*sin(((double)i/(double)slices)*2*pi)+center.z;
        points[i].x=center.x;

    }


	for(int i=0;i<slices;i++) {


        if(i%2==0)
            glColor3f(1,1,1);
        else
            glColor3f(0,0,0);

        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x,points[i].y,points[i].z);
            glVertex3f(points[i+1].x,points[i+1].y,points[i+1].z);

            glVertex3f(points[i].x+length,points[i].y,points[i].z);
            glVertex3f(points[i+1].x+length,points[i+1].y,points[i+1].z);
        }
        glEnd();


	}


}

void draw_half_sphere(double radius,point center,bool isLeft) {
    struct point points[100][100];
	int i,j;
	double h,r;
	int stacks = 35;
	int slices = 35;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].y=r*cos(((double)j/(double)slices)*2*pi)+center.y;
			points[i][j].z=r*sin(((double)j/(double)slices)*2*pi)+center.z;
			points[i][j].x=h+center.x;
		}
	}

	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{
            if(j%2==0)
                glColor3f(1,1,1);
            else
                glColor3f(0,0,0);

			glBegin(GL_QUADS);{
			    //left hemisphere
			    if(!isLeft) {
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			    }
                //right hemisphere
                else {
                    glVertex3f(-points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(-points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(-points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(-points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                }
			}glEnd();
		}
	}

}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void print_console() {
    printf("Camera position: (%.2lf,%.2lf,%.2lf)\n",cam_pos.x,cam_pos.y,cam_pos.z);
    printf("Looking at: (%.2lf,%.2lf,%.2lf)",cam_pos.x+look.x,cam_pos.y+look.y,cam_pos.z+look.z);
}

// L = one    R = two   U = axis



void Rotate(vect *one,vect *two, vect *axis, double angle)
{

     double ldot=dot_product(*axis,*one);
     struct vect lcross=cross_product(*axis,*one);

     double rdot=dot_product(*axis,*two);
     struct vect rcross=cross_product(*axis,*two);

    one->x=one->x*cos(angle)+lcross.x*sin(angle)+(axis->x)*ldot*(1-cos(angle));
    one->y=one->y*cos(angle)+lcross.y*sin(angle)+(axis->y)*ldot*(1-cos(angle));
    one->z=one->z*cos(angle)+lcross.z*sin(angle)+(axis->z)*ldot*(1-cos(angle));

    two->x=two->x*cos(angle)+rcross.x*sin(angle)+(axis->x)*rdot*(1-cos(angle));
    two->y=two->y*cos(angle)+rcross.y*sin(angle)+(axis->y)*rdot*(1-cos(angle));
    two->z=two->z*cos(angle)+rcross.z*sin(angle)+(axis->z)*rdot*(1-cos(angle));




}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
            Rotate(&look,&right,&up,angle);
			break;

        case '2':
            Rotate(&look,&right,&up,-angle);
            break;
        case '3':
            Rotate(&look,&up,&right,angle);
            break;
        case '4':

            Rotate(&look,&up,&right,-angle);
            break;
        case '5':
            Rotate(&up,&right,&look,angle);
            break;
        case '6':
            Rotate(&up,&right,&look,-angle);
            break;
        case 'p':
            print_console();
            break;

        case 'q':
            canon_rotation_Z++;
            break;
        case 'w':
            canon_rotation_Z--;
            break;

        case 'e':
            canon_rotation_Y_1++;
            break;
        case 'r':
            canon_rotation_Y_1--;
            break;
        case 'a':
            canon_rotation_Y_2++;
            break;
        case 's':
            canon_rotation_Y_2--;
            break;
        case 'd':
            canon_rotation_X++;
            break;
        case 'f':
            canon_rotation_X--;
            break;



		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cam_pos = add_vect_point(cam_pos,neg_vect(look));
			break;
		case GLUT_KEY_UP:		// up arrow key
		    cam_pos = add_vect_point(cam_pos,look);
			break;

		case GLUT_KEY_RIGHT:
			cam_pos = add_vect_point(cam_pos,right);
			break;
		case GLUT_KEY_LEFT:
			cam_pos = add_vect_point(cam_pos,neg_vect(right));
			break;

		case GLUT_KEY_PAGE_UP:
		    cam_pos = add_vect_point(cam_pos,up);
			break;
		case GLUT_KEY_PAGE_DOWN:
		    cam_pos = add_vect_point(cam_pos,neg_vect(up));
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void draw_canon() {


    glRotatef(canon_rotation_Z,0,0,1);

    draw_half_sphere(50,point(0,0,0),true);

    glRotatef(canon_rotation_Y_1,0,1,0);


    draw_half_sphere(50,point(0,0,0),false);

    glTranslatef(50,0,0);
    glRotatef(canon_rotation_Y_2,0,1,0);

    glRotatef(canon_rotation_X,1,0,0);

    draw_half_sphere(25,point(-25,0,0),true);
    draw_Cylinder(25,point(25,0,0),250);


}

void draw_plane(double side, double dist) {

    glColor3f(.5,.5,.5);
    glBegin(GL_QUADS);{

        glVertex3f(dist,side,side);
        glVertex3f(dist,-side,side);
        glVertex3f(dist,-side,-side);
        glVertex3f(dist,side,-side);

    }glEnd();


}

void draw_red_dot(point c)
{
    glColor3f(1,0,0);

    glBegin(GL_QUADS);{

        glVertex3f(c.x,c.y+5,c.z+5);
        glVertex3f(c.x,c.y-5,c.z+5);
        glVertex3f(c.x,c.y-5,c.z-5);
        glVertex3f(c.x,c.y+5,c.z-5);

    }glEnd();

}


void draw_red_dots() {

    for(int i=0;i<red_dot_count;i++) {
        draw_red_dot(red_dots[i]);
    }

}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which angle is the camera's UP angle?

	gluLookAt(cam_pos.x,cam_pos.y,cam_pos.z,
                cam_pos.x+look.x,cam_pos.y+look.y,cam_pos.z+look.z,
                up.x,up.y,up.z
             );


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();

	draw_plane(250,505);
    draw_red_dots();

	draw_canon();






	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=(pi)/360.0;

	canon_rotation_X = 0.0;
	canon_rotation_Y_1 = 0.0;
	canon_rotation_Y_2 = 0.0;
	canon_rotation_Z = 0.0;

	red_dot_count = 1;
	red_dots[0] = point(500,1,1);


	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X angle (horizontally)
	//near distance
	//far distance

	// gluLookAt(cam_pos.x,cam_pos.y,cam_pos.z,	0,0,0,	0,1,0);

	cam_pos = point(138,350,50);
	look = vect(-1/sqrt(2),-1/sqrt(2),0);
	up = vect(0,0,1);
	right = vect(-1/sqrt(2),1/sqrt(2),0);





}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

