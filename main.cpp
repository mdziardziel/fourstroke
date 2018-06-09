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
#include "lodepng.h"

using namespace glm;


float aspect=1;
float speed=3.14; //prêdkoœæ k¹towa obrotu w radianach na sekundê
float speed_x = 0, speed_y = 0;
GLuint tex; //Globalnie

//deklarujemy obiekty Template
Template *mushroom;
Template *rod;
Template *piston;
Template *glow;
Template *valve;
Template *crankshaft;

void initObjects(){
    //inicjalzujemy obiekty Template (wgrywamy tekstury, kolory itp)
    mushroom = new Template("objects\\mushroom.obj");
    rod = new Template("objects\\rod.obj");
    piston = new Template("objects\\piston.obj");
    glow = new Template("objects\\glow.obj");
    valve = new Template("objects\\valve.obj");
    crankshaft = new Template("objects\\crankshaft.obj");
}

void removeObjects(){
    //usuwamy obiekty
    delete mushroom;
    delete rod;
    delete piston;
    delete glow;
    delete valve;
    delete crankshaft;
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


void move_circle(float &x, float &y, bool &course_x, bool &course_y, bool &valve_move, float &st360){
    float spd = 3;
    float promien = 0.12;

    st360 = st360 + spd;
    if(st360>=360)
        st360 = 0;

    if(st360==90)
        valve_move = !valve_move;

    y = std::sin(st360*PI/180)*promien;
    if(st360!=90)x = std::cos(st360*PI/180)*promien;
    else x = 0;

    //std::cout<<valve_move<<" "<<st360<<" "<<x<<" "<<y<<std::endl;


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
    //glEnable(GL_COLOR_MATERIAL); //W³¹cz ustawianie koloru materia³u przez polecenia glColor
    glEnable(GL_NORMALIZE);
    std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
    unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
    //Wczytaj obrazek
    unsigned error = lodepng::decode(image, width, height, "metal.png");

    //Import do pamięci karty graficznej
    glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
    glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
    //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_TEXTURE_2D);
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


    //rysowanie prętu
    glBindTexture(GL_TEXTURE_2D,tex);
    glColor4f(0,1,1,1);
    mat4 M,R,T,S,I,R2;
    I = mat4(1);
    T = translate(I,vec3(dane_f[0],dane_f[1] -1,0));
    S = scale(I,vec3(0.2,0.2,0.2));
    R=rotate(I,angle_x,vec3(1.0f,0.0f,0.0f));
	R=rotate(R,-angle_y,vec3(0.0f,1.0f,0.0f));
	R=rotate(R,dane_f[0],vec3(0,0,1));
	//std::cout<<dane_f[0]<<" "<<dane_f[1]<<std::endl;
	M=R*T*S;

	glLoadMatrixf(value_ptr(V*M)); //Za³aduj macierz model-widok
	rod -> drawSolid();



	//rysowanie tłoka
	glColor4f(1,0,1,1);
    I = mat4(1);
    T = translate(I,vec3(0,dane_f[1] + 0.4,0));
    S = scale(I,vec3(0.25,0.25,0.25));
    R=rotate(I,angle_x,vec3(1.0f,0.0f,0.0f));
	R=rotate(R,-angle_y,vec3(0.0f,1.0f,0.0f));
	M=R*T*S;

	glLoadMatrixf(value_ptr(V*M)); //Za³aduj macierz model-widok
	piston -> drawSolid();



	//rysowanie świecy
	glColor4f(1,0.5,0.5,1);
    I = mat4(1);
    T = translate(I,vec3(0.02,1.05,0));
    S = scale(I,vec3(0.2,0.2,0.2));
    R=rotate(I,angle_x,vec3(1.0f,0.0f,0.0f));
	R=rotate(R,-angle_y,vec3(0.0f,1.0f,0.0f));
	M=R*T*S;

	glLoadMatrixf(value_ptr(V*M)); //Za³aduj macierz model-widok
	glow -> drawSolid();

    //rysowanie zaworu po prawej
    glColor4f(1,1,0,1);
    I = mat4(1);
    if(dane_b[2] && dane_f[0] < 0)
        if(dane_f[1]<0)
            T = translate(I,vec3((dane_f[0])*0.025+0.15,-(dane_f[1]+1)*0.05 + 1.03,0));
        else
            T = translate(I,vec3((dane_f[0])*0.025+0.15,(dane_f[1]-1)*0.05 + 1.03,0));
    else
        T = translate(I,vec3(0.15,1.03,0));
    S = scale(I,vec3(0.15,0.15,0.15));
    R=rotate(I,angle_x,vec3(1.0f,0.0f,0.0f));
	R=rotate(R,-angle_y,vec3(0.0f,1.0f,0.0f));
	R=rotate(R,15*PI/180,vec3(0,0,1));
	M=R*T*S;

	glLoadMatrixf(value_ptr(V*M)); //Za³aduj macierz model-widok
	valve -> drawSolid();

	//rysowanie zaworu po lewej
	glColor4f(1,1,0,1);
    I = mat4(1);
    if(dane_b[2] && dane_f[0] > 0)
        if(dane_f[1]<0)
            T = translate(I,vec3((dane_f[0])*0.025-0.12,-(dane_f[1]+1)*0.05 + 1.03,0));
        else
            T = translate(I,vec3((dane_f[0])*0.025-0.12,(dane_f[1]-1)*0.05 + 1.03,0));
    else
        T = translate(I,vec3(-0.12,1.03,0));
    S = scale(I,vec3(0.15,0.15,0.15));
    R=rotate(I,angle_x,vec3(1.0f,0.0f,0.0f));
	R=rotate(R,-angle_y,vec3(0.0f,1.0f,0.0f));
	R=rotate(R,-15*PI/180,vec3(0,0,1));
	M=R*T*S;

	glLoadMatrixf(value_ptr(V*M)); //Za³aduj macierz model-widok
	valve -> drawSolid();


    //rysowanie wału korbowego
    glColor4f(1,0,0,1);
    I = mat4(1);
    T = translate(I,vec3(0.2,-0.75,0));
    S = scale(I,vec3(0.15,0.105,0.15));
    R=rotate(I,angle_x,vec3(1.0f,0.0f,0.0f));
	R=rotate(R,-angle_y,vec3(0.0f,1.0f,0.0f));
	R=rotate(R,90*PI/180,vec3(0,1,0.0));
	R2=rotate(I,(-dane_f[2] +90)*PI/180 ,vec3(1,0,0));
	M=R*T*R2*S;

	glLoadMatrixf(value_ptr(V*M)); //Za³aduj macierz model-widok
	crankshaft -> drawSolid();


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

	window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.


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
	float st360 = 0;
	bool rod_cx = true, rod_cy = true, valve_move = true;
	float dane_f[3];
	bool dane_b[3];
	//G³ówna pêtla
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
	    move_circle(rod_x, rod_y, rod_cx, rod_cy, valve_move, st360);
        dane_f[0] = rod_x;
        dane_f[1] = rod_y;
        dane_f[2] = st360;
        dane_b[0] = rod_cx;
        dane_b[1] = rod_cy;
        dane_b[2] = valve_move;
	    angle_x+=speed_x*glfwGetTime(); //Oblicz przyrost kąta obrotu i zwiększ aktualny kąt
        angle_y+=speed_y*glfwGetTime(); //Oblicz przyrost kąta obrotu i zwiększ aktualny kąt
	    glfwSetTime(0); //Wyzeruj timer
		drawScene(window,angle_x,angle_y, dane_f, dane_b); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}
    glDeleteTextures(1,&tex);
	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);

	removeObjects();//usuwamy obiekty
}
