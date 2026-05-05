#include <GL/freeglut.h>
#include <cmath>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

static constexpr size_t MAX_EXPLODE_FACTOR = 8;

static float angle = 0.0f;

static float explodeFactor = 0.0f;

bool transparencyEnabled = false;
bool textureEnabled = true;
bool lightingEnabled = true;

static float cameraAngle = 1.0f;
static float cameraDistance = 6.0f;
static float cameraHeight = 3.0f;

GLuint texture;
GLuint loadTexture(const std::string& filename)
{
    int width, height, channels;

    // загрузка изображения
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename.c_str(),
        &width, &height, &channels, 0);

    if (!data) {
        throw std::runtime_error("Failed to load image: " + filename);
    }

    // определяем формат
    GLenum format;
    if (channels == 1)       
        format = GL_RED;
    else if (channels == 3)  
        format = GL_RGB;
    else if (channels == 4)  
        format = GL_RGBA;
    else {
        stbi_image_free(data);
        throw std::runtime_error("Unsupported channel count");
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    // фильтрация
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return tex;
}

void drawExplodingCube(float size) {
    float half = size / 2.0f;

    glBegin(GL_QUADS);

    // передняя грань
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); 
    glVertex3f(-half,-half,half+explodeFactor);
    glTexCoord2f(1,0); 
    glVertex3f(half,-half,half+explodeFactor);
    glTexCoord2f(1,1); 
    glVertex3f(half,half,half+explodeFactor);
    glTexCoord2f(0,1); 
    glVertex3f(-half,half,half+explodeFactor);

    // задняя грань
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); 
    glVertex3f(-half,-half,-half-explodeFactor);
    glTexCoord2f(1,0); 
    glVertex3f(half,-half,-half-explodeFactor);
    glTexCoord2f(1,1); 
    glVertex3f(half,half,-half-explodeFactor);
    glTexCoord2f(0,1); 
    glVertex3f(-half,half,-half-explodeFactor);

    // правая грань
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); 
    glVertex3f(half+explodeFactor,-half,-half);
    glTexCoord2f(1,0); 
    glVertex3f(half+explodeFactor,-half,half);
    glTexCoord2f(1,1); 
    glVertex3f(half+explodeFactor,half,half);
    glTexCoord2f(0,1); 
    glVertex3f(half+explodeFactor,half,-half);

    // левая грань
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); 
    glVertex3f(-half-explodeFactor,-half,-half);
    glTexCoord2f(1,0); 
    glVertex3f(-half-explodeFactor,-half,half);
    glTexCoord2f(1,1); 
    glVertex3f(-half-explodeFactor,half,half);
    glTexCoord2f(0,1); 
    glVertex3f(-half-explodeFactor,half,-half);

    // верхняя грань
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); 
    glVertex3f(-half,half+explodeFactor,-half);
    glTexCoord2f(1,0); 
    glVertex3f(half,half+explodeFactor,-half);
    glTexCoord2f(1,1); 
    glVertex3f(half,half+explodeFactor,half);
    glTexCoord2f(0,1); 
    glVertex3f(-half,half+explodeFactor,half);

    // нижняя грань
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); 
    glVertex3f(-half,-half-explodeFactor,-half);
    glTexCoord2f(1,0); 
    glVertex3f(half,-half-explodeFactor,-half);
    glTexCoord2f(1,1); 
    glVertex3f(half,-half-explodeFactor,half);
    glTexCoord2f(0,1); 
    glVertex3f(-half,-half-explodeFactor,half);

    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // позиция камеры
    float camX = cameraDistance * cos(cameraAngle);
    float camY = cameraHeight;
    float camZ = cameraDistance * sin(cameraAngle);

    gluLookAt(camX, camY, camZ,
              0, 0, 0,
              0, 1, 0);

    // координаты источника света
    float lightX = 3 * cos(angle);
    float lightZ = 3 * sin(angle);

    GLfloat lightPos[] = {lightX, 2.0f, lightZ, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glPushAttrib(GL_ALL_ATTRIB_BITS);


    // Рисуем сферу источника света
    glPushMatrix();

    glTranslatef(lightX, 2.0f, lightZ);

    glDisable(GL_TEXTURE_2D); 
    glDisable(GL_LIGHTING); 
    glColor3f(1, 1, 0);

    glutSolidSphere(0.15, 20, 20);

    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
    glPopAttrib();

    if (textureEnabled) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    if (transparencyEnabled) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glDepthMask(GL_FALSE);

        glColor4f(1, 1, 1, 0.4f);
    } 
    else {
        glDisable(GL_BLEND);
        // glDepthMask(GL_TRUE);

        glColor3f(1, 1, 1);
    }

    drawExplodingCube(2.0f);
    // glDepthMask(GL_TRUE);

    glutSwapBuffers();
}


void update(int value) {
    angle += 0.02f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void keyboard(unsigned char key, int x, int y) {
    if (key=='a' || key=='A')
        cameraAngle += 0.1f;

    if (key=='d' || key=='D')
        cameraAngle -= 0.1f;

    if (key=='w' || key=='W')
        cameraDistance -= 0.3f;

    if (key=='s' || key=='S')
        cameraDistance += 0.3f;

    if (key=='q' || key=='Q')
        cameraHeight += 0.3f;

    if (key=='e' || key=='E')
        cameraHeight -= 0.3f;

    if (key=='z' || key=='Z')
        explodeFactor += 0.1f;

    if (key=='x' || key=='X')
        explodeFactor -= 0.1f;

    if (key=='r'|| key=='R')
        textureEnabled=!textureEnabled;

    if (key=='l'|| key=='L') {
        lightingEnabled=!lightingEnabled;

        if(lightingEnabled) {
            glEnable(GL_LIGHTING);
        }
        else {
            glDisable(GL_LIGHTING);
        }
    }

    if (explodeFactor < 0) {
        explodeFactor = 0;
    }

    // прозрачность
    if (key=='t' || key=='T')
        transparencyEnabled = !transparencyEnabled;

    if (explodeFactor < 0) {
        explodeFactor = 0;
    }
    
    if (explodeFactor > 15) {
	    explodeFactor = 15;
    }
    glutPostRedisplay();
}


void init(const std::string& filename) {
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightDiffuse[] = {1, 1, 1, 1};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    texture = loadTexture(filename);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 1.0, 100);

    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
    if (argc < 2) {
        throw std::runtime_error("Too few arguments");
    }

    std::string filename = argv[1];
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
 
    glutCreateWindow("Lab 5");
    init(filename);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}