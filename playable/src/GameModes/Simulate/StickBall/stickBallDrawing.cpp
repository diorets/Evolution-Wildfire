#include "GameModes/Simulate/StickBall/stickBallDrawing.h"

#include "Glut/myGlut.h"           // Glut
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include "Math/myMath.h"
#include "ErrorHandle/error.h"
#include "global.h"
#include <math.h>
#include <stdio.h>
#include "GameModes/Simulate/General/drawing.h"
#include "GameModes/Simulate/Stickball/stickBallPhysics.h"
#include "Functional/buttons.h"
#include "Functional/graph.h"

static void createMenu();
void displaySystemInfo(creature * pop, int genSize, int id);




void fline(double x, double y, double X, double Y) {
    drawLine(x, y, ground(vec(x, y, 0)), X, Y, ground(vec(X, Y, 0)));
    return;
}



bool inSquare(posi p, double x, double y, double X, double Y) {
    if (p.x >= x)
    if (p.x <= X)
    if (p.y >= y)
    if (p.y <= Y)
    return true;
    return false;
}

//#include <pthread.h>
//typedef struct thread_datum {
//   int thread_id;
//   posi c;
//   double range;
//   double h;
//} thread_datum;
//
//
//void *BusyWork(void *threadarg) {
//    thread_datum *my_data = (thread_datum *) threadarg;
//    int taskid = my_data->thread_id;
//    double h = my_data->h;
//    posi c = my_data->c;
//    double range = my_data->range;
////    printf("%f, %f, %f\n", c.x - range, c.x + range, h);
//    glBegin(GL_QUADS);
//    for (double x = c.x - range; x <= c.x + range; x+=h) {
//        for (double y = c.y - range; y <= c.y + range; y+=h) {
//            posi N = normal(ground, vec(x+h, y+h, 0));
//            glNormal3f(ARG(N));
//            glVertex3f(x, y, ground(vec(x, y, 0)));
//
////                N = normal(ground, vec(x+h, y, 0));
////                glNormal3f(ARG(N));
//            glVertex3f(x+h, y, ground(vec(x+h, y, 0)));
////
////                N = normal(ground, vec(x+h, y+h, 0));
////                glNormal3f(ARG(N));
//            glVertex3f(x+h, y+h, ground(vec(x+h, y+h, 0)));
//
////                N = normal(ground, vec(x, y+h, 0));
////                glNormal3f(ARG(N));
//            glVertex3f(x, y+h, ground(vec(x, y+h, 0)));
//        }
//    }
//    glEnd();
//   pthread_exit(NULL);
//}

void drawTiles(posi c, double range, double h) {
    /* Tiles */
    glColor3f(GREEN);
    glBegin(GL_QUADS);
        GLfloat zero[] = {0,0,0,0};
//        GLfloat A[] = {1,1,1,1};
        glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
        glMaterialfv(GL_FRONT, GL_AMBIENT, zero);

        for (double x = c.x - range; x <= c.x + range; x+=h) {
            for (double y = c.y - range; y <= c.y + range; y+=h) {
                    posi N;

                N = normal(ground, vec(x+h, y+h, 0));
                N = scale(N, 1);
                glNormal3f(ARG(N));
                glVertex3f(x, y, ground(vec(x, y, 0)));

                    N = normal(ground, vec(x+h, y, 0));
                N = scale(N, 1);
                    glNormal3f(ARG(N));
                glVertex3f(x+h, y, ground(vec(x+h, y, 0)));
    //
                    N = normal(ground, vec(x+h, y+h, 0));
                N = scale(N, 1);
                    glNormal3f(ARG(N));
                glVertex3f(x+h, y+h, ground(vec(x+h, y+h, 0)));

                    N = normal(ground, vec(x, y+h, 0));
                N = scale(N, 1);
                    glNormal3f(ARG(N));
                glVertex3f(x, y+h, ground(vec(x, y+h, 0)));
            }
        }
//        #define NUM_THREADS 8
//        pthread_t threads[NUM_THREADS];
//        long taskids[NUM_THREADS];
//        pthread_attr_t attr;
//        void *status;
//
//        /* Initialize and set thread detached attribute */
//        pthread_attr_init(&attr);
//        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
//
//        thread_datum * datum = (thread_datum*) malloc(sizeof(thread_datum));
//        datum->c = c;
//        datum[0].h = h;
//        datum->range = range;
//        for(int t=0;t<NUM_THREADS;t++) {
//            taskids[t] = t;
////            printf("Creating thread %d\n", t);
//            (*datum).thread_id = t;
//            int rc = pthread_create(&threads[t], NULL, BusyWork, (void *) datum);
//            if (rc) {
//                printf("ERROR; return code from pthread_create() is %d\n", rc);
//                exit(-1);
//            }
//        }
//
//        /* Free attribute and wait for the other threads */
//        pthread_attr_destroy(&attr);
//        for(int t=0; t<NUM_THREADS; t++) {
//            int rc = pthread_join(threads[t], &status);
//            if (rc) {
//                printf("ERROR; return code from pthread_join() is %d\n", rc);
//                exit(-1);
//            }
////            printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
//        }
    glEnd();
    return;
}

posi * getNodeShadows(int numNodes, creature s) {
    /* Node Shadows */
    posi * nodeShadows = (posi*) malloc(sizeof(posi) * numNodes);
    GLfloat zero[] = {0,0,0,0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
    glColor3f(DARKGREEN);
    for (int i = 0; i < numNodes; i++) {
        posi m = vec(SUN); //sun (moon)->m
        posi l_0 = ((stickball*) (&s)->components)->nodes[i].loc;
        l_0.z -= RADIUS; // correction to make shadow come from base of node.
        posi l = sub(m, l_0);

        posi n = vec(0,0,-1); // defaul to approximation
        posi p = add(m, scale(l, -dot(m, n) / dot(l, n))) ; // initial guess

//        posi p0 = add(scale(l, -0.2), l_0);
//        posi pn = add(scale(l, 1.5), l_0);
//            drawCylinder(p0.x, p0.y, p0.z, pn.x, pn.y, pn.z, 0.05, 3);


        double searchBound = -0.2;
        for (double t = 0; t >= searchBound; t -= 0.0001) {
            posi p1 = add(scale(l, t), l_0);

            if (p1.z < ground(p1)) { // change of sign means found zero / interesection.
                n = normal(ground, p1);
                p = p1;
                break;
            }
        }
        nodeShadows[i] = p;
    }
    return nodeShadows;
}

void drawGridLines(posi c, double range, double h, double hover) {
    /* Grid Lines */
    glLineWidth(0.5);
    glColor3f(BLACK);
    glBegin(GL_LINES);
        for (double x = c.x - range; x <= c.x + range; x+=h) {
            for (double y = c.y- range; y <= c.y + range; y+=h) {
                double z_x_y   = 1.2*hover+ground(vec(x  , y, 0));
                double z_xh_y  = 1.2*hover+ground(vec(x+h, y, 0));
                double z_xh_yh = 1.2*hover+ground(vec(x+h, y+h, 0));
//                double z_x_yh  = 1.2*hover+ground(vec(x  , y+h, 0));

                glVertex3f(x  , y  , z_x_y);
                glVertex3f(x+h, y  , z_xh_y);

                glVertex3f(x+h, y  , z_xh_y);
                glVertex3f(x+h, y+h, z_xh_yh);
            }
        }
    glEnd();
    return;
}



static void twoPointShadow(posi p1, posi p2, double wc, double ws, double hover) {
//    posi N = normal(ground, vec(p1.x-wc, p1.y+ws, 0)); // shadowing
//                    glNormal3f(ARG(N));

//    glNormal3f(ARG(N));

    glVertex3f(p1.x-wc, p1.y+ws, hover + ground(vec(p1.x-wc, p1.y+ws, 0)));
    glVertex3f(p2.x-wc, p2.y+ws, hover + ground(vec(p2.x-wc, p2.y+ws, 0)));
    glVertex3f(p2.x+wc, p2.y-ws, hover + ground(vec(p2.x+wc, p2.y-ws, 0)));
    glVertex3f(p1.x+wc, p1.y-ws, hover + ground(vec(p1.x+wc, p1.y-ws, 0)));
    return;
}

posi getPointOnLine(posi p1, posi p2, double t) { // from p1 to p2
    posi dir = sub(p2, p1);
    posi p = add(scale(dir, t), p1);
//    drawSphere(p.x, p.y, p.z, 0.1);
    return p;
}

posi findIntersection(posi p1, posi p2) {
//    drawCylinder(p1.x, p1.y, p1.z,
//                 p2.x, p2.y, p2.z,
//                 0.1, 10);
    posi dir = sub(p2, p1);
    for (double t = 1; t < 1.5; t += 0.0003) {
        posi trialP = add(scale(dir, t), p1);
        if (trialP.z <= ground(trialP)) {
            return trialP;
        }
    }
//    puts("Error, couldn't find shadow intersection.");
    return vec(0,0,0);
}


void drawTwoNodeShadow(posi a, posi b, double w, double hover) {

    posi AB_t = getPointOnLine(a, b, 0);
    AB_t.z -= RADIUS;
    posi p1 = findIntersection(vec(SUN), AB_t);
    double step = 0.3333;
    for (double t = 0; t < 1-step; t += step) {
        posi AB_tdt = getPointOnLine(a, b, t+step);
        AB_tdt.z -= RADIUS;
        posi p2 = findIntersection(vec(SUN), AB_tdt);

        double angle = atan2(p1.y - p2.y, p1.x - p2.x);

        double ws = w * cos(angle);
        double wc = w * sin(angle);

        twoPointShadow(p1, p2, wc, ws, hover);

        AB_t = AB_tdt;
        p1 = p2;
    }
    return;
}

void drawConnectionShadows(creature s, double w, double hover) {
    int numMuscle = s.genome->iData[mus];
    int numBone = s.genome->iData[bon];

    glColor3f(DARKGREEN);
    glBegin(GL_QUADS);
    node * nodes = ((stickball*) (&s)->components)->nodes;

    /* Muscle Shadows */
    for (int i = 0; i < numMuscle; i++) {
        muscle mus = ((stickball*) (&s)->components)->muscles[i];
        drawTwoNodeShadow(nodes[mus.a].loc, nodes[mus.b].loc, w, hover);
    }
    /* Bone Shadows */
    for (int i = 0; i < numBone; i++) {
        bone bons = ((stickball*) (&s)->components)->bones[i];
        drawTwoNodeShadow(nodes[bons.a].loc, nodes[bons.b].loc, w, hover);
    }
    glEnd();
    return;
}

void drawFunction(creature s) {
//    GLfloat light_position[] = {100.0, 100.0, 100.0, 0.0};
    GLfloat light_position[] = {1000.0,-1000.0,1000.0, 5.0};
//    GLfloat A[] = {.5,.5,.5, 0.0};
    GLfloat A[] = {1,1,1, 0.5};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, A);


    bool shading = true;
    double h = 1; // 1.5
    double range = 60; // 50
    double w = 0.15;
    double hover = w / 10;

    posi c = getCom(s);
    c = vec(round(c.x / h) * h, floor(c.y / h) * h, floor(c.z / h) * h); // round c to h so grid moves in steps

    drawTiles(c, range, h);
    if (shading) drawConnectionShadows(s, w, hover);
    drawGridLines(c, range, h, hover);
    return;
}

bool invalidSphere(posi * points, posi trial, int numP, double radius) {
    for (int i = 0; i < numP; i++) {
        if (euc(points[i], trial) < 2 * radius) {
            return true;
        }
    }
    return false;
}

void stickBallSystem(creature * pop, int genSize, int gen, int id, int simTime, int maxTime) {
    creature input = pop[id];

    /* Clear, Reset, Camera */
    glutDrawing();

    /* 3D Drawing */
    reenable3D();

//    drawGrass(400, 0);
//    glLineWidth(50);
//    drawGrid(400, 0, 10);
//    drawGround(800, CAGESIZE, CAGESIZE);
//    drawCylinder(-20, 0, input.fitness,
//             20, 0, input.fitness,
//             0.25, 10);
    drawAxes(10, 10, 10);
    drawFunction(input);
    drawSun();
    drawStickBall(input);

    glColor3f(BLACK);
    for (int i = 0; i < simTime-1; i++) {
        glLineWidth(5);
        posi p1 = ((stickball*) input.components)->trail[i];
        posi p2 = ((stickball*) input.components)->trail[i+1];
        drawLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
    }
    glColor3f(RED);

    posi prev = vec(0.5,0,6);
    for (int t = 1; t < simTime-1; t++) {
        double s = 1 / 100.0;
//        double val = 0.5 * cos(t*s) + cos(t*s * 3) + 1.5 + 0.5*s*t;
        double val = s*t;//2*sqrt(s*t);
        posi path = vec(s*t, 0,  2 * sqrt(s*t));
        path = scale(path, 3);

        drawLine(prev.x, prev.y, prev.z, path.x, path.y, path.z);
        prev = path;
    }

    /* 2D drawing */
    enable2D();

    glLineWidth(2);
    bool twoG = globalData[creatureFitnessE].g.display && globalData[generationFitnessE].g.display;
    drawGraph(globalData[creatureFitnessE].g, 0, 0.5 * wx * (1.0 + !twoG));
    drawGraph(globalData[generationFitnessE].g, 0.5 * wx * twoG, wx);
    glColor3f(BLACK);
    drawLine(wx/2.0, (wy-45)/2.0+5, wx/2.0, (wy-45)/2.0-5);
    drawLine(wx/2.0+5, (wy-45)/2.0, wx/2.0-5, (wy-45)/2.0);
    createMenu();

    for (button * b = buttons; b != NULL; b = b->next) {
        if (b->toggled && !strcmp("System Info", b->label)) {
            displaySystemInfo(pop, genSize, id);
        }
    }
    drawDetails(genSize, gen, id, simTime, maxTime);


    glutSwapBuffers(); // Make it all visible
}

static void assignCords(button * b) {
    switch (b->group) {
        case 0:
            {
            int numButtons = getNumButtons(buttons, b->group);
            double width = (wx - 5) / (double) numButtons;
            b->x = b->id * width + 5;
            b->y = wy - 45;
            b->w = (b->id+1) * width;
            b->h = wy - 5;
            }
            break;
        case 1:
            b->x = wx * 0.90;
            b->y = 45 * (b->id);
            b->w = wx;
            b->h = 45 * (b->id + 1);
        break;
        default: return;
    }
    return;
}

void displaySystemInfo(creature * pop, int genSize, int id) {
    int yTop = 5 * 20;
    int yBottom = 5 * 20 + 20 * 14 + 20; // 50 % of screen
    drawSquare(0, yBottom, 220, yTop);
    glColor3f(WHITE);
    int height = yTop;
    char str[100];
    sprintf(str, "Creature Genes"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Total: %d",   pop[id].genome->iData[tot]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Nodes: %d",   pop[id].genome->iData[nod]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Bones: %d",   pop[id].genome->iData[bon]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Muscles: %d", pop[id].genome->iData[mus]); drawText(str, 20, height += 20, false, false);
    height+=20;
    double popStats[] = {0, 0, 0, 0, 0};
    for (int i = 0; i < genSize; i++) {
        popStats[0] += pop[i].genome->iData[tot];
        popStats[1] += pop[i].genome->iData[nod];
        popStats[2] += pop[i].genome->iData[bon];
        popStats[3] += pop[i].genome->iData[mus];
        popStats[4] += pop[i].fitness; // Only works because fitness for pop[i > id] = 0
    }
    double oneOverGenSize = 1.0 / (double) genSize;
    popStats[0] *= oneOverGenSize;
    popStats[1] *= oneOverGenSize;
    popStats[2] *= oneOverGenSize;
    popStats[3] *= oneOverGenSize;
    popStats[4] *= id == 0 ? 0 : 1.0 / (double) id;

    sprintf(str, "Population Genes"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Total: %.2f",   popStats[0]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Nodes: %.2f",   popStats[1]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Bones: %.2f",   popStats[2]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Muscles: %.2f", popStats[3]); drawText(str, 20, height += 20, false, false);
    height+=20;

    sprintf(str, "Population Fitness"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Current Average: %.2f",   popStats[4]); drawText(str, 20, height += 20, false, false);


}

#include "GameModes/inputFunctions.h"
void createMenu() {
    /* Create Buttons */
    if (buttons == NULL) {
        int i = 0;
        int j = 0;
        buttons = createButton("Fitness Graph"   , toggleCreatureGraph  , assignCords, j++    , 0, true , false);
        buttons = createButton("Generation Graph", toggleGenerationGraph, assignCords, j++    , 0, true , false);
        buttons = createButton("Exit"            , normalExit           , assignCords, i++    , 1, false, false);
        buttons = createButton("Back to Menu"    , nullFunc             , assignCords, i++    , 1, false, false);
        buttons = createButton("Full Screen"     , toggleFullScreen     , assignCords, i++    , 1, true , true );
        buttons = createButton("System Info"     , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Mutations"       , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Selection"       , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Save Genome"     , nullFunc             , assignCords, (++i)++, 1, false , false);
    }

    /* Draw Background */
    glColor3f(BLACK);
    drawSquare(0, wy, wx, wy-50);

    /* Highlighting */
    for (button * b = buttons; b != NULL; b = b->next) {
        b->highlighted = hoveringOver(b);
    }

    drawButtons();
    return;
}

void drawStickBall(creature individual) {
    int * sizes = individual.genome->iData;
    stickball * components = ((stickball*) individual.components);
    posi com = getCom(individual);

    /* Nodes */
    glColor3f(BEIGE);
    for (int i = 0; i < sizes[nod]; i++) {
        posi loc = components->nodes[i].loc;
        drawSphere(loc.x, loc.y, loc.z, RADIUS);
    }

    double thickness = RADIUS * 0.5;
    glColor3f(WHITE);
    for (int i = 0; i < sizes[bon]; i++) {
        int a = components->bones[i].a;
        int b = components->bones[i].b;
        if (a == b) quit(GENOME_ERROR); // Do I need this check?
        //printf("%d, %d\n", a, b);
        posi locA = components->nodes[a].loc;
        posi locB = components->nodes[b].loc;
        drawCylinder(locA.x, locA.y, locA.z,
                      locB.x, locB.y - 0.1, locB.z,
                        thickness ,     5);
    }

    /* Muscle */
    glColor3f(RED);
    for (int i = 0; i < sizes[mus]; i++) {
        int a = components->muscles[i].a;
        int b = components->muscles[i].b;
        if (a == b) quit(GENOME_ERROR); // Do I need this check?

        posi locA = components->nodes[a].loc;
        posi locB = components->nodes[b].loc;
        double minV = -40 / (i-10) + components->muscles[i].origLength;
        double maxV = +40 / (i-10) + components->muscles[i].origLength;
        double percent = (euc(locA, locB) - minV) / (maxV - minV);
        percent = 1;
        drawCylinder(locA.x, locA.y, locA.z,
                      locB.x, locB.y + 0.1, locB.z,
                       0.5*RADIUS * percent,     5);
    }

    /* COMs / Origins */
    glColor3f(WHITE);
    drawSphere(com.x, com.y, com.z, 0.1); // COM
    drawSphere(com.x, com.y,     0, 0.1); // Ground COM
    drawSphere(components->origin.x, components->origin.y, components->origin.z, 0.1); // Origin
    glColor3f(RED);
    drawLine(com.x, com.y, 0.1, // Distance
             components->origin.x, components->origin.y, 0.1);
    return;
}









