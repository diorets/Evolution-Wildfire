bool twoD = true; /// Change in both files
#include "GameModes/Simulate/StickBall/stickBallMutations.h"

#include <math.h> // Fabs
#include "Math/myMath.h"       // Chance, vec, euc
#include "GameModes/Simulate/StickBall/stickBallGenes.h"  // Various Genes
#include "Functional/list.h"   // Various linked list functions
#include "ErrorHandle/error.h" // Quit

void modifyWeights(gene * head, double modifyChance) {
    FOR_ALL(head, 'a') {
        if (chance(modifyChance)) {
            current->fData[weightE] += pmRandf(0.05);
        }
    }
    return;
}
void removeNodeByIndex(gene * head, int toRemove);
void removeNodes(gene * head, double removeChance) {
    if (!chance(removeChance)) return;
    /* Pick a Random Node */
    int counter = 0;
    FOR_ALL(head, 'n') {
        counter++;
    }
    if (counter < 2) return;
    int toRemove = rand() % counter;

    removeNodeByIndex(head, toRemove);
    return;
}

void moveNodes(gene * head, double moveChance) {
    while (chance(moveChance)) {
        removeNodes(head, 100);

        /* Add a New One */
        head = addToBack(head, nodeGene(head));
        head->iData[tot]++;
        head->iData[nod]++;
    }
    return;
}

void addNodes(gene * head, double addChance) {
    if (chance(addChance)) {
        head = addToBack(head, nodeGene(head));
        head->iData[tot]++;
        head->iData[nod]++;
    }
}

#include "global.h"
#include <stdio.h>

void alterMuscles(gene * head, double bigChange, double smallChange) {
    FOR_ALL(head, 'm') {
        if (chance(bigChange)) {
            current->fData[0] = rateFunction();
            current->fData[1] = shiftFunction();
        }
        if (chance(smallChange)) {
            double change0 = pmRandf(0.2);
            double change1 = pmRandf(0.5);

            /* "If adding the change brings it out of bounds, go the other way" */
            current->fData[0] += fabs(change0 + current->fData[0]) > (0.5*3.1415926) ? -change0 : change0;

            if (change1 + current->fData[1] < 0) current->fData[1] += -change1;
            else if (change1 + current->fData[1] > 5) current->fData[1] += -change1;
        }
    }
    return;
}

void mutateStickball(creature * toMutate) { // Move node, addconnection, remove connection
    moveNodes(toMutate->genome, 30);
    addNodes(toMutate->genome, 50);
    removeNodes(toMutate->genome, 8);
    alterMuscles(toMutate->genome, 0, 100);

    removeStrandedNodes(toMutate->genome);
    /* Varification */
    verifyGenome    (toMutate->genome);

    return;

    /* Nodes Mods */
    alterMuscles(toMutate->genome, 3, 10);
//
    relocateNodes(toMutate->genome, toMutate->genome->fData[0]); // These assume a valid location exists
    shiftNodes   (toMutate->genome, toMutate->genome->fData[1], 0.5);
    addNode      (toMutate->genome, toMutate->genome->fData[2], 15,  15);
    removeNode   (toMutate->genome, toMutate->genome->fData[3]);
//    changeStats     (toMutate->genome, chance); // Least important

    /* Connections */
//    changeConnection(toMutate->genome, chance); // goto diferent muscles
    addConnection   (toMutate->genome, toMutate->genome->fData[4], 50, 100);
    removeConnection(toMutate->genome, toMutate->genome->fData[5]);
//    swapConnection  (toMutate->genome, toMutate->genome->fData[6]);

    /* Neural Network */
    // modifyWeights(toMutate->genome, toMutate->genome->fData[6]);

    removeStrandedNodes(toMutate->genome);
    /* Varification */
    verifyGenome    (toMutate->genome);
    return;
}

void shiftNodes(gene * head, double shiftChance, double amount) {
    FOR_ALL(head, 'n') if (chance(shiftChance)) {
        double x = current->fData[xposi];
        double y = current->fData[yposi];
        double z = current->fData[zposi];
        posi newPos = getValidShift(head, x, y, z, amount);
        current->fData[xposi] = newPos.x;
        current->fData[yposi] = twoD ? 0 : newPos.y;
        current->fData[zposi] = newPos.z;
    }
    return;
}

void relocateNodes(gene * head, double shiftChance) {
    FOR_ALL(head, 'n') if (chance(shiftChance)) {
        posi newPos = getValidPosition(head);
        current->fData[xposi] = newPos.x;
        current->fData[yposi] = newPos.y;
        current->fData[zposi] = newPos.z;
    }
    return;
}

void addNode(gene * head, double nodeChance, double muscleChance, double boneChance) {
    if (head->iData[nod] >= MAX_ELEMENTS) return;
    if (chance(nodeChance)) {
        /* Add Node */
        addToBack(head, nodeGene(head));
        head->iData[tot]++;
        head->iData[nod]++;

        /* Add (Potentially) Multiple Connections */
        int counter = 0;
        FOR_ALL(head, 'n') {
            if (counter == head->iData[nod] - 1) continue; // Counter is at new node
            // Function can be improved to make it more evenly random
            if (chance(muscleChance)) {
                if (head->iData[mus] >= MAX_ELEMENTS) continue;
                addToBack(head, muscleGene(counter, head->iData[nod] - 1));
                head->iData[tot]++;
                head->iData[mus]++;
            }
            else if (chance(boneChance)) {
                if (head->iData[bon] >= MAX_ELEMENTS) continue;
                addToBack(head, boneGene(counter, head->iData[nod] - 1));
                head->iData[tot]++;
                head->iData[bon]++;
            }
            counter++;
        }
    }
    return;
}

void removeNode(gene * head, double removeChance) {
    if (head->iData[nod] <= 1) return;

    if (chance(removeChance)) {
        int toRemove = rand() % head->iData[nod];
        removeNode(head, toRemove);
    }
    return;
}

void removeNodeByIndex(gene * head, int toRemove) {
    if (head->iData[nod] <= 1) return;
    if (toRemove < 0) return;

    /* Remove Node */
    int nodeNum = 0;
    FOR_ALL(head, 'n') {
        if (nodeNum == toRemove) {
            removeItem(head, current);
            head->iData[tot]--;
            head->iData[nod]--;
            break;
        }
        nodeNum++;
    }

    /* Remove Connected Muscles */
    FOR_ALL(head, 'm') {
        if (current->iData[0] == toRemove || current->iData[1] == toRemove) {
            removeItem(head, current);
            head->iData[tot]--;
            head->iData[mus]--;
        }
    }

    /* Remove Connected Bones */
    FOR_ALL(head, 'b') {
        if (current->iData[0] == toRemove || current->iData[1] == toRemove) {
            removeItem(head, current);
            head->iData[tot]--;
            head->iData[bon]--;
        }
    }

    /* Adjust Connections */ // Removing node 3, means conn > 3 need to be decremented
    for (gene * current = head; current != NULL; current = current->next) {
        if (current->start == 'm' || current->start == 'b') {
            if (current->iData[0] >= toRemove) current->iData[0]--; // should never be ==
            if (current->iData[1] >= toRemove) current->iData[1]--;
        }
    }
    return;
}

void addConnection(gene * head, double addChance, double muscleChance, double boneChance) {
    int numNodes   = head->iData[nod];
    int numMuscles = head->iData[mus];
    int numBones   = head->iData[bon];

    if (numMuscles + numBones + 1 > comb(numNodes)) return; // +1 for added

    if (chance(addChance)) {
        conn connection = goodConnection(head);
        if (chance(muscleChance)) { // make it more balanced
            if (head->iData[mus] >= MAX_ELEMENTS) return;
            addToBack(head, muscleGene(connection.a, connection.b));
            head->iData[tot]++;
            head->iData[mus]++;
        } else if (chance(boneChance)) {
            if (head->iData[bon] >= MAX_ELEMENTS) return;
            addToBack(head, boneGene(connection.a, connection.b));
            head->iData[tot]++;
            head->iData[bon]++;
        }
    }
    return;
}

void removeConnection(gene * head, double addChance) {
    int numMuscles = head->iData[mus];
    int numBones   = head->iData[bon];

    if (numMuscles + numBones <= 0) return;

    if (chance(addChance)) {
        /* No Muscles -> Remove Bone */
        if (numMuscles <= 0) { // Cant Both be 0
            int toRemove = rand() % numBones;
            if (numBones == 1) toRemove = 0;
            int i = 0;
            FOR_ALL(head, 'b') {
                if (i == toRemove) {
                   removeItem(head, current);
                   head->iData[tot]--;
                   head->iData[bon]--;
                }
                i++;
            }
            return;
        }

        /* No Bones-> Remove Muscle */
        if (numBones <= 0) {
            int toRemove = rand() % numMuscles;
            if (numMuscles == 1) toRemove = 0;
            int muscleNum = 0;
            FOR_ALL(head, 'm') {
                if (muscleNum == toRemove) {
                    removeItem(head, current);
                    head->iData[tot]--;
                    head->iData[mus]--;
                }
                muscleNum++;
            }
            return;
        }

        if (chance(50)) {
            /* Remove Muscle */
            int toRemove = rand() % numMuscles;
            if (numMuscles == 1) toRemove = 0;
            int muscleNum = 0;
            FOR_ALL(head, 'm') {
                if (muscleNum == toRemove) {
                    removeItem(head, current);
                    head->iData[tot]--;
                    head->iData[mus]--;
                    return;
                }
                muscleNum++;
            }
        } else {
            /* Remove Bone */
            int toRemove = rand() % numBones;
            if (numBones == 1) toRemove = 0;
            int i = 0;
            FOR_ALL(head, 'b') {
                if (i == toRemove) {
                    removeItem(head, current);
                    head->iData[tot]--;
                    head->iData[bon]--;
                    return;
                }
                i++;
            }
        }
    }
    return;
}

void removeStrandedNodes(gene * head) { // should be used any time connections are removed
    int numNodes = head->iData[nod];
    int * needed = (int*) malloc(sizeof(int) * numNodes);
    for (int i = 0; i < numNodes; i++) {
        needed[i] = i;
    }
    FOR_ALL_GENES(head) if (current->start == 'm' || current->start == 'b') {
        needed[current->iData[0]] = -1;
        needed[current->iData[1]] = -1;
    }

    /* Remove Node */
    int numRemoved = 0;
    for (int i = 0; i < numNodes; i++) {
        int toRemove = needed[i];
        if (toRemove == -1) continue;
        removeNodeByIndex(head, toRemove - numRemoved);
        numRemoved++;
    }
    free(needed);
    return;
}

// This function will break if muscles and bones have different attributes
void swapConnection(gene * head, double swapChance) {
    /* Muscle -> Bone */ // Can make this fast by combining

//    addAxons(head, head->iData[mus]);
//    reduceAxons(head, muscleNum--);
    int muscleNum = 0;
    FOR_ALL(head, 'm') if (chance(swapChance)) {
        if (head->iData[bon] >= MAX_ELEMENTS) break;
        int a = current->iData[0];
        int b = current->iData[1];
        if (connectionExists(head, a, b)) {
//            reduceAxons(head, muscleNum--);

            current->start = 'b';
            head->iData[mus]--;
            head->iData[bon]++;
        }
        muscleNum++;
    }

    FOR_ALL(head, 'b') {
        if (chance(swapChance)) {
            if (head->iData[mus] >= MAX_ELEMENTS) break;
            int a = current->iData[0];
            int b = current->iData[1];
            if (connectionExists(head, a, b)) {
//                addAxons(head, /* How do I know what muscle this is */);

                current->fData[0] = pmRandf(3.1415926 / 2.0);
                current->fData[1] = randf(5);

                current->start = 'm';
                head->iData[mus]++;
                head->iData[bon]--;
            }
        }
    }
    return;
}

void verifyGenome(gene * head) {
    int numGenes   = head->iData[tot];
    int numNodes   = head->iData[nod];
    int numMuscles = head->iData[mus];
    int numBones   = head->iData[bon];
    int numAxons   = head->iData[neu];

    /* Number Errors */
    if (numNodes   > MAX_ELEMENTS || numNodes   <= 0) quit(GENOME_ERROR);
    if (numMuscles > MAX_ELEMENTS || numMuscles <  0) quit(GENOME_ERROR);
    if (numBones   > MAX_ELEMENTS || numBones   <  0) quit(GENOME_ERROR);
    if (numAxons   > 0) quit(GENOME_ERROR);

    if (numNodes + numMuscles + numBones + numAxons + 1 != numGenes) quit(GENOME_ERROR);

    if (numMuscles + numBones > comb(numNodes)) quit(GENOME_ERROR);

    /* Muscle  */
    FOR_ALL(head, 'm') {
        if (fabs(current->fData[0]) > (3.1415926 / 2.0 + 0.001)) {
            printf("%f\n", current->fData[0]);
            quit(GENOME_ERROR);
        }
        if (current->fData[1] < 0) {
            quit(GENOME_ERROR);
        }
        if (current->fData[1] > 5) {
            quit(GENOME_ERROR);
        }
    }

    /* Stranded Nodes */
    if (numNodes > 1) {
        int * needed = (int*) malloc(sizeof(int) * numNodes);
        for (int i = 0; i < numNodes; i++) {
            needed[i] = i;
        }
        FOR_ALL_GENES(head) if (current->start == 'm' || current->start == 'b') {
            needed[current->iData[0]] = -1;
            needed[current->iData[1]] = -1;
        }
        for (int i = 0; i < numNodes; i++) {
            if (needed[i] != -1) {
                printf("%d, %d, %d\n", numNodes, numMuscles, numBones);
                quit(GENOME_ERROR);
            }
        }
    }


    /* Connection Errors */
    for (gene * i = head; i != NULL; i = i->next) {
        for (gene * j = i->next; j != NULL; j = j->next) {
            bool error = false;
            /* Only look at muscles and bones */
            if (i->start != 'b' && i->start != 'm') continue;
            if (j->start != 'b' && j->start != 'm') continue;

            /* Any two sets of connections */
            int a1 = i->iData[0];
            int a2 = i->iData[1];
            int b1 = j->iData[0];
            int b2 = j->iData[1];

            /* Self-Attachment */
            if (a1 == a2) error = true;
            if (b1 == b2) error = true;
            if (error) quit(GENOME_ERROR);

            /* Duplicate Attachments */
            if (a1 == b1 && a2 == b2) error = true;
            if (a1 == b2 && a2 == b1) error = true;
            if (error) quit(GENOME_ERROR);
        }
    }

    /* Node Location Errors */
    for (gene * i = head; i != NULL; i = i->next) {
        if (i->start != 'n') continue;
        bool error = false;
        double x1 = i->fData[xposi];
        double y1 = i->fData[yposi];
        double z1 = i->fData[zposi];

        for (gene * j = i->next; j != NULL; j = j->next) {
            if (j->start != 'n') continue;
            double x2 = j->fData[xposi];
            double y2 = j->fData[yposi];
            double z2 = j->fData[zposi];

            /* Bounds */
            if (fabs(x1 - x2) < MIN_NODE_DISTANCE &&
                fabs(y1 - y2) < MIN_NODE_DISTANCE &&
                fabs(z1 - z2) < MIN_NODE_DISTANCE ){
                error = true;
            }
            if (error) {
                printf("%f\n", euc(vec(x1,y1,z1), vec(x2, y2,z2)));
                quit(GENOME_ERROR);
            }

            /* Overlap  & Proximit*/
            if (euc(vec(x1, y1, z1), vec(x2, y2, z2)) < MIN_NODE_DISTANCE) quit(GENOME_ERROR);
            if ((fabs(x2 - x1) < MIN_NODE_DISTANCE) &&
                (fabs(y2 - y1) < MIN_NODE_DISTANCE) &&
                (fabs(z2 - z1) < MIN_NODE_DISTANCE)) {
                    error = true;;
            }
            if (error) quit(GENOME_ERROR);
        }
    }
    return;
}

/** TYPES OF MUTATIONS
INFO: None -> Must be updated
NODES:
    Change Mass (Big / Small)
    Change Friction
BONES:
    Change Attachment (?)
MUSCLES:
    Change Attachment (?)
NEURONS:

MUTABILITY:
    Large Change
    Small Change

**/

























