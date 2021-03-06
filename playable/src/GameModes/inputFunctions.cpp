#include "global.h"
#include "Glut/myGlut.h"                // Glut functions and definitions
#include "GameModes/inputFunctions.h"
#define MAX_PLAY_BACK 8192

#include "Functional/list.h"
#include "Glut/initialize.h"
#include "Glut/basic.h"
#include "Math/myMath.h"
#include "Functional/music.h"

/* Functions that can be called in any game mode */
void newGameMode(int mode) {
    gameMode = mode;
    switch(gameMode) {
        case simMode:
            endSong("menuSong");
            keyPressed('o', 0, 0);
//            playBackSpeed = 2;
            simTime = 0;
            gen = 0;
            id = 0;
            initiatePopulation(data[0], data[1], data[2], data[3], false);
//          initiatePopulation(5, 5, 5, 5 * HIDDEN_LAYER_SIZE * 2, false);
            break;
        case startMode:
            break;
        default: return;
    }
    return;
}

void setPlayBackSpeed(int number) {
    if (playBackSpeed == number) return;
    if (playBackSpeed < number) { // Speed Up
        for (;playBackSpeed < number; playBackSpeed *= 2) {
            playerSpeed   /= 2;
            cameraSpeed   /= 2;
        }
    } else { // Speed Down
        for (;playBackSpeed > number; playBackSpeed /= 2) {
            playerSpeed   *= 2;
            cameraSpeed   *= 2;
        }
    }
}

void changePlayBack(bool inc) {
    if (inc && (playBackSpeed < MAX_PLAY_BACK)) {
        playBackSpeed *= 2;
        playerSpeed   /= 2;
        cameraSpeed   /= 2;
    }
    else if (!inc && (playBackSpeed > 1)) {
        playBackSpeed /= 2;
        playerSpeed   *= 2;
        cameraSpeed   *= 2;
    }
    return;
}
/* Setting change functions */
void toggleFullScreen() {
    if (fullscreen) glutReshapeWindow(800, 400);
    else glutFullScreen();

    fullscreen ^= true; // toggle
    keyStates['f'] = false; // stop multiple toggle per key press
}

void toggleLights() {
    if (glIsEnabled(GL_LIGHTING)) glDisable(GL_LIGHTING);
    else glEnable(GL_LIGHTING);

    lights ^= true; // toggle
    keyStates['l'] = false; // stop multiple toggle per key press
}

void togglePerson() {
    person ^= true;
    keyStates['p'] = false; // stop multiple toggle per key press
}

void toggleDisplay() {
    display ^= true;
    keyStates['i'] = false;

}

void resetCamera() {
    cameraPos = vec(0.0, -40.0, 1.8);
    cameraDir = vec(0.0,   1.0, 0.0);
    cameraAng = zero();
    return;
}
