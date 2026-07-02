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
float rockAngle   = 0.0f;
int   rockOpening = 0;
int   rockClosing = 0;

#define DEG2RAD(a) ((a) * 3.14159265f / 180.0f)
#define PI 3.14159265f

/* =====================================================================
   Primitivas auxiliares
   ===================================================================== */

void drawCylinderY(float baseR, float topR, float height, int slices, int stacks)
{
    glPushMatrix();
    if (height < 0.0f) { glRotatef(180.0f, 1.0f, 0.0f, 0.0f); height = -height; }
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, baseR, topR, height, slices, stacks);
    glPopMatrix();
}

void drawCylinderX(float baseR, float topR, float length, int sl)
{
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quad, baseR, topR, length, sl, 1);
    glPopMatrix();
}

void drawBox(float sx, float sy, float sz)
{
    glPushMatrix();
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

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
        glRotatef(side * 22.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(side * swing, 1.0f, 0.0f, 0.0f);
        glColor3f(1.0f, 0.88f, 0.1f);
        drawCylinderY(0.22f, 0.18f, -2.2f, 10, 4);
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
        glColor3f(1.0f, 0.88f, 0.1f);
        drawCylinderY(0.28f, 0.26f, -1.8f, 10, 4);
        glTranslatef(0.0f, -1.8f, 0.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCylinderY(0.30f, 0.30f, -0.75f, 10, 4);
        glColor3f(0.72f, 0.72f, 0.72f);
        glPushMatrix();
            glTranslatef(0.0f, -0.25f, 0.0f);
            drawCylinderY(0.31f, 0.31f, -0.08f, 10, 2);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0f, -0.45f, 0.0f);
            drawCylinderY(0.31f, 0.31f, -0.08f, 10, 2);
        glPopMatrix();
        glTranslatef(0.0f, -0.75f, 0.0f);
        glColor3f(0.05f, 0.05f, 0.05f);
        glPushMatrix();
            glTranslatef(0.0f, -0.28f, 0.45f);
            glScalef(1.4f, 0.55f, 2.0f);
            glutSolidSphere(0.65f, 12, 8);
        glPopMatrix();
    glPopMatrix();
}

void drawBobEsponja(float x, float y, float z, float phase)
{
    float bob = 0.35f * sinf(DEG2RAD(2.0f * (danceAngle + phase)));

    glPushMatrix();
        glTranslatef(x, y + bob, z);

        /* corpo amarelo */
        glColor3f(1.0f, 0.88f, 0.1f);
        glPushMatrix();
            glTranslatef(0.0f, 4.5f, 0.0f);
            drawBox(4.0f, 5.0f, 2.5f);
        glPopMatrix();

        /* furos da esponja */
        glColor3f(0.88f, 0.72f, 0.05f);
        float hx[] = {-1.2f, 0.5f, -0.5f, 1.1f, -1.0f, 0.8f};
        float hy[] = {6.8f,  6.3f, 5.2f,  5.6f, 4.2f, 4.8f};
        float hr[] = {0.22f, 0.18f, 0.25f, 0.2f, 0.2f, 0.23f};
        for (int i = 0; i < 6; i++) {
            glPushMatrix();
                glTranslatef(hx[i], hy[i], 1.26f);
                glutSolidSphere(hr[i], 8, 8);
            glPopMatrix();
        }

        /* olhos */
        glPushMatrix();
            glTranslatef(-0.85f, 6.05f, 1.32f);
            drawEye(0.72f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.85f, 6.05f, 1.32f);
            drawEye(0.72f);
        glPopMatrix();

        /* cilios pretos */
        glColor3f(0.0f, 0.0f, 0.0f);
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

        /* nariz */
        glColor3f(0.95f, 0.72f, 0.18f);
        glPushMatrix();
            glTranslatef(0.0f, 5.0f, 1.55f);
            glutSolidSphere(0.38f, 10, 10);
        glPopMatrix();

        /* boca + dentes */
        glColor3f(0.85f, 0.55f, 0.05f);
        glPushMatrix();
            glTranslatef(0.0f, 3.75f, 1.32f);
            drawBox(1.9f, 0.55f, 0.15f);
        glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            glTranslatef(-0.35f, 3.85f, 1.36f);
            drawBox(0.4f, 0.45f, 0.12f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.35f, 3.85f, 1.36f);
            drawBox(0.4f, 0.45f, 0.12f);
        glPopMatrix();

        /* gravata vermelha */
        glColor3f(0.85f, 0.05f, 0.05f);
        glPushMatrix();
            glTranslatef(0.0f, 2.0f, 1.28f);
            glRotatef(5.0f, 0.0f, 0.0f, 1.0f);
            drawBox(0.3f, 1.2f, 0.1f);
            glTranslatef(0.05f, -0.50f, 0.3f);
            glBegin(GL_TRIANGLES);
                glNormal3f(0,0,1);
                glVertex3f(-0.25f, 0.0f, 0.0f);
                glVertex3f( 0.25f, 0.0f, 0.0f);
                glVertex3f( 0.05f,-0.35f, 0.0f);
            glEnd();
        glPopMatrix();

        /* calca marrom */
        glColor3f(0.42f, 0.22f, 0.04f);
        glPushMatrix();
            glTranslatef(0.0f, 1.7f, 0.0f);
            drawBox(4.2f, 1.3f, 2.6f);
        glPopMatrix();
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
            glTranslatef(0.0f, 2.36f, 0.0f);
            drawBox(4.22f, 0.22f, 2.62f);
        glPopMatrix();
        glColor3f(0.7f, 0.65f, 0.1f);
        glPushMatrix();
            glTranslatef(0.0f, 2.36f, 1.32f);
            drawBox(0.55f, 0.3f, 0.12f);
        glPopMatrix();

        drawBobArm(-1.0f, phase);
        drawBobArm( 1.0f, phase + 180.0f);
        drawBobLeg(-1.0f, phase);
        drawBobLeg( 1.0f, phase + 180.0f);

    glPopMatrix();
}

/* =====================================================================
   Patrick Estrela
   Miolo grande e redondo; 4 pontas longas (bracos/pernas) + 1 topo curto.
   As pontas SAO os membros — sem cilindros separados.
   ===================================================================== */
#define STAR_MAX_V 100

/*
 * buildRoundedStarVar: estrela com raio externo diferente por ponta.
 * outerPerTip[5]: raio de cada uma das 5 pontas (t=0 topo, sentido horario).
 * innerR: raio do miolo (quanto maior, mais redondo e gordo o centro).
 * arcR: raio do arco de arredondamento nas pontas.
 */
void buildRoundedStarVar(float outerPerTip[5], float innerR, float arcR, int arcSteps,
                         float *vx, float *vy, int *n)
{
    *n = 0;
    for (int t = 0; t < 5; t++) {
        float tipAng = DEG2RAD(90.0f - t * 72.0f);
        float outerR = outerPerTip[t];
        float tx = outerR * cosf(tipAng);
        float ty = outerR * sinf(tipAng);

        float aL = tipAng + DEG2RAD(36.0f);
        float aR = tipAng - DEG2RAD(36.0f);
        float lx = innerR * cosf(aL), ly = innerR * sinf(aL);
        float rx = innerR * cosf(aR), ry = innerR * sinf(aR);

        float elx = lx-tx, ely = ly-ty, el = sqrtf(elx*elx+ely*ely);
        float erx = rx-tx, ery = ry-ty, er = sqrtf(erx*erx+ery*ery);
        if (el < 0.001f) el = 0.001f;
        if (er < 0.001f) er = 0.001f;
        elx/=el; ely/=el; erx/=er; ery/=er;

        float c = fmaxf(-1.0f, fminf(1.0f, elx*erx + ely*ery));
        float halfTheta = acosf(c) * 0.5f;
        if (halfTheta < 0.05f) halfTheta = 0.05f;

        /* limita arcR para nao ultrapassar a metade da aresta */
        float maxArc = el * sinf(halfTheta) * 0.85f;
        float ar = (arcR < maxArc) ? arcR : maxArc;

        float offset = ar / sinf(halfTheta);
        float cx = tx - offset * cosf(tipAng);
        float cy = ty - offset * sinf(tipAng);

        float arcHalf = (float)PI * 0.5f - halfTheta;
        float aStart  = tipAng + arcHalf;
        float aEnd    = tipAng - arcHalf;

        for (int j = 0; j <= arcSteps; j++) {
            float a = aStart + (aEnd - aStart) * (float)j / arcSteps;
            vx[*n] = cx + ar * cosf(a);
            vy[*n] = cy + ar * sinf(a);
            (*n)++;
        }

        vx[*n] = rx;
        vy[*n] = ry;
        (*n)++;
    }
}

/* wrapper uniforme (usado por nada mais, mas mantido caso precise) */
void buildRoundedStar(float outerR, float innerR, float arcR, int arcSteps,
                      float *vx, float *vy, int *n)
{
    float tips[5] = {outerR, outerR, outerR, outerR, outerR};
    buildRoundedStarVar(tips, innerR, arcR, arcSteps, vx, vy, n);
}

void drawStarPrismFromVerts(float *vx, float *vy, int n, float depth)
{
    float hz = depth * 0.5f;

    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, hz);
        for (int i = 0; i < n; i++) glVertex3f(vx[i], vy[i], hz);
        glVertex3f(vx[0], vy[0], hz);
    glEnd();

    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, -hz);
        for (int i = n-1; i >= 0; i--) glVertex3f(vx[i], vy[i], -hz);
        glVertex3f(vx[n-1], vy[n-1], -hz);
    glEnd();

    glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= n; i++) {
            int k  = i % n;
            int kp = (i + 1) % n;
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

void drawRoundedStarPrism(float outerR, float innerR, float arcR, int arcSteps, float depth)
{
    float vx[STAR_MAX_V], vy[STAR_MAX_V]; int n;
    buildRoundedStar(outerR, innerR, arcR, arcSteps, vx, vy, &n);
    drawStarPrismFromVerts(vx, vy, n, depth);
}

void drawPatrick(float x, float y, float z, float phase)
{
    float bob  = 0.35f * sinf(DEG2RAD(2.0f * (danceAngle + phase)));
    float sway = 22.0f * sinf(DEG2RAD(danceAngle + phase));       /* balanco lateral */
    float lean = 10.0f * sinf(DEG2RAD(danceAngle + phase + 90.f)); /* inclinacao frente/tras */

    /*
     * Pontas da estrela como membros (sentido horario a partir do topo):
     *   t=0  90deg  topo (cabeca/gorducho) — curto
     *   t=1  18deg  braco direito          — longo
     *   t=2 -54deg  perna direita          — longo
     *   t=3 -126deg perna esquerda         — longo
     *   t=4  162deg braco esquerdo         — longo
     * innerR grande deixa o miolo redondo e gordo.
     */
    float outerPerTip[5] = {3.2f, 5.2f, 4.9f, 4.9f, 5.2f};
    float innerR = 2.8f;
    float arcR   = 0.80f;
    int   arcSteps = 7;
    float depth  = 2.4f;

    float vx[STAR_MAX_V], vy[STAR_MAX_V]; int n;
    buildRoundedStarVar(outerPerTip, innerR, arcR, arcSteps, vx, vy, &n);

    glPushMatrix();
        glTranslatef(x, y + bob, z);
        glRotatef(sway, 0.0f, 0.0f, 1.0f);   /* balanca como "Stayin' Alive" */
        glRotatef(lean, 1.0f, 0.0f, 0.0f);

        /* corpo estrela */
        glColor3f(1.0f, 0.55f, 0.60f);
        glPushMatrix();
            glTranslatef(0.0f, 3.8f, 0.0f);
            drawStarPrismFromVerts(vx, vy, n, depth);
        glPopMatrix();

        /* calcinha verde com listras roxas (na barriga do corpo) */
        float shortZ = depth * 0.5f + 0.05f;  /* frente do prisma */
        glColor3f(0.15f, 0.55f, 0.2f);
        glPushMatrix();
            glTranslatef(0.0f, 3.6f, shortZ);
            drawBox(2.5f, 1.1f, 0.12f);
        glPopMatrix();
        glColor3f(0.55f, 0.1f, 0.7f);
        for (int i = -1; i <= 1; i++) {
            glPushMatrix();
                glTranslatef(i * 0.75f, 3.6f, shortZ + 0.05f);
                drawBox(0.18f, 1.15f, 0.08f);
            glPopMatrix();
        }

        /* olhos na face frontal do miolo */
        float eyeZ = shortZ + 0.02f;
        glPushMatrix();
            glTranslatef(-0.78f, 4.55f, eyeZ);
            drawEye(0.58f);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.78f, 4.55f, eyeZ);
            drawEye(0.58f);
        glPopMatrix();

        /* boca */
        glColor3f(0.0f, 0.0f, 0.0f);
        glPushMatrix();
            glTranslatef(0.0f, 3.85f, eyeZ + 0.55f);
            drawBox(1.3f, 0.2f, 0.12f);
        glPopMatrix();

    glPopMatrix();
}

/* =====================================================================
   Casa do Patrick: pedra cúpula escura + enfeite dourado
   Tecla O = abrir, tecla F = fechar
   ===================================================================== */

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
    float R = 7.5f;
    float hingeZ = z - R * 0.85f;

    glPushMatrix();
        glTranslatef(x, y, 0.0f);

        /* base de pedra */
        glColor3f(0.22f, 0.18f, 0.18f);
        glPushMatrix();
            glTranslatef(0.0f, 0.4f, z);
            glScalef(5.5f, 0.8f, 5.5f);
            glutSolidSphere(1.0f, 16, 8);
        glPopMatrix();

        /* cupula: gira em torno da dobradica */
        glTranslatef(0.0f, 0.0f, hingeZ);
        glRotatef(-rockAngle, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -hingeZ);

        /* interior visivel quando aberto */
        glColor3f(0.18f, 0.08f, 0.08f);
        glPushMatrix();
            glTranslatef(0.0f, 0.2f, z);
            glScalef(1.0f, 0.05f, 1.0f);
            glutSolidSphere(R * 0.92f, 20, 4);
        glPopMatrix();

        /* cupula escura */
        glColor3f(0.23f, 0.07f, 0.09f);
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, z);
            glScalef(1.0f, 0.65f, 0.92f);
            glPushMatrix();
                glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
                GLdouble eq[4] = {0, -1, 0, 0};
                glClipPlane(GL_CLIP_PLANE0, eq);
                glEnable(GL_CLIP_PLANE0);
                glutSolidSphere(R, 28, 16);
                glDisable(GL_CLIP_PLANE0);
            glPopMatrix();
        glPopMatrix();

        /* enfeite dourado */
        glPushMatrix();
            glTranslatef(0.0f, R * 0.65f - 0.3f, z);
            glColor3f(0.82f, 0.68f, 0.1f);
            drawCylinderY(0.1f, 0.08f, 1.8f, 8, 2);
            glTranslatef(0.0f, 1.8f, 0.0f);
            glPushMatrix();
                glTranslatef(-1.0f, 0.0f, 0.0f);
                drawCylinderX(0.07f, 0.07f, 2.0f, 8);
            glPopMatrix();
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

/* Fundo degradê: azul claro (#5AAAFA) embaixo -> azul escuro (#166ABE) em cima.
   Desenhado como quad ortografico ANTES de qualquer objeto 3D. */
void drawBackground(void)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glBegin(GL_QUADS);
                /* topo: #166ABE */
                glColor3f(0.086f, 0.416f, 0.745f);
                glVertex2f(-1.0f,  1.0f);
                glVertex2f( 1.0f,  1.0f);
                /* base: #5AAAFA */
                glColor3f(0.353f, 0.667f, 0.980f);
                glVertex2f( 1.0f, -1.0f);
                glVertex2f(-1.0f, -1.0f);
            glEnd();
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

/* Chao arenoso com textura procedural (manchas, listras de ondas, pedrinhas). */
void drawGround(void)
{
    /* base da areia */
    glColor3f(0.84f, 0.76f, 0.34f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-80.0f, 0.0f, -50.0f);
        glVertex3f( 80.0f, 0.0f, -50.0f);
        glVertex3f( 80.0f, 0.0f,  40.0f);
        glVertex3f(-80.0f, 0.0f,  40.0f);
    glEnd();

    /* listras de ondas de areia (faixas horizontais ligeiramente mais escuras) */
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 18; i++) {
        float zLine = -45.0f + i * 5.0f;
        float shade = (i % 2 == 0) ? 0.78f : 0.72f;
        glColor3f(shade, shade * 0.91f, shade * 0.38f);
        glBegin(GL_QUADS);
            glVertex3f(-80.0f, 0.01f, zLine);
            glVertex3f( 80.0f, 0.01f, zLine);
            glVertex3f( 80.0f, 0.01f, zLine + 1.5f);
            glVertex3f(-80.0f, 0.01f, zLine + 1.5f);
        glEnd();
    }

    /* manchas de granulos de areia (pequenas elipsoides achatadas) */
    glNormal3f(0.0f, 1.0f, 0.0f);
    static const float gx[] = {
        -18,-10, -3,  5, 13, 20,-25, -8, 2, 18,
         -5, 15,-20,  9,-14, 22,-30, 7, -1, 25,
          3,-22, 12, -7, 17, -4, 11,-16,  0, 28};
    static const float gz[] = {
         3,  8, -2, 12,  1, -9,  6, -5,15, -7,
        -12, 4, 10,  0, -3, 16,  2,-10, 7,-14,
         13,-6,  5, -8, 11,  3,-11, 18,-4,  0};
    static const float gc[] = {
        0.90f,0.82f,0.86f,0.79f,0.88f,0.83f,0.77f,0.91f,0.80f,0.85f,
        0.74f,0.89f,0.83f,0.78f,0.87f,0.81f,0.76f,0.84f,0.90f,0.73f,
        0.88f,0.80f,0.85f,0.77f,0.82f,0.92f,0.79f,0.86f,0.83f,0.75f};
    for (int i = 0; i < 30; i++) {
        float c = gc[i];
        glColor3f(c, c * 0.90f, c * 0.38f);
        glPushMatrix();
            glTranslatef(gx[i], 0.02f, gz[i]);
            glScalef(1.8f + (i%3)*0.5f, 0.06f, 1.2f + (i%4)*0.3f);
            glutSolidSphere(0.8f, 8, 4);
        glPopMatrix();
    }

    /* pedrinhas maiores */
    static const float px[] = {-15,-10, -5, 3,  12, 20,-22,  8,-18, 16, -2, 24};
    static const float pz[] = {  2,  9, -7, 14,  4,-11,  7,-14,  1, -5, 11, -3};
    static const float ps[] = {0.7f,0.5f,0.9f,0.6f,0.8f,0.55f,0.75f,0.65f,0.85f,0.5f,0.7f,0.6f};
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 12; i++) {
        float g = 0.42f + (i % 5) * 0.04f;
        glColor3f(g, g, g + 0.05f);
        glPushMatrix();
            glTranslatef(px[i], ps[i]*0.25f, pz[i]);
            glScalef(ps[i]*1.3f, ps[i]*0.38f, ps[i]);
            glutSolidSphere(0.7f, 9, 6);
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
        case '+': danceSpeed += 1.0f; break;
        case '-': if (danceSpeed > 1.0f) danceSpeed -= 1.0f; break;
        case 'o': case 'O':
            rockOpening = 1; rockClosing = 0;
            printf("Abrindo a casa do Patrick...\n"); break;
        case 'f': case 'F':
            rockClosing = 1; rockOpening = 0;
            printf("Fechando a casa do Patrick...\n"); break;
        case 'R': case 'r': glColor3f(1,0,0); break;
        case 'G': case 'g': glColor3f(0,1,0); break;
        case 'B': case 'b': glColor3f(0,0,1); break;
    }
    glutPostRedisplay();
}

void myKeyboardSpecial(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:   glutFullScreen(); break;
        case GLUT_KEY_DOWN: glutReshapeWindow(800, 600); break;
        default: printf("Tecla especial: %d\n", key); break;
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
    glClearColor(0.086f, 0.416f, 0.745f, 0.0f); /* fallback: azul escuro #166ABE */
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
    printf("ESPACO     - liga/desliga danca\n");
    printf("+/-        - velocidade da danca\n");
    printf("O          - abre a casa do Patrick\n");
    printf("F          - fecha a casa do Patrick\n");
    printf("Seta cima  - tela cheia\n");
    printf("Seta baixo - janela 800x600\n");
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* 1) degradê de fundo (sem perspectiva, sem depth) */
    drawBackground();

    /* 2) cena 3D */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 12.0f, 38.0f,  0.0f, 5.0f, 0.0f,  0.0f, 1.0f, 0.0f);

    drawGround();
    drawPatrickHouse(0.0f, 3.0f, -14.0f);
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
