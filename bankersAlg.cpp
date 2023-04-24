#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    const int T = 4; // number of threads
    const int R = 3; // number of resources

    string resourceNames[R]; // store resource names
    int maxResource[R]; // store maximum resources in the system
    string threadsName[T]; // threads names
    int resourcesRequested[T][R]; // resources requested for each thread
    int resourceAllocated[T][R]; // allocated resources for each thread
    int neededResources[T][R]; // This matrix is computed to show the needed resources for each thread
    int currentAvailResources[R]; // This a computed array of the number of resources available

    ifstream fin;
    fin.open("/Users/mikey/documents/compOperatingSys/data.txt");

    // Read the available resources
    for (int i = 0; i < R; i++) {
        fin >> resourceNames[i] >> maxResource[i];
    }

    // Read the threads and their maximum resource requests
    for (int i = 0; i < T; i++) {
        fin >> threadsName[i];
        for (int j = 0; j < R; j++) {
            fin >> resourcesRequested[i][j];
        }
    }

    // Read the allocated resources for each thread
    for (int i = 0; i < T; i++) {
        for (int j = 0; j < R; j++) {
            fin >> resourceAllocated[i][j];
            neededResources[i][j] = resourcesRequested[i][j] - resourceAllocated[i][j]; // initialize neededResources
        }
    }

    // Compute current available resources
    for (int j = 0; j < R; j++) {
        int totalAllocated = 0;
        for (int i = 0; i < T; i++) {
            totalAllocated += resourceAllocated[i][j];
        }
        currentAvailResources[j] = maxResource[j] - totalAllocated;
    }

    // Implement the Banker's algorithm
    int safeSeq[T];
    int work[R];
    bool finish[T];
    for (int i = 0; i < R; i++) {
        work[i] = currentAvailResources[i];
    }
    for (int i = 0; i < T; i++) {
        finish[i] = false;
    }
    int count = 0;
    while (count < T) {
        bool found = false;
        for (int i = 0; i < T; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (neededResources[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == R) {
                    for (int k = 0; k < R; k++) {
                        work[k] += resourceAllocated[i][k];
                    }
                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                    cout << "Thread " << i << " finished. Current state: " << endl;
                    cout << "Work: ";
                    for (int l = 0; l < R; l++) {
                        cout << work[l] << " ";
                    }
                    cout << endl;
                    cout << "Finish: ";
                    for (int l = 0; l < T; l++) {
                        cout << finish[l] << " ";
                    }
                    cout << endl;
                    cout << "Safe sequence: ";
                    for (int l = 0; l < count; l++) {
                        cout << threadsName[safeSeq[l]] << " ";
                    }
                    cout << endl;
                }
            }
        }
        if (!found) {
            cout << "No safe state available" << endl;
            return 0;
        }
    }

    // Print the safe sequence
    cout << "Safe sequence: ";
    for (int i = 0; i < T; i++) {
        cout << threadsName[safeSeq[i]] << " ";
    }
    cout << endl;

    return 0;
}
