#include "objloader.hpp"
#include "class_template.hpp"

Template::Template(char * path){
    this -> path = path; //bierzemy œcie¿kê do modelu
    loadeObj(path, &vertices, &vertexNormals, &texCoords,&normals, &colors, &vertexCount); //przetwarzamy obiekt na tablice wwierzcho³ków, normalnych, textur, itp
}

void Template::drawSolid() { //metoda rysuj¹ca modele
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(4,GL_FLOAT,0,vertices);
    //glColorPointer(4,GL_FLOAT,0,colors);
    glNormalPointer(GL_FLOAT,sizeof(float)*4,vertexNormals);
    glTexCoordPointer(2,GL_FLOAT,0,texCoords);

    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}



