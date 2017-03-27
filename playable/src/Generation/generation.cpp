#include "Generation/generation.h"

#include <stdlib.h>            // NULL
#include "Math/myMath.h"       // Com, euc
#include "global.h"            // Globals
#include "Generation/genes.h"  // Various Genes
#include "Functional/list.h"   // Various Linked List Functions
#include "ErrorHandle/error.h" // Quit

#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
gene * parseGenome(gene * head) {
    static int trials = 0; // or 0?
    static int counter = 0;
    if (counter % genSize == 0) {
        trials++;
    }
    counter++;
    /* Get Genome String */
    std::string s = \
    "250:F:0.209656><i|5,2,4,0,12,<n|-3.000000,12.000000,16.422843,1.769677,0.469469,<n|17.000000,2.000000,9.018583,2.536058,0.137867,<n|12.000000,17.000000,5.789256,4.326151,0.600992,<n|-17.000000,17.000000,6.885105,3.351024,0.853148,<n|-4.000000,-2.000000,10.278294,1.667623,0.119239,<m|1,4,<b|0,3,<b|0,4,<m|0,1,<m|2,0,<m|3,2,<";
//    std::fstream myfile("../assets/genomes.txt");
//    for (int i = 0; (i != trials) && getline(myfile, s); i++);
//    myfile.close();

    //std::cout << trials << std::endl;
    /* Fill Genome */
    std::string geneDelim = "<";
    std::string dataDelim = ",";
    std::string geneStr;
    size_t genePos = 0;
    while ((genePos = s.find(geneDelim)) != std::string::npos) {
        geneStr = s.substr(0, genePos);
        s.erase(0, genePos + geneDelim.length());
        char geneType = geneStr[0];
        if (isdigit(geneType)) { // ignores first entry
            continue;
        }
        geneStr = geneStr.substr(2, std::string::npos);
        size_t dataPos = 0;
        std::string dataStr;

        generic datums[10];
        int i = 0;
        while ((dataPos = geneStr.find(dataDelim)) != std::string::npos) {
            dataStr = geneStr.substr(0, dataPos);
            if (geneType == 'n') {
                datums[i].f = atof(dataStr.c_str());
            } else {
                datums[i].i = atoi(dataStr.c_str());
            }
            geneStr.erase(0, dataPos + dataDelim.length());
            i++;
        }

        switch (geneType) {
            case 'i':
                head = infoGene(datums[0].i, datums[1].i, datums[2].i, datums[3].i);
                break;
            case 'n':
                head = addToBack(head, nodeGene(datums[0].f, datums[1].f, datums[2].f, datums[3].f, datums[4].f));
                break;
            case 'b':
                head = addToBack(head, boneGene(datums[0].i, datums[1].i));
                break;
            case 'm':
                head = addToBack(head, muscleGene(datums[0].i, datums[1].i));
                break;
            default:
                quit(GENOME_ERROR);
        }
    }
    head = addToBack(head, NULL); // Can likely remove
    return head;
}

gene * createGenome(gene * head, int a, int b, int c, int d) {
    return parseGenome(head);
    if (b + c > comb(a)) quit(GENOME_ERROR);


    /* Creating Genome */
    head = infoGene(a, b, c, d);
    int * sizes = head->iData;

    for (int i = 0; i < sizes[nod]; i++) {
        head = addToBack(head, nodeGene(head));
    }
    for (int i = 0; i < sizes[bon]; i++) {
        head = addToBack(head, boneGene(head));
    }
    for (int i = 0; i < sizes[mus]; i++) {
        head = addToBack(head, muscleGene(head));
    }
    head = addToBack(head, NULL); // Can likely remove
    return head;
}

void createCreature(creature * newBorn) {
    /* Reading genome */
    int numNodes   = 0;
    int numBones   = 0;
    int numMuscles = 0;
    int numAxons   = 0;

    gene * currentGenome = newBorn->genome;
    while (currentGenome != NULL) {
        int a;
        int b;
        switch (currentGenome->start) {
            case 'n':
                newBorn->nodes[numNodes].loc.x = currentGenome->fData[xposi];
                newBorn->nodes[numNodes].loc.y = currentGenome->fData[yposi];
                newBorn->nodes[numNodes].loc.z = currentGenome->fData[zposi];

                newBorn->nodes[numNodes].vel.x = 0.0;
                newBorn->nodes[numNodes].vel.y = 0.0;
                newBorn->nodes[numNodes].vel.z = 0.0;

                newBorn->nodes[numNodes].force.x = 0.0;
                newBorn->nodes[numNodes].force.y = 0.0;
                newBorn->nodes[numNodes].force.z = 0.0;

                newBorn->nodes[numNodes].mass     = currentGenome->fData[mass];
                newBorn->nodes[numNodes].friction = currentGenome->fData[fric];

                numNodes++;
                break;
            case 'b':
                a = newBorn->bones[numBones].a = currentGenome->iData[0];
                b = newBorn->bones[numBones].b = currentGenome->iData[1];
                newBorn->bones[numBones].length = euc(newBorn->nodes[a].loc, newBorn->nodes[b].loc);
                numBones++;
                break;
            case 'm':
                a = newBorn->muscles[numMuscles].a = currentGenome->iData[0];
                b = newBorn->muscles[numMuscles].b = currentGenome->iData[1];
                newBorn->muscles[numMuscles].origLength = euc(newBorn->nodes[a].loc, newBorn->nodes[b].loc);
                numMuscles++;
                break;
            case 'a':
                break; // No axons
                newBorn->axons[numAxons].a      = currentGenome->iData[connectionA];
                newBorn->axons[numAxons].b      = currentGenome->iData[connectionB];
                newBorn->axons[numAxons].layer  = currentGenome->iData[layerE];
                newBorn->axons[numAxons].weight = currentGenome->fData[weightE];
                numAxons++;
                break;
            default:
                break;
        }
        currentGenome = currentGenome->next;
    }
    newBorn->origin = getCom(*newBorn);
    // Add a check here for valid creature
    return;
}














