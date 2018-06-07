/*
Niniejszy program jest wolnym oprogramowaniem; mo¿esz go
rozprowadzaæ dalej i / lub modyfikowaæ na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacjê Wolnego
Oprogramowania - wed³ug wersji 2 tej Licencji lub(wed³ug twojego
wyboru) którejœ z póŸniejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej¹, i¿ bêdzie on
u¿yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyœlnej
gwarancji PRZYDATNOŒCI HANDLOWEJ albo PRZYDATNOŒCI DO OKREŒLONYCH
ZASTOSOWAÑ.W celu uzyskania bli¿szych informacji siêgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnoœci¹ wraz z niniejszym programem otrzyma³eœ te¿ egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeœli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.hpp"
#include "class_template.hpp"
#include <iostream>

using namespace glm;


float aspect=1;
float speed=3.14; //prêdkoœæ k¹towa obrotu w radianach na sekundê
float speed_x = 0, speed_y = 0;

//deklarujemy obiekty Template
Template *mushroom;
Template *rod;
Template *piston;

void initObjects(){
    //inicjalzujemy obiekty Template (wgrywamy tekstury, kolory itp)
    mushroom = new Template("objects\\mushroom.obj");
    rod = new Template("objects\\rod.obj");
    piston = new Template("objects\\piston.obj");
}

void removeObjects(){
    //usuwamy obiekty
    delete mushroom;
    delete rod;
    delete piston;
}



//Framebuffer size change event processing
void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); //Generate images in this resolution
	aspect=(float)width/(float)height; //Compute aspect ratio of width to height of the window
}

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}



//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) speed_y=PI/2;
        if (key == GLFW_KEY_RIGHT) speed_y=-PI/2;
        if (key == GLFW_KEY_UP) speed_x=PI/2;
        if (key == GLFW_KEY_DOWN) speed_x=-PI/2;
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT) speed_y=0;
        if (key == GLFW_KEY_RIGHT) speed_y=0;
        if (key == GLFW_KEY_UP) speed_x=0;
        if (key == GLFW_KEY_DOWN) speed_x=0;
    }
}


void move_circle(float &x, float &y, bool &course_x, bool &course_y){
    float spd = 0.05;
    if(course_x){
        x = x+ spd;
    }else{
        x = x - spd;
    }

    if(course_y){
        y = y+ spd;
    }else{
        y = y - spd;
    }

    std::cout<<x<<" "<<y<<std::endl;
    if(x >= 1.0)
        course_x = false;
    if(x <= -1.0)
        course_x = true;

    if(y >= 1.0)
        course_y = false;
    if(y <= -1.0)
        course_y = true;
}


//Procedura inicjuj¹ca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************

	glfwSetFramebufferSizeCallback(window, windowResize);//Zarejestruj procedurê obs³ugi zmiany rozmiaru ekranu.

	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury

    glClearColor(0,0,0,1); //Ustaw kolor czyszczenia bufora kolorów na czarno
    glEnable(GL_LIGHTING); //W³¹cz cieniowanie
    glEnable(GL_LIGHT0); //W³¹cz œwia³o numer 0
    glEnable(GL_DEPTH_TEST); //W³¹cz bufor g³êbokoœci
    glEnable(GL_COLOR_MATERIAL); //W³¹cz ustawianie koloru materia³u przez polecenia glColor
}




//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window,float angle_x, float angle_y, float *dane_f, bool *dane_b) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyœæ bufor kolorów i bufor g³êbokoœci

	//Przygotuj macierze rzutowania i widoku dla renderowanego obrazu
	mat4 P= perspective(50.0f*PI/180.0f,1.0f,1.0f, 50.0f); //Wylicz macierz rzutowania

	mat4 V=lookAt( //Wylicz macierz widoku
	vec3(0.0f,0.0f,-5.0f),
	vec3(0.0f,0.0f,0.0f),
	vec3(0.0f,1.0f,0.0f));

	glMatrixMode(GL_PROJECTION); //W³¹cz tryb modyfikacji macierzy rzutowania
	glLoadMatrixf(value_ptr(P)); //Skopiuj macierz rzutowania
	glMatrixMode(GL_MODELVIEW); //W³¹cz tryb modyfikacji macierzy model-widok. UWAGA! Macierz ta bêdzie ³adowana przed narysowaniem ka¿dego modelu


    //Rysowanie pojedynczego modelu
    //1. Oblicz i za³aduj macierz modelu
    mat4 M,R,T,S,I,R2;
    I = mat4(1);
    //R = rotate(I,dane_f[0]/180,glm::vec3(1,1,1));
    T = translate(I,vec3(dane_f[0]*0.2,dane_f[1]*0.2 -1,0));
    S = scale(I,vec3(0.3,0.3,0.3));
	//R=rotate(R,dane_f[1]*180,vec3(0.0f,1.0f,0.0f));
    R=rotate(I,angle_x,vec3(1.0f,0.0f,0.0f));
	R=rotate(R,-angle_y,vec3(0.0f,1.0f,0.0f));
	R=rotate(R,dane_f[0]/3,vec3(0,0,1));
	M=R*T*S;

	glLoadMatrixf(value_ptr(V*M)); //Za³aduj macierz model-widok
	//2. Narysuj obiekt
	//glColor3d(0,1,0);
	rod -> drawSolid();
    //Stop
/*
    P=perspective(120.0f*PI/180.0f, aspect, 1.0f, 10.0f); //Wylicz macierz rzutowania
	M=mat4(1.0f); //Wylicz macierz modelu
	M=rotate(M,angle,vec3(0.0f,1.0f,0.0f));
	glLoadMatrixf(value_ptr(V*M)); //Za³aduj macierz model-widok


    //glColor3d(1,0,0);
	piston -> drawSolid();
*/
	//Koniec
	glfwSwapBuffers(window);
}

int main(void)
{
    initObjects();//inicjujemy obiekty

	GLFWwindow* window; //WskaŸnik na obiekt reprezentuj¹cy okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów

	if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.


	if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Zainicjuj bibliotekê GLEW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj¹ce

	float angle=0; //Aktualny k¹t obrotu obiektu
	glfwSetTime(0); //Wyzeruj licznik czasu

	float angle_x = 0, angle_y = 0;
	float rod_x = 0, rod_y = -1;
	bool rod_cx = true, rod_cy = true;
	float dane_f[2];
	bool dane_b[2];
	//G³ówna pêtla
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
	    move_circle(rod_x, rod_y, rod_cx, rod_cy);
        dane_f[0] = rod_x;
        dane_f[1] = rod_y;
        dane_b[0] = rod_cx;
        dane_b[1] = rod_cy;
	    angle_x+=speed_x*glfwGetTime(); //Oblicz przyrost kąta obrotu i zwiększ aktualny kąt
        angle_y+=speed_y*glfwGetTime(); //Oblicz przyrost kąta obrotu i zwiększ aktualny kąt
	    glfwSetTime(0); //Wyzeruj timer
		drawScene(window,angle_x,angle_y, dane_f, dane_b); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);

	removeObjects();//usuwamy obiekty
}
