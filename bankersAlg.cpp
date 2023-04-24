#include <iostream>
#include <thread>
#include <fstream>
using namespace std;

const int T = 4; // Threads
const int R = 3; // Resources

string resourceNames[R]; // Store resource name
int maxResource[R]; // Avaliable resources of each type

string threadName[T]; // Store thread name
int resourcesRequested[T][R]; // Number of each resource that each thread may want
int resourcesAllocated[T][R]; // Number of each resource that each thread is using
int neededResources[T][R]; // Number of resources that each thread might still request

int currentAvailResource[R]; // Current available resources


void readFile(string filename);
void getNeededRecourses();
void getAvailableRecourses();
bool isSafe();


int main() {
    readFile("/Users/mikey/documents/compOperatingSys/data.txt");

    cout << endl;

    int numSafeStates = isSafe();
    if (numSafeStates > 0) {
        cout << "Number of safe states: " << numSafeStates << endl;
    } else {
        cout << "No safe state found." << endl;
    }

    return 0;
}




void readFile(string filename) {
    ifstream fin;
    fin.open(filename);

    if (!fin) {
        cout << "Error opening file: " << filename << endl;
        return;
    }
    char ignore;

    // Read the available resources
    for (int i = 0; i < R; i++) {
        fin >> resourceNames[i] >> maxResource[i];
    }

    fin >> ignore; // Handles the #

    // Read the requested resources for each thread
    for (int i = 0; i < T; i++) {
        fin >> threadName[i];
        for (int j = 0; j < R; j++) {
            fin >> resourcesRequested[i][j];
        }
    }

    fin >> ignore;

    // Read the allocated resources
    for (int i = 0; i < T; i++) {
        for (int j = 0; j < R; j++) {
            fin >> resourcesAllocated[i][j];
        }
    }
}

void getNeededRecourses() {
    for (int i = 0; i < T; i++) {
        for (int j = 0; j < R; j++) {
            // Needed = Max need to complete - Allocated
            neededResources[i][j] = resourcesRequested[i][j] - resourcesAllocated[i][j];
        }
    }
}

void getAvailableRecourses() {
    // Get available resources
    for (int i = 0; i < R; i++) {
        currentAvailResource[i] = maxResource[i];
    }

    // Available = Max - Sum of Allocated
    for (int i = 0; i < T; i++) {
        for (int j = 0; j < R; j++) {
            currentAvailResource[j] -= resourcesAllocated[i][j];
        }
    }
}

bool isSafe() {
    string completedThreads[T];
    bool finished[T];
    int safeSequenceCount = 0; // Counter for safe sequences

    // Set all finished to false.
    for (int i = 0; i < T; i++) {
        finished[i] = false;
    }

    getNeededRecourses();
    getAvailableRecourses();

    for (int i = 0; i < T; i++) {

        // Find thread that can be completed
        if (!finished[i]) {
            // Check threads
            for (int j = 0; j < T; j++) {

                // Check if process already completed.
                bool processCompleted = false;
                for (int i = 0; i < T; i++) {
                    if (threadName[j] == completedThreads[i]) {
                        processCompleted = true;
                    }
                }

                if (!processCompleted) {

                    int hasAllResources = 0;
                    for (int k = 0; k < R; k++) {

                        if (currentAvailResource[k] >= neededResources[j][k]) {
                            hasAllResources++;
                        }
                    }
                    if (hasAllResources == R) {
                        // Set thread to finished if resources complete
                        finished[i] = true;
                        completedThreads[i] = threadName[j];
                        //Add back resources to available.
                        for (int k = 0; k < R; k++) {
                            currentAvailResource[k] += resourcesAllocated[j][k];
                        }
                        break;
                    }
                }
            }
        }
    }

    bool isSafe;

    for (int i = 0; i < T; i++) {
        if (!finished[i]) {
            isSafe = false;
            cout << "No safe state possible" << endl;
            break;
        }
        else {
            isSafe = true;
        }
    }

    if (isSafe) {
        safeSequenceCount++; // Increment the counter
        cout << "The safe state is: {";
        for (int i = 0; i < T; i++) {
            cout << completedThreads[i];
            if (i != T - 1) {
                cout << ", ";
            }
        }
        cout << "}";
        cout << endl;
    }

    if (safeSequenceCount > 1) {
        cout << "There are " << safeSequenceCount << " safe sequences" << endl;
    }

    return isSafe;
}

