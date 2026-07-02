#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int win_id;
GLUquadric *quad;

/* ---- controle da animacao (danca "Stayin' Alive") ---- */
int   animate    = 1;     /* liga/desliga animacao (tecla espaco) */
float danceAngle = 0.0f;  /* angulo acumulado usado nas senoides  */
float danceSpeed = 4.0f;  /* velocidade da danca (tecla +/-)      */

#define DEG2RAD(a) ((a) * 3.14159265f / 180.0f)

/* =====================================================================
   Funcoes utilitárias de desenho (apenas primitivas/estruturas do OpenGL)
   ===================================================================== */

/* cilindro alinhado com o eixo Y, indo de y=0 até y=height */
void drawCylinderY(float baseR, float topR, float height, int slices, int stacks)
{
  glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, baseR, topR, height, slices, stacks);
  glPopMatrix();
}

/* caixa simples usando glScalef + glutSolidCube */
void drawBox(float sx, float sy, float sz)
{
  glPushMatrix();
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
  glPopMatrix();
}

/* desenha um prisma em forma de estrela de 5 pontas (corpo do Patrick) */
void drawStarPrism(float outerR, float innerR, float depth)
{
  int i;
  float ang, ax[10], ay[10];

  /* calcula os 10 vertices alternando raio externo/interno, ponta para cima */
  for (i = 0; i < 10; i++) {
    ang = DEG2RAD(90.0f - i * 36.0f);
    float r = (i % 2 == 0) ? outerR : innerR;
    ax[i] = r * cosf(ang);
    ay[i] = r * sinf(ang);
  }

  /* face frontal */
  glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, depth/2.0f);
    for (i = 0; i <= 10; i++)
      glVertex3f(ax[i % 10], ay[i % 10], depth/2.0f);
  glEnd();

  /* face traseira */
  glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, -depth/2.0f);
    for (i = 10; i >= 0; i--)
      glVertex3f(ax[i % 10], ay[i % 10], -depth/2.0f);
  glEnd();

  /* laterais ligando as duas faces */
  glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= 10; i++) {
      int k = i % 10;
      float nx = cosf(DEG2RAD(90.0f - k * 36.0f));
      float ny = sinf(DEG2RAD(90.0f - k * 36.0f));
      glNormal3f(nx, ny, 0.0f);
      glVertex3f(ax[k], ay[k], depth/2.0f);
      glVertex3f(ax[k], ay[k], -depth/2.0f);
    }
  glEnd();
}

/* olho simples: esfera branca + iris azul + pupila preta */
void drawEye(float scale)
{
  glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(scale, 12, 12);
    glPushMatrix();
      glTranslatef(0.0f, 0.0f, scale * 0.7f);
      glColor3f(0.1f, 0.4f, 0.9f);
      glutSolidSphere(scale * 0.55f, 10, 10);
      glTranslatef(0.0f, 0.0f, scale * 0.35f);
      glColor3f(0.0f, 0.0f, 0.0f);
      glutSolidSphere(scale * 0.28f, 8, 8);
    glPopMatrix();
  glPopMatrix();
}

/* =====================================================================
   Cenario de fundo: predio simples (Balde de Gelo / Krusty Krab estilizado)
   ===================================================================== */
void drawBuilding(float x, float y, float z)
{
  glPushMatrix();
    glTranslatef(x, y, z);

    /* corpo do predio */
    glColor3f(0.55f, 0.27f, 0.07f);
    glPushMatrix();
      glTranslatef(0.0f, 8.0f, 0.0f);
      drawBox(18.0f, 16.0f, 12.0f);
    glPopMatrix();

    /* telhado em forma de piramide */
    glColor3f(0.6f, 0.05f, 0.05f);
    glPushMatrix();
      glTranslatef(0.0f, 16.0f, 0.0f);
      glBegin(GL_TRIANGLES);
        /* frente */
        glNormal3f(0,0.4f,1);
        glVertex3f(-9.0f, 0.0f,  6.0f);
        glVertex3f( 9.0f, 0.0f,  6.0f);
        glVertex3f( 0.0f, 5.0f,  0.0f);
        /* tras */
        glNormal3f(0,0.4f,-1);
        glVertex3f( 9.0f, 0.0f, -6.0f);
        glVertex3f(-9.0f, 0.0f, -6.0f);
        glVertex3f( 0.0f, 5.0f,  0.0f);
        /* esquerda */
        glNormal3f(-1,0.4f,0);
        glVertex3f(-9.0f, 0.0f, -6.0f);
        glVertex3f(-9.0f, 0.0f,  6.0f);
        glVertex3f( 0.0f, 5.0f,  0.0f);
        /* direita */
        glNormal3f(1,0.4f,0);
        glVertex3f( 9.0f, 0.0f,  6.0f);
        glVertex3f( 9.0f, 0.0f, -6.0f);
        glVertex3f( 0.0f, 5.0f,  0.0f);
      glEnd();
    glPopMatrix();

    /* porta */
    glColor3f(0.3f, 0.15f, 0.05f);
    glPushMatrix();
      glTranslatef(0.0f, 3.0f, 6.05f);
      drawBox(3.0f, 6.0f, 0.2f);
    glPopMatrix();

    /* janelas */
    glColor3f(0.6f, 0.9f, 1.0f);
    glPushMatrix();
      glTranslatef(-6.0f, 10.0f, 6.05f);
      drawBox(2.5f, 2.5f, 0.2f);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(6.0f, 10.0f, 6.05f);
      drawBox(2.5f, 2.5f, 0.2f);
    glPopMatrix();

  glPopMatrix();
}

void drawGround(void)
{
  glColor3f(0.85f, 0.78f, 0.35f); /* areia */
  glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-60.0f, 0.0f, -40.0f);
    glVertex3f( 60.0f, 0.0f, -40.0f);
    glVertex3f( 60.0f, 0.0f,  40.0f);
    glVertex3f(-60.0f, 0.0f,  40.0f);
  glEnd();
}

/* =====================================================================
   Bob Esponja
   ===================================================================== */
void drawBobEsponja(float x, float y, float z, float phase)
{
  float swing = 35.0f * sinf(DEG2RAD(danceAngle + phase)); /* danca dos bracos */
  float legSwing = -20.0f * sinf(DEG2RAD(danceAngle + phase));
  float bob = 0.4f * sinf(DEG2RAD(2.0f * (danceAngle + phase)));

  glPushMatrix();
    glTranslatef(x, y + bob, z);

    /* corpo amarelo (esponja) */
    glColor3f(1.0f, 0.9f, 0.1f);
    glPushMatrix();
      glTranslatef(0.0f, 4.5f, 0.0f);
      drawBox(4.0f, 5.0f, 2.5f);
    glPopMatrix();

    /* furos da esponja (pequenas esferas marrom-claras) */
    glColor3f(0.85f, 0.7f, 0.05f);
    glPushMatrix();
      glTranslatef(-1.0f, 5.5f, 1.3f);
      glutSolidSphere(0.25f, 8, 8);
      glTranslatef(1.6f, -1.0f, 0.0f);
      glutSolidSphere(0.2f, 8, 8);
      glTranslatef(-0.8f, -1.6f, 0.0f);
      glutSolidSphere(0.22f, 8, 8);
    glPopMatrix();

    /* olhos */
    glPushMatrix();
      glTranslatef(-0.9f, 6.0f, 1.3f);
      drawEye(0.75f);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.9f, 6.0f, 1.3f);
      drawEye(0.75f);
    glPopMatrix();

    /* nariz */
    glColor3f(0.9f, 0.7f, 0.2f);
    glPushMatrix();
      glTranslatef(0.0f, 4.9f, 1.5f);
      glutSolidSphere(0.35f, 10, 10);
    glPopMatrix();

    /* boca */
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
      glTranslatef(0.0f, 3.7f, 1.35f);
      drawBox(2.0f, 0.25f, 0.2f);
    glPopMatrix();

    /* braco esquerdo (gira no ombro) */
    glColor3f(1.0f, 0.9f, 0.1f);
    glPushMatrix();
      glTranslatef(-2.0f, 6.0f, 0.0f);
      glRotatef(swing, 1.0f, 0.0f, 0.0f);
      glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
      drawCylinderY(0.35f, 0.3f, -3.0f, 10, 4);
    glPopMatrix();

    /* braco direito (movimento oposto) */
    glPushMatrix();
      glTranslatef(2.0f, 6.0f, 0.0f);
      glRotatef(-swing, 1.0f, 0.0f, 0.0f);
      glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
      drawCylinderY(0.35f, 0.3f, -3.0f, 10, 4);
    glPopMatrix();

    /* calca marrom */
    glColor3f(0.45f, 0.25f, 0.05f);
    glPushMatrix();
      glTranslatef(0.0f, 1.7f, 0.0f);
      drawBox(4.2f, 1.2f, 2.6f);
    glPopMatrix();

    /* perna esquerda */
    glColor3f(1.0f, 0.9f, 0.1f);
    glPushMatrix();
      glTranslatef(-1.0f, 1.1f, 0.0f);
      glRotatef(legSwing, 1.0f, 0.0f, 0.0f);
      drawCylinderY(0.4f, 0.4f, -1.8f, 10, 4);
      glTranslatef(0.0f, -1.8f, 0.0f);
      glColor3f(0.05f, 0.05f, 0.05f);
      drawBox(1.0f, 0.5f, 1.4f);
    glPopMatrix();

    /* perna direita (oposta) */
    glPushMatrix();
      glTranslatef(1.0f, 1.1f, 0.0f);
      glColor3f(1.0f, 0.9f, 0.1f);
      glRotatef(-legSwing, 1.0f, 0.0f, 0.0f);
      drawCylinderY(0.4f, 0.4f, -1.8f, 10, 4);
      glTranslatef(0.0f, -1.8f, 0.0f);
      glColor3f(0.05f, 0.05f, 0.05f);
      drawBox(1.0f, 0.5f, 1.4f);
    glPopMatrix();

  glPopMatrix();
}

/* =====================================================================
   Patrick Estrela
   ===================================================================== */
void drawPatrick(float x, float y, float z, float phase)
{
  float swing = 35.0f * sinf(DEG2RAD(danceAngle + phase));
  float legSwing = -20.0f * sinf(DEG2RAD(danceAngle + phase));
  float bob = 0.4f * sinf(DEG2RAD(2.0f * (danceAngle + phase)));

  glPushMatrix();
    glTranslatef(x, y + bob, z);

    /* corpo em forma de estrela */
    glColor3f(1.0f, 0.55f, 0.6f);
    glPushMatrix();
      glTranslatef(0.0f, 4.0f, 0.0f);
      drawStarPrism(3.5f, 1.5f, 2.0f);
    glPopMatrix();

    /* olhos */
    glPushMatrix();
      glTranslatef(-0.8f, 4.6f, 1.05f);
      drawEye(0.6f);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.8f, 4.6f, 1.05f);
      drawEye(0.6f);
    glPopMatrix();

    /* boca */
    glColor3f(0.5f, 0.05f, 0.05f);
    glPushMatrix();
      glTranslatef(0.0f, 3.6f, 1.05f);
      glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
      drawBox(0.5f, 1.6f, 0.2f);
    glPopMatrix();

    /* braco esquerdo saindo da ponta lateral da estrela */
    glColor3f(1.0f, 0.55f, 0.6f);
    glPushMatrix();
      glTranslatef(-3.3f, 4.2f, 0.0f);
      glRotatef(swing, 1.0f, 0.0f, 0.0f);
      glRotatef(25.0f, 0.0f, 0.0f, 1.0f);
      drawCylinderY(0.35f, 0.3f, -2.5f, 10, 4);
    glPopMatrix();

    /* braco direito */
    glPushMatrix();
      glTranslatef(3.3f, 4.2f, 0.0f);
      glRotatef(-swing, 1.0f, 0.0f, 0.0f);
      glRotatef(-25.0f, 0.0f, 0.0f, 1.0f);
      drawCylinderY(0.35f, 0.3f, -2.5f, 10, 4);
    glPopMatrix();

    /* perna esquerda saindo da ponta inferior */
    glPushMatrix();
      glTranslatef(-1.1f, 1.6f, 0.0f);
      glRotatef(legSwing, 1.0f, 0.0f, 0.0f);
      drawCylinderY(0.4f, 0.4f, -1.6f, 10, 4);
    glPopMatrix();

    /* perna direita */
    glPushMatrix();
      glTranslatef(1.1f, 1.6f, 0.0f);
      glRotatef(-legSwing, 1.0f, 0.0f, 0.0f);
      drawCylinderY(0.4f, 0.4f, -1.6f, 10, 4);
    glPopMatrix();

  glPopMatrix();
}

/* =====================================================================
   Callbacks
   ===================================================================== */

void myKeyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 'R':
    case 'r':
      glColor3f(1.0f, 0.0f, 0.0f);
      break;
    case 'G':
    case 'g':
      glColor3f(0.0f, 1.0f, 0.0f);
      break;
    case 'B':
    case 'b':
      glColor3f(0.0f, 0.0f, 1.0f);
      break;
    case ' ': /* liga/desliga a danca */
      animate = !animate;
      break;
    case '+': /* aumenta velocidade da danca */
      danceSpeed += 1.0f;
      break;
    case '-': /* diminui velocidade da danca */
      if (danceSpeed > 1.0f) danceSpeed -= 1.0f;
      break;
  }
  glutPostRedisplay();
}

void myKeyboardSpecial(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:
      glutFullScreen();
      break;
    case GLUT_KEY_DOWN:
      glutReshapeWindow(640, 480);
      break;
    default:
      printf("Você apertou a tecla especial código: %d\n", key);
      break;
  }
}

void myMouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON)
    if (state == GLUT_DOWN) {
      float r, g, b;
      r = (double)rand() / (double)RAND_MAX;
      g = (double)rand() / (double)RAND_MAX;
      b = (double)rand() / (double)RAND_MAX;
      glColor3f(r,g,b);
      printf("%.2f, %.2f, %.2f, na posicao %d, %d\n", r, g, b, x, y);
    }
  glutPostRedisplay();
}

/* timer que avanca a animacao da danca */
void myTimer(int value)
{
  if (animate) {
    danceAngle += danceSpeed;
    if (danceAngle > 36000.0f) danceAngle -= 36000.0f;
  }
  glutPostRedisplay();
  glutTimerFunc(30, myTimer, 0);
}

void init(void)
{
  /* ceu/mar ao fundo */
  glClearColor(0.45f, 0.75f, 0.95f, 0.0f);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  GLfloat light_ambient[]  = { 0.3, 0.3, 0.3, 1.0 };
  GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 10.0, 30.0, 30.0, 0.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  glColor3f(0.5, 1.0, 0.5);

  quad = gluNewQuadric();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0f, 14.0f, 35.0f,  0.0f, 6.0f, 0.0f,  0.0f, 1.0f, 0.0f);

  drawGround();
  drawBuilding(0.0f, 0.0f, -15.0f);

  /* Bob Esponja e Patrick dancando "Stayin' Alive" */
  drawBobEsponja(-5.0f, 0.0f, 5.0f, 0.0f);
  drawPatrick(5.0f, 0.0f, 5.0f, 180.0f); /* fase oposta -> dança alternada */

  glFlush();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  win_id = glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(myKeyboard);
  glutSpecialFunc(myKeyboardSpecial);
  glutMouseFunc(myMouse);
  glutTimerFunc(30, myTimer, 0);
  glutMainLoop();
  return 0;
}
