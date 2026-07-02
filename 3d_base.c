#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int win_id;
GLUquadric *quad;

/* ---- danca ---- */
int   animate    = 1;
float danceAngle = 0.0f;
float danceSpeed = 11.0f;

/* ---- casa do Patrick (pedra) ---- */
float rockAngle   = 0.0f;   /* 0 = fechada, 85 = aberta */
int   rockOpening = 0;
int   rockClosing = 0;

#define DEG2RAD(a) ((a) * 3.14159265f / 180.0f)
#define PI 3.14159265f

/* =====================================================================
   Primitivas auxiliares
   ===================================================================== */

/* cilindro alinhado com +Y, de y=0 ate y=height (height pode ser negativo) */
void drawCylinderY(float baseR, float topR, float height, int slices, int stacks)
{
    glPushMatrix();
    if (height < 0.0f) {
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        height = -height;
    }
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, baseR, topR, height, slices, stacks);
    glPopMatrix();
}

/* cilindro alinhado com +X */
void drawCylinderX(float baseR, float topR, float length, int sl)
{
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quad, baseR, topR, length, sl, 1);
    glPopMatrix();
}

/* cilindro alinhado com +Z */
void drawCylinderZ(float baseR, float topR, float length, int sl)
{
    gluCylinder(quad, baseR, topR, length, sl, 1);
}

void drawBox(float sx, float sy, float sz)
{
    glPushMatrix();
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

/* olho: esfera branca + iris azul + pupila preta (projetados para frente) */
void drawEye(float scale)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(scale, 14, 14);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, scale * 0.68f);
        glColor3f(0.1f, 0.45f, 0.9f);
        glutSolidSphere(scale * 0.58f, 12, 12);
        glTranslatef(0.0f, 0.0f, scale * 0.38f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glutSolidSphere(scale * 0.30f, 10, 10);
    glPopMatrix();
}

/* =====================================================================
   Bob Esponja
   ===================================================================== */

void drawBobArm(float side, float phase)
{
    float swing = 40.0f * sinf(DEG2RAD(danceAngle + phase));
    glPushMatrix();
        glTranslatef(side * 2.1f, 4.8f, 0.0f);
        glRotatef(side * 22.0f, 0.0f, 0.0f, 1.0f);   /* abre para o lado */
        glRotatef(side * swing, 1.0f, 0.0f, 0.0f);    /* balanca frente/tras */
        /* braco fino amarelo */
        glColor3f(1.0f, 0.88f, 0.1f);
        drawCylinderY(0.22f, 0.18f, -2.2f, 10, 4);
        /* mao como esfera pequena */
        glTranslatef(0.0f, -2.2f, 0.0f);
        glutSolidSphere(0.28f, 10, 10);
    glPopMatrix();
}

void drawBobLeg(float side, float phase)
{
    float legSwing = side * 22.0f * sinf(DEG2RAD(danceAngle + phase));
    glPushMatrix();
        glTranslatef(side * 0.9f, 1.05f, 0.0f);
        glRotatef(legSwing, 1.0f, 0.0f, 0.0f);
        /* perna fina amarela */
        glColor3f(1.0f, 0.88f, 0.1f);
        drawCylinderY(0.28f, 0.26f, -1.8f, 10, 4);
        glTranslatef(0.0f, -1.8f, 0.0f);
        /* meia branca */
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCylinderY(0.30f, 0.30f, -0.75f, 10, 4);
        /* listras da meia (cinza) */
        glColor3f(0.72f, 0.72f, 0.72f);
        glPushMatrix();
            glTranslatef(0.0f, -0.25f, 0.0f);
            drawCylinderY(0.31f, 0.31f, -0.08f, 10, 2);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, -0.45f, 0.0f);
            drawCylinderY(0.31f, 0.31f, -0.08f, 10, 2);
        glPopMatrix();
        /* sapato preto (largo, oval) */
        glTranslatef(0.0f, -0.75f, 0.0f);
        glColor3f(0.05f, 0.05f, 0.05f);
        glPushMatrix();
            glTranslatef(0.0f, -0.28f, 0.45f);
            glScalef(1.4f, 0.55f, 2.0f);
            glutSolidSphere(0.65f, 12, 8);   /* sapato ovalado */
        glPopMatrix();
    glPopMatrix();
}

void drawBobEsponja(float x, float y, float z, float phase)
{
    float bob = 0.35f * sinf(DEG2RAD(2.0f * (danceAngle + phase)));

    glPushMatrix();
        glTranslatef(x, y + bob, z);

        /* --- corpo amarelo  --- */
        glColor3f(1.0f, 0.88f, 0.1f);
        glPushMatrix();
            glTranslatef(0.0f, 4.5f, 0.0f);
            drawBox(4.0f, 5.0f, 2.5f);
        glPopMatrix();


        /* --- olhos --- */
        glPushMatrix();
            glTranslatef(-0.85f, 6.05f, 1.32f);
            drawEye(0.72f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.85f, 6.05f, 1.32f);
            drawEye(0.72f);
        glPopMatrix();

        /* cílios */
        glColor3f(0.0f, -5.0f, 0.0f);
        float cilX[] = {-1.4f, -0.85f, -0.3f};
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
                glTranslatef(cilX[i], 6.65f, 1.32f);
                glRotatef(-15.0f + i*15.0f, 0.0f, 0.0f, 1.0f);
                drawCylinderY(0.04f, 0.02f, 0.45f, 6, 2);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-cilX[i], 6.65f, 1.32f);
                glRotatef(15.0f - i*15.0f, 0.0f, 0.0f, 1.0f);
                drawCylinderY(0.04f, 0.02f, 0.45f, 6, 2);
            glPopMatrix();
        }

        /* --- nariz --- */
        glColor3f(0.95f, 0.72f, 0.18f);
        glPushMatrix();
            glTranslatef(0.0f, 5.0f, 1.55f);
            glutSolidSphere(0.38f, 10, 10);
        glPopMatrix();

        /* --- boca (curva simulada com 3 boxes em arco) --- */
        glColor3f(0.85f, 0.55f, 0.05f);  /* interior boca */
        glPushMatrix();
            glTranslatef(0.0f, 3.75f, 1.32f);
            drawBox(1.9f, 0.55f, 0.15f);
        glPopMatrix();
        /* dentes */
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            glTranslatef(-0.35f, 3.85f, 1.36f);
            drawBox(0.4f, 0.45f, 0.12f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.35f, 3.85f, 1.36f);
            drawBox(0.4f, 0.45f, 0.12f);
        glPopMatrix();

        /* --- gravata vermelha --- */
        glColor3f(0.85f, 0.05f, 0.05f);
        glPushMatrix();
            glTranslatef(0.0f, 2.0f, 1.28f);
            glRotatef(5.0f, 0.0f, 0.0f, 1.0f);
            drawBox(0.3f, 1.2f, 0.1f);
            /* ponta da gravata */
            glTranslatef(0.05f, -0.50f, 0.3f);
            glBegin(GL_TRIANGLES);
                glNormal3f(0,0,1);
                glVertex3f(-0.25f, 0.0f, 0.0f);
                glVertex3f( 0.25f, 0.0f, 0.0f);
                glVertex3f( 0.05f,-0.35f, 0.0f);
            glEnd();
        glPopMatrix();

        /* --- calca marrom quadrada --- */
        glColor3f(0.42f, 0.22f, 0.04f);
        glPushMatrix();
            glTranslatef(0.0f, 1.7f, 0.0f);
            drawBox(4.2f, 1.3f, 2.6f);
        glPopMatrix();
        /* cinto preto */
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
            glTranslatef(0.0f, 2.36f, 0.0f);
            drawBox(4.22f, 0.22f, 2.62f);
        glPopMatrix();
        /* fivela do cinto */
        glColor3f(0.7f, 0.65f, 0.1f);
        glPushMatrix();
            glTranslatef(0.0f, 2.36f, 1.32f);
            drawBox(0.55f, 0.3f, 0.12f);
        glPopMatrix();

        /* --- bracos e pernas animados --- */
        drawBobArm(-1.0f, phase);
        drawBobArm( 1.0f, phase + 180.0f);
        drawBobLeg(-1.0f, phase);
        drawBobLeg( 1.0f, phase + 180.0f);

    glPopMatrix();
}

/* =====================================================================
   Patrick Estrela
   ===================================================================== */

/*
 * buildRoundedStar: gera o poligono da estrela com pontas arredondadas.
 * Cada uma das 5 pontas aguda e substituida por um arco circular de
 * arcSteps+1 vertices. O arco e a circunferencia inscrita no angulo da
 * ponta com raio arcR, sem adicionar nenhum objeto externo.
 * Retorna os vertices em vx/vy e a contagem em *n.
 */
#define STAR_MAX_V 80
void buildRoundedStar(float outerR, float innerR, float arcR, int arcSteps,
                      float *vx, float *vy, int *n)
{
    *n = 0;
    for (int t = 0; t < 5; t++) {
        float tipAng  = DEG2RAD(90.0f - t * 72.0f);
        float tx = outerR * cosf(tipAng);
        float ty = outerR * sinf(tipAng);

        /* vertices internos adjacentes */
        float aL = tipAng + DEG2RAD(36.0f);
        float aR = tipAng - DEG2RAD(36.0f);
        float lx = innerR * cosf(aL), ly = innerR * sinf(aL);
        float rx = innerR * cosf(aR), ry = innerR * sinf(aR);

        /* vetores das arestas a partir da ponta, normalizados */
        float elx = lx-tx, ely = ly-ty, el = sqrtf(elx*elx+ely*ely);
        float erx = rx-tx, ery = ry-ty, er = sqrtf(erx*erx+ery*ery);
        elx/=el; ely/=el; erx/=er; ery/=er;

        /* meio-angulo da ponta */
        float c = fmaxf(-1.0f, fminf(1.0f, elx*erx + ely*ery));
        float halfTheta = acosf(c) * 0.5f;

        /* centro do arco: recua da ponta ao longo da direcao interna */
        float offset = arcR / sinf(halfTheta);
        float cx = tx - offset * cosf(tipAng);
        float cy = ty - offset * sinf(tipAng);

        /* angulos do arco a partir do centro (simetrico em torno de tipAng) */
        float arcHalf = (float)PI * 0.5f - halfTheta;
        float aStart  = tipAng + arcHalf;   /* lado esquerdo da ponta */
        float aEnd    = tipAng - arcHalf;   /* lado direito da ponta  */

        /* emite vertices do arco em sentido horario (aStart -> aEnd) */
        for (int j = 0; j <= arcSteps; j++) {
            float a = aStart + (aEnd - aStart) * (float)j / arcSteps;
            vx[*n] = cx + arcR * cosf(a);
            vy[*n] = cy + arcR * sinf(a);
            (*n)++;
        }

        /* vertice interno direito (conecta ao proximo trecho) */
        vx[*n] = rx;
        vy[*n] = ry;
        (*n)++;
    }
}

void drawRoundedStarPrism(float outerR, float innerR, float arcR, int arcSteps, float depth)
{
    float vx[STAR_MAX_V], vy[STAR_MAX_V];
    int n;
    buildRoundedStar(outerR, innerR, arcR, arcSteps, vx, vy, &n);

    float hz = depth * 0.5f;

    /* face frontal */
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, hz);
        for (int i = 0; i < n; i++) glVertex3f(vx[i], vy[i], hz);
        glVertex3f(vx[0], vy[0], hz);
    glEnd();

    /* face traseira (winding invertido) */
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, -hz);
        for (int i = n-1; i >= 0; i--) glVertex3f(vx[i], vy[i], -hz);
        glVertex3f(vx[n-1], vy[n-1], -hz);
    glEnd();

    /* laterais com normais corretas por segmento */
    glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= n; i++) {
            int  k  = i % n;
            int  kp = (i + 1) % n;
            /* normal apontando para fora: media dos dois vertices do segmento */
            float nx = (vx[k] + vx[kp]) * 0.5f;
            float ny = (vy[k] + vy[kp]) * 0.5f;
            float len = sqrtf(nx*nx + ny*ny);
            if (len > 0.001f) { nx /= len; ny /= len; }
            glNormal3f(nx, ny, 0.0f);
            glVertex3f(vx[k], vy[k],  hz);
            glVertex3f(vx[k], vy[k], -hz);
        }
    glEnd();
}


void drawPatrick(float x, float y, float z, float phase)
{
    float bob = 0.35f * sinf(DEG2RAD(2.0f * (danceAngle + phase)));

    glPushMatrix();
        glTranslatef(x, y + bob, z);

        /* corpo estrela rosa */
        glColor3f(1.0f, 0.55f, 0.6f);
        glPushMatrix();
            glTranslatef(0.0f, 4.2f, 0.0f);
            drawRoundedStarPrism(3.4f, 1.9f, 0.6f, 6, 2.2f);
        glPopMatrix();

        /* calcinha verde com listras roxas (Patrick usa na barriga) */
        glColor3f(0.15f, 0.55f, 0.2f);
        glPushMatrix();
            glTranslatef(0.0f, 2.5f, 1.12f);
            drawBox(2.4f, 1.0f, 0.15f);
        glPopMatrix();
        glColor3f(0.55f, 0.1f, 0.7f);
        for (int i = -1; i <= 1; i++) {
            glPushMatrix();
                glTranslatef(i * 0.7f, 2.5f, 1.16f);
                drawBox(0.18f, 1.05f, 0.1f);
            glPopMatrix();
        }

        /* olhos */
        glPushMatrix();
            glTranslatef(-0.75f, 4.75f, 1.12f);
            drawEye(0.58f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.75f, 4.75f, 1.12f);
            drawEye(0.58f);
        glPopMatrix();

        /* boca */
        glColor3f(0.0f, 0.0f, 0.0f);
        glPushMatrix();
            glTranslatef(0.0f, 3.85f, 1.12f);
            drawBox(1.3f, 0.2f, 0.12f);
        glPopMatrix();

    glPopMatrix();
}

/* =====================================================================
   Casa do Patrick: pedra escura com enfeite dourado no topo
   Tecla O = abrir, tecla F = fechar
   ===================================================================== */

/* galho dourado recursivo (enfeite no topo da pedra) */
void drawBranch(float len, int depth, float angleZ)
{
    if (depth == 0 || len < 0.05f) return;
    glColor3f(0.82f, 0.68f, 0.1f);
    glPushMatrix();
        glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
        drawCylinderY(0.08f * len, 0.05f * len, len, 8, 2);
        glTranslatef(0.0f, len, 0.0f);
        drawBranch(len * 0.55f, depth-1,  38.0f);
        drawBranch(len * 0.55f, depth-1, -38.0f);
        drawBranch(len * 0.48f, depth-1,   8.0f);
    glPopMatrix();
}

void drawPatrickHouse(float x, float y, float z)
{
    float R = 7.5f;   /* raio da cúpula */
    float hingeZ = z - R * 0.85f;   /* eixo de dobradiça no fundo da pedra */

    glPushMatrix();
        glTranslatef(x, y, 0.0f);

        /* base de pedra (pequena) */
        glColor3f(0.22f, 0.18f, 0.18f);
        glPushMatrix();
            glTranslatef(0.0f, 0.4f, z);
            glScalef(5.5f, 0.8f, 5.5f);
            glutSolidSphere(1.0f, 16, 8);
        glPopMatrix();

        /* pedra / cupula – rotaciona em torno da dobradica no fundo */
        glTranslatef(0.0f, 0.0f, hingeZ);      /* move hinge para origem */
        glRotatef(-rockAngle, 1.0f, 0.0f, 0.0f); /* abre girando para tras */
        glTranslatef(0.0f, 0.0f, -hingeZ);     /* volta */

        /* interior visivel quando aberto */
        glColor3f(0.18f, 0.08f, 0.08f);
        glPushMatrix();
            glTranslatef(0.0f, 0.2f, z);
            glScalef(1.0f, 0.05f, 1.0f);
            glutSolidSphere(R * 0.92f, 20, 4);
        glPopMatrix();

        /* cúpula exterior escura (marrom muito escuro) */
        glColor3f(0.23f, 0.07f, 0.09f);
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, z);
            glScalef(1.0f, 0.65f, 0.92f);
            /* hemisferio superior usando meia esfera */
            glPushMatrix();
                glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
                /* gluSphere completa mas cortamos a parte de baixo via clip */
                GLdouble eq[4] = {0, -1, 0, 0};
                glClipPlane(GL_CLIP_PLANE0, eq);
                glEnable(GL_CLIP_PLANE0);
                glutSolidSphere(R, 28, 16);
                glDisable(GL_CLIP_PLANE0);
            glPopMatrix();
        glPopMatrix();

        /* enfeite dourado no topo da pedra */
        glPushMatrix();
            glTranslatef(0.0f, R * 0.65f - 0.3f, z);
            /* haste vertical */
            glColor3f(0.82f, 0.68f, 0.1f);
            drawCylinderY(0.1f, 0.08f, 1.8f, 8, 2);
            glTranslatef(0.0f, 1.8f, 0.0f);
            /* barra horizontal */
            glPushMatrix();
                glTranslatef(-1.0f, 0.0f, 0.0f);
                drawCylinderX(0.07f, 0.07f, 2.0f, 8);
            glPopMatrix();
            /* galhos a partir da barra */
            glPushMatrix();
                glTranslatef(-0.8f, 0.0f, 0.0f);
                drawBranch(0.55f, 2, 0.0f);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.8f, 0.0f, 0.0f);
                drawBranch(0.55f, 2, 0.0f);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}

/* =====================================================================
   Cenario
   ===================================================================== */
void drawGround(void)
{
    glColor3f(0.85f, 0.78f, 0.35f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-80.0f, 0.0f, -50.0f);
        glVertex3f( 80.0f, 0.0f, -50.0f);
        glVertex3f( 80.0f, 0.0f,  40.0f);
        glVertex3f(-80.0f, 0.0f,  40.0f);
    glEnd();
    /* algumas pedrinhas no chao */
    glColor3f(0.6f, 0.55f, 0.3f);
    float px[] = {-15.0f, 12.0f, -5.0f, 20.0f, -22.0f};
    float pz[] = {  2.0f,  3.5f, -8.0f,  1.0f,   6.0f};
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
            glTranslatef(px[i], 0.15f, pz[i]);
            glScalef(1.0f, 0.3f, 0.8f);
            glutSolidSphere(0.6f, 8, 6);
        glPopMatrix();
    }
}

/* =====================================================================
   Callbacks
   ===================================================================== */
void myKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case ' ':
            animate = !animate;
            printf("Animacao: %s\n", animate ? "ligada" : "desligada");
            break;
        case '+':
            danceSpeed += 1.0f;
            break;
        case '-':
            if (danceSpeed > 1.0f) danceSpeed -= 1.0f;
            break;
        /* abertura/fechamento da pedra do Patrick */
        case 'o': case 'O':
            rockOpening = 1;
            rockClosing = 0;
            printf("Abrindo a casa do Patrick...\n");
            break;
        case 'f': case 'F':
            rockClosing = 1;
            rockOpening = 0;
            printf("Fechando a casa do Patrick...\n");
            break;
        case 'R': case 'r': glColor3f(1,0,0); break;
        case 'G': case 'g': glColor3f(0,1,0); break;
        case 'B': case 'b': glColor3f(0,0,1); break;
    }
    glutPostRedisplay();
}

void myKeyboardSpecial(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:
            glutFullScreen();
            break;
        case GLUT_KEY_DOWN:
            glutReshapeWindow(800, 600);
            break;
        default:
            printf("Tecla especial: %d\n", key);
            break;
    }
}

void myMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float r = (float)rand()/RAND_MAX;
        float g = (float)rand()/RAND_MAX;
        float b = (float)rand()/RAND_MAX;
        glColor3f(r,g,b);
    }
    glutPostRedisplay();
}

void myTimer(int value)
{
    if (animate) {
        danceAngle += danceSpeed;
        if (danceAngle > 36000.0f) danceAngle -= 36000.0f;
    }
    /* anima abertura/fechamento da pedra */
    if (rockOpening && rockAngle < 85.0f) {
        rockAngle += 2.5f;
        if (rockAngle > 85.0f) rockAngle = 85.0f;
    }
    if (rockClosing && rockAngle > 0.0f) {
        rockAngle -= 2.5f;
        if (rockAngle < 0.0f) rockAngle = 0.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(30, myTimer, 0);
}

void init(void)
{
    glClearColor(0.42f, 0.72f, 0.92f, 0.0f);  /* azul claro (fundo do mar) */
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    GLfloat light_ambient[]  = { 0.35f, 0.35f, 0.35f, 1.0f };
    GLfloat light_diffuse[]  = { 1.0f,  1.0f,  1.0f,  1.0f };
    GLfloat light_specular[] = { 0.6f,  0.6f,  0.6f,  1.0f };
    GLfloat light_position[] = { 15.0f, 30.0f, 30.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    printf("=== Cena Bob Esponja ===\n");
    printf("ESPACO    - liga/desliga danca\n");
    printf("+/-       - velocidade da danca\n");
    printf("O         - abre a casa do Patrick\n");
    printf("F         - fecha a casa do Patrick\n");
    printf("Seta cima - tela cheia\n");
    printf("Seta baixo- janela 800x600\n");
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 12.0f, 38.0f,  0.0f, 5.0f, 0.0f,  0.0f, 1.0f, 0.0f);

    drawGround();
    drawPatrickHouse(0.0f, 3.0f, -14.0f);

    /* personagens dancando "Stayin' Alive" com fase oposta */
    drawBobEsponja(-6.5f, 2.3f, 5.5f,   0.0f);
    drawPatrick(    6.5f, 2.3f, 5.5f, 180.0f);

    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 0.5, 200.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    win_id = glutCreateWindow("Bob Esponja - Cena OpenGL");
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
