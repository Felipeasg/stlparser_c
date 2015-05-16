/*
 * main.c
 *
 *  Created on: May 16, 2015
 *      Author: felipeasg
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OF_FACETS				100000
#define MAX_OF_CHAR_IN_ONE_LINE	 	100

typedef struct _Vertex{
float x;
float y;
float z;
} Vertex;


typedef struct _Facet {
Vertex normal; // facet surface normal
Vertex v1; // vertex 1
Vertex v2; // vertex 2
Vertex v3; // vertex 3
} Facet;

typedef struct _STL {
	Facet facet[MAX_OF_FACETS];
	unsigned int numberOfFacets;
	unsigned int numberOfVertex;
}STL;

int main(int argc, char* argv[])
{
	FILE* stlFile;
	char lineToRead[MAX_OF_CHAR_IN_ONE_LINE];
	STL stl;
	int numberOfFacets = 0;



	stlFile = fopen(argv[1], "r");
//	stlFile = fopen("sphmms128.stl", "r");

	printf("Opening stl!\n");
	if(stlFile == NULL)
	{
		printf("File could not be open!\n");

		return 0;
	}

	stl.numberOfFacets = 0;
	stl.numberOfVertex = 0;
	while(strcmp(lineToRead, "endsolid\n")!=0)
	{
		if(fgets(lineToRead, 1000, stlFile) == NULL)
			break;
		//printf("%s", lineToRead);

		char *pch = strtok(lineToRead, " ");
		if(strcmp(pch,"solid\n") == 0)
		{
			printf("solid\n");
			continue;
		}
		else if(strcmp(pch,"solid\n") != 0 && strcmp(pch,"facet") != 0 && strcmp(pch,"outer") != 0 && strcmp(pch,"vertex") != 0)
		{
			continue;
		}

		if(strcmp(pch,"facet") == 0)
		{
			pch = strtok (NULL, " "); //get normal string

			pch = strtok (NULL, " "); //get x
			float x = atof(pch);

			pch = strtok (NULL, " "); //get y
			float y = atof(pch);

			pch = strtok (NULL, " "); //get z
			float z = atof(pch);

			stl.facet[numberOfFacets].normal.x = x;
			stl.facet[numberOfFacets].normal.y = y;
			stl.facet[numberOfFacets].normal.z = z;


			continue;
		}

		if(strcmp(pch,"outer") == 0)
		{
			pch = strtok (NULL, " "); //get loopstring

			continue;
		}

		if(strcmp(pch,"vertex") == 0)
		{
			static int vertexCount = 0;

			pch = strtok (NULL, " "); //get x
			double x = atof(pch);

			pch = strtok (NULL, " "); //get y
			double y = atof(pch);

			pch = strtok (NULL, " "); //get z
			double z = atof(pch);

			switch(vertexCount)
			{
			case 0:

				stl.facet[numberOfFacets].v1.x = x;
				stl.facet[numberOfFacets].v1.y = y;
				stl.facet[numberOfFacets].v1.z = z;

				break;
			case 1:

				stl.facet[numberOfFacets].v2.x = x;
				stl.facet[numberOfFacets].v2.y = y;
				stl.facet[numberOfFacets].v2.z = z;

				break;
			case 2:

				stl.facet[numberOfFacets].v3.x = x;
				stl.facet[numberOfFacets].v3.y = y;
				stl.facet[numberOfFacets].v3.z = z;

				break;
			}

			vertexCount++;
			if(vertexCount >= 3)
			{
				numberOfFacets++;

				stl.numberOfFacets = numberOfFacets;

				stl.numberOfVertex += vertexCount;
				vertexCount = 0;
			}

			continue;
		}

		if(numberOfFacets > MAX_OF_FACETS)
		{
			printf("Number of facets is big. Change the MAX_OF_FACETS.\n");
			break;
		}

	}

	fclose(stlFile);

	printf("\n-Printing stl file-\n");
	printf("Number of facets: %d\n", stl.numberOfFacets);
	printf("Number of vertexes: %d\n\n", stl.numberOfVertex);

	for(int i = 0; i < stl.numberOfFacets; i++)
	{
		printf("normal[%lf %lf %f]\n",stl.facet[i].normal.x,
				stl.facet[i].normal.y,
				stl.facet[i].normal.z);

		printf(" v1[%lf %lf %f]\n",stl.facet[i].v1.x,
						stl.facet[i].v1.y,
						stl.facet[i].v1.z);

		printf(" v2[%lf %lf %f]\n",stl.facet[i].v2.x,
						stl.facet[i].v2.y,
						stl.facet[i].v3.z);

		printf(" v3[%lf %lf %f]\n",stl.facet[i].v3.x,
						stl.facet[i].v3.y,
						stl.facet[i].v3.z);
	}


	return 0;
}
