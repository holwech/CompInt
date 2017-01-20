#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 0
#define ANS 1
#define NMAX 10
#define MAX_POINTS 1000

// END ARRAY

typedef struct {
    double d[3000];
    int size;
} DVector;

void initDVector(DVector* vector) {
  vector->size = 0;
}

void DVPush(DVector* vector, double val) {
    vector->d[vector->size] = val;
    vector->size++;
}

void DVSetAll(DVector* vector, double val, int end) {
    int i;
    for(i = 0; i < end; i++) {
        vector->d[i] = val;
    }
}

void DVScale(DVector* vector, double scale) {
    int i;
    for(i = 0; i < vector->size; i++) {
			vector->d[i] = vector->d[i] / scale;
    }
}

double randomVal(int max, int offset, double divisor) {
  double r = (double)(rand() % max - offset) / divisor;
  return r;
}

double randomDVal(double max, double offset, double divisor) {
	int newMax = max * 10000;
	int newOffset = offset * 10000;
	//printf("newMax: %d, newOffset: %d\n", newMax, newOffset);
  double r = (rand() % newMax - newOffset) / divisor;
	r = r / 10000;
	//printf("max: %lf, offset: %lf, rand: %lf\n", max, offset, r);
  return r;
}

void DVSetAllRandom(DVector* vector, int end) {
    int i;
    for (i = 0; i < end; i++) {
        double r = randomVal(200, 100, 8500.0);
        //printf("Rval: %lf\n", r);
        DVPush(vector, r);
    }
}


// END DVECTOR

typedef struct {
	DVector points;
	int clusters;
	DVector CEst;
	DVector prevCEst;
  double maxRange;
	double maxX, minX, maxY, minY;
} KMeans;

double calcDistance(double cEstX, double cEstY, double pointX, double pointY) {
	return sqrt(pow(cEstX - pointX, 2) + pow(cEstY - pointY, 2));
}

int train(KMeans* km) {
	// Total value for each cluster
	double total[MAX_POINTS * 2] = {0};
	// Total number of points for each cluster
	double numPoints[MAX_POINTS] = {0};
	// Check each point which estimate is closest
	for (int point = 0; point < km->points.size; point += 2) {
		// Index of closest cluster
		int closestEst = 0;
		// Distance to closest cluster
		double shortestDist = 1000000;
		for (int ce = 0; ce < km->CEst.size; ce += 2) {
			double cEstX = km->CEst.d[ce];
			double cEstY = km->CEst.d[ce + 1];
			double pointX = km->points.d[point];
			double pointY = km->points.d[point + 1];
			double newShortestDist = calcDistance(cEstX, cEstY, pointX, pointY);
			// If a given estimate is closer to the point
			if (newShortestDist < shortestDist) {
				shortestDist = newShortestDist;
				closestEst = ce;
			}
		}
		total[closestEst] += km->points.d[point];
		total[closestEst + 1] += km->points.d[point + 1];
		numPoints[closestEst / 2]++;
	}
	// Average out and get new positions
	for (int ce = 0; ce < km->CEst.size; ce += 2) {
    // If zero points have been allocated to a cluster
    // return fail
    if (numPoints[ce / 2] == 0) {
      return 0;
    }
		//printf("----- Cluster: %d\n", ce);
		//printf("total: %lf, pointX: %lf\n", total[ce], numPoints[ce / 2]);
		if (total[ce] != 0.0) {
			km->CEst.d[ce] = total[ce] / numPoints[ce / 2];
		}
		//printf("CE: %d, total: %lf, pointY: %lf\n", ce, total[ce + 1], numPoints[ce / 2]);
		if (total[ce + 1] != 0.0) {
			km->CEst.d[ce + 1] = total[ce + 1] / numPoints[ce / 2];
		}
	}
  // Success
  return 1;
}

void readFromFile(KMeans* km);
void readFromConsole(KMeans* km);


int runKMeans(KMeans km, double sigma) {
	int done = 0;
	//printf("maxX: %lf, maxY: %lf, minX: %lf, minY: %lf\n", km.maxX, km.maxY, km.minX, km.minY);
	for (int cluster = 0; cluster < km.clusters; cluster++) {
		//printf("----- Cluster: %d\n", cluster);
		DVPush(&km.CEst, randomDVal(fabs(km.maxX) + fabs(km.minX), fabs(km.minX), 1));
		DVPush(&km.CEst, randomDVal(fabs(km.maxY) + fabs(km.minY), fabs(km.minY), 1));
	}
	// Set deltas to a non-zero value the first round
	for (int cluster = 0; cluster < km.CEst.size; cluster += 2) {
		km.prevCEst.d[cluster] = 10000000;
	}
  int count = 0;
  int converged = 1;
	while (!done) {
		converged = train(&km);
		count++;
		done = 1;
		//printf("-----\n");
		for (int cluster = 0; cluster < km.CEst.size; cluster += 2) {
			//printf("%lf\n",fabs(km.CEst.d[cluster] - km.prevCEst.d[cluster]));
			if (fabs(km.CEst.d[cluster] - km.prevCEst.d[cluster]) > 0.0001) {
				km.prevCEst.d[cluster] = km.CEst.d[cluster];
				km.prevCEst.d[cluster + 1] = km.CEst.d[cluster + 1];
				done = 0;
			}
		}
	}
  if (converged == 0) {
    return 0;
  }
  for (int cluster = 0; cluster < km.CEst.size - 2; cluster += 2) {
    for (int otherCluster = cluster + 2; otherCluster < km.CEst.size; otherCluster +=2) {
      if (sqrt(pow(km.CEst.d[cluster] - km.CEst.d[otherCluster], 2) +
               pow(km.CEst.d[cluster + 1] - km.CEst.d[otherCluster + 1], 2)) <
          km.maxRange * sigma) {
        return 0;
        /*printf("Clusters {%lf, %lf} and {%lf, %lf} too close\n",
                km.CEst.d[cluster - 2], km.CEst.d[cluster - 1],
                km.CEst.d[cluster], km.CEst.d[cluster + 1]);
        */
      }
    }
  }
	for (int cluster = 0; cluster < km.CEst.size; cluster += 2) {
		printf("%lf,%lf\n", km.CEst.d[cluster], km.CEst.d[cluster + 1]);
		//printf("(%lf,%lf),", km.CEst.d[cluster], km.CEst.d[cluster + 1]);
    //printf("numRetries: %d\n", numRetries);
	}
  return 1;
}

void KMeansHandler() {
	KMeans km;
	initDVector(&km.points);
	initDVector(&km.prevCEst);
    readFromConsole(&km);
  if (km.maxX - km.minX > km.maxY - km.minY) {
    km.maxRange = km.maxX - km.minX;
  } else {
    km.maxRange = km.maxY - km.minY;
  }
   int done = 0;
   int runs = 0;
   double sigma = 0.06;
   while (done == 0) {
     done = runKMeans(km, sigma);
     runs++;
     if (runs == 100) {
        sigma -= 0.02;
     }
   }
}

void runOldKMeans() {
  int retry = 1;
  while (retry == 1) {
    //printf("Running KMeans\n");
  	KMeans km;
  	initDVector(&km.points);
  	initDVector(&km.CEst);
  	initDVector(&km.prevCEst);
  	readFromConsole(&km);
    if (km.maxX - km.minX > km.maxY - km.minY) {
      km.maxRange = km.maxX - km.minX;
    } else {
      km.maxRange = km.maxY - km.minY;
    }
  	int done = 0;
  	//printf("maxX: %lf, maxY: %lf, minX: %lf, minY: %lf\n", km.maxX, km.maxY, km.minX, km.minY);
  	for (int cluster = 0; cluster < km.clusters; cluster++) {
  		//printf("----- Cluster: %d\n", cluster);
  		DVPush(&km.CEst, randomDVal(fabs(km.maxX) + fabs(km.minX), fabs(km.minX), 1));
  		DVPush(&km.CEst, randomDVal(fabs(km.maxY) + fabs(km.minY), fabs(km.minY), 1));
  	}
  	// Set deltas to a non-zero value the first round
  	for (int cluster = 0; cluster < km.CEst.size; cluster += 2) {
  		km.prevCEst.d[cluster] = 10000000;
  	}
  	int count = 0;
    int converged = 1;
  	while (!done) {
  		converged = train(&km);
  		count++;
  		done = 1;
  		//printf("-----\n");
  		for (int cluster = 0; cluster < km.CEst.size; cluster += 2) {
  			//printf("%lf\n",fabs(km.CEst.d[cluster] - km.prevCEst.d[cluster]));
  			if (fabs(km.CEst.d[cluster] - km.prevCEst.d[cluster]) > 0.0001) {
  				km.prevCEst.d[cluster] = km.CEst.d[cluster];
  				km.prevCEst.d[cluster + 1] = km.CEst.d[cluster + 1];
  				done = 0;
  			}
  		}
  	}
    retry = 0;
    if (converged == 0) {
      //printf("Clusters did not converge\n");
      retry = 1;
    } else {
      for (int cluster = 0; cluster < km.CEst.size - 2; cluster += 2) {
        for (int otherCluster = cluster + 2; otherCluster < km.CEst.size; otherCluster +=2) {
          if (sqrt(pow(km.CEst.d[cluster] - km.CEst.d[otherCluster], 2) +
                   pow(km.CEst.d[cluster + 1] - km.CEst.d[otherCluster + 1], 2)) <
              km.maxRange * 0.1) {
            retry = 1;
            /*printf("Clusters {%lf, %lf} and {%lf, %lf} too close\n",
                    km.CEst.d[cluster - 2], km.CEst.d[cluster - 1],
                    km.CEst.d[cluster], km.CEst.d[cluster + 1]);
            */
          }
        }
      }
    }
	  //printf("Count: %d\n", count);
    if (retry == 0) {
    	for (int cluster = 0; cluster < km.CEst.size; cluster += 2) {
    		printf("%lf,%lf\n", km.CEst.d[cluster], km.CEst.d[cluster + 1]);
    		//printf("(%lf,%lf),", km.CEst.d[cluster], km.CEst.d[cluster + 1]);
        //printf("numRetries: %d\n", numRetries);
    	}
      return;
    }
  }
}

// END KMEANS


void readFromFile(KMeans* km) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  int read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
	// Get number of clusters
	read = getline(&line, &length, filePtr);
	sscanf(line, "%d", &km->clusters);
	read = getline(&line, &length, filePtr);
	double x, y;
	char ch;
	sscanf(line, "%lf%c%lf", &x, &ch, &y);
	km->maxX = x;
	km->minX = x;
	km->maxY = y;
	km->minY = y;
	DVPush(&km->points, x);
	DVPush(&km->points, y);
  while ((read = getline(&line, &length, filePtr)) != -1) {
    sscanf(line, "%lf%c%lf", &x, &ch, &y);
		DVPush(&km->points, x);
		DVPush(&km->points, y);
		if (x > km->maxX) { km->maxX = x; }
		if (x < km->minX) { km->minX = x; }
		if (y > km->maxY) { km->maxY = y; }
		if (y < km->minY) { km->minY = y; }
	}
  fclose(filePtr);
  if (line) {
      free(line);
  }
}

void readFromConsole(KMeans* km) {
	double x, y;
	char ch;
	scanf("%d", &km->clusters);
	scanf("%lf%c%lf", &x, &ch, &y);
	km->maxX = x;
	km->minX = x;
	km->maxY = y;
	km->minY = y;
	DVPush(&km->points, x);
	DVPush(&km->points, y);
	while(scanf("%lf%c%lf", &x, &ch, &y) != EOF) {
		DVPush(&km->points, x);
		DVPush(&km->points, y);
		if (x > km->maxX) { km->maxX = x; }
		if (x < km->minX) { km->minX = x; }
		if (y > km->maxY) { km->maxY = y; }
		if (y < km->minY) { km->minY = y; }
	}
	// Get number of clusters
}

int main() {
  srand(time(0));
	KMeansHandler();
	return (0);
}
