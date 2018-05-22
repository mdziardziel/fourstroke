/*
Niniejszy program jest wolnym oprogramowaniem; mo�esz go
rozprowadza� dalej i / lub modyfikowa� na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacj� Wolnego
Oprogramowania - wed�ug wersji 2 tej Licencji lub(wed�ug twojego
wyboru) kt�rej� z p�niejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej�, i� b�dzie on
u�yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domy�lnej
gwarancji PRZYDATNO�CI HANDLOWEJ albo PRZYDATNO�CI DO OKRE�LONYCH
ZASTOSOWA�.W celu uzyskania bli�szych informacji si�gnij do
Powszechnej Licencji Publicznej GNU.

Z pewno�ci� wraz z niniejszym programem otrzyma�e� te� egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
je�li nie - napisz do Free Software Foundation, Inc., 59 Temple
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
#include "constants.h"
#include "allmodels.h"

using namespace glm;

float aspect=1;
float speed=3.14; //pr�dko�� k�towa obrotu w radianach na sekund�

//Framebuffer size change event processing
void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); //Generate images in this resolution
	aspect=(float)width/(float)height; //Compute aspect ratio of width to height of the window
}

//Procedura obs�ugi b��d�w
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjuj�ca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************

	glfwSetFramebufferSizeCallback(window, windowResize);//Zarejestruj procedur� obs�ugi zmiany rozmiaru ekranu.

    glClearColor(0,0,0,1); //Ustaw kolor czyszczenia bufora kolor�w na czarno
    glEnable(GL_LIGHTING); //W��cz cieniowanie
    glEnable(GL_LIGHT0); //W��cz �wia�o numer 0
    glEnable(GL_DEPTH_TEST); //W��cz bufor g��boko�ci
    glEnable(GL_COLOR_MATERIAL); //W��cz ustawianie koloru materia�u przez polecenia glColor
}




//Procedura rysuj�ca zawarto�� sceny
void drawScene(GLFWwindow* window,float angle) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczy�� bufor kolor�w i bufor g��boko�ci

	//Przygotuj macierze rzutowania i widoku dla renderowanego obrazu
	mat4 P=perspective(50.0f*PI/180.0f, aspect, 1.0f, 50.0f); //Wylicz macierz rzutowania

	mat4 V=lookAt( //Wylicz macierz widoku
	vec3(0.0f,0.0f,-5.0f),
	vec3(0.0f,0.0f,0.0f),
	vec3(0.0f,1.0f,0.0f));

	glMatrixMode(GL_PROJECTION); //W��cz tryb modyfikacji macierzy rzutowania
	glLoadMatrixf(value_ptr(P)); //Skopiuj macierz rzutowania
	glMatrixMode(GL_MODELVIEW); //W��cz tryb modyfikacji macierzy model-widok. UWAGA! Macierz ta b�dzie �adowana przed narysowaniem ka�dego modelu


    //Rysowanie pojedynczego modelu
    //1. Oblicz i za�aduj macierz modelu
	mat4 M=mat4(1.0f); //Wylicz macierz modelu
	M=rotate(M,angle,vec3(0.0f,1.0f,0.0f));
	glLoadMatrixf(value_ptr(V*M)); //Za�aduj macierz model-widok
	//2. Narysuj obiekt
	glColor3d(0,1,0);
	Models::torus.drawSolid();
    //Stop

	//Koniec
	glfwSwapBuffers(window);
}

int main(void)
{
	GLFWwindow* window; //Wska�nik na obiekt reprezentuj�cy okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedur� obs�ugi b��d�w

	if (!glfwInit()) { //Zainicjuj bibliotek� GLFW
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utw�rz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.


	if (!window) //Je�eli okna nie uda�o si� utworzy�, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje si� aktywny i polecenia OpenGL b�d� dotyczy� w�a�nie jego.
	glfwSwapInterval(1); //Czekaj na 1 powr�t plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Zainicjuj bibliotek� GLEW
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj�ce

	float angle=0; //Aktualny k�t obrotu obiektu
	glfwSetTime(0); //Wyzeruj licznik czasu
	//G��wna p�tla
	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
	    angle+=speed*glfwGetTime(); //Powi�ksz k�t obrotu o szybko�� obrotu razy czas, kt�ry min�� od poprzedniej klatki
	    glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window,angle); //Wykonaj procedur� rysuj�c�
		glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
	}

	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}
