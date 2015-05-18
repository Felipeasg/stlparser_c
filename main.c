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
#include <stdint.h>
#include <float.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glut.h>

#define MAX_OF_FACETS				100000
#define MAX_OF_CHAR_IN_ONE_LINE	 	100

#define VERBOSE	0

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

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

GLfloat xf, yf, win;
GLint view_w, view_h;

STL stl;

GLfloat xmax = FLT_MIN;
GLfloat xmin = FLT_MAX;
GLfloat ymax = FLT_MIN;
GLfloat ymin = FLT_MAX;
GLfloat zmax = FLT_MIN;
GLfloat zmin = FLT_MAX;

GLfloat zoomfactor = 1000;

// Inicializa parâmetros de rendering
void Initialize (void)
{
	// Define a cor de fundo da janela de visualização como preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Função callback chamada quando o tamanho da janela é alterado
void resize(GLsizei w, GLsizei h)
{
	// Evita a divisao por zero
	if(h == 0) h = 1;

	// Especifica as dimensões da Viewport
	glViewport(0, 0, w, h);

	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	if(w < h)
	{
		glOrtho(-zoomfactor, zoomfactor, -zoomfactor*(h/w),
				zoomfactor*h/w, -zoomfactor*5000.0f, zoomfactor*5000);
	}
	else
	{
		glOrtho(-zoomfactor*w/h, zoomfactor*w/h,
		            -zoomfactor, zoomfactor, -zoomfactor*5000.0f, zoomfactor*5000.0f);
	}


}


// Função callback chamada para fazer o desenho
void Draw(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Limpa a janela de visualização com a cor de fundo especificada
	glClear(GL_COLOR_BUFFER_BIT);

	// Especifica que a cor corrente é vermelha
	//         R     G     B
	glColor3f(1.0f, 0.0f, 0.0f);

	// Desenha um quadrado preenchido com a cor corrente
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < stl.numberOfFacets; i++)
	{
		glNormal3f(stl.facet[i].normal.x, stl.facet[i].normal.y, stl.facet[i].normal.y);

		glVertex3f(stl.facet[i].v1.x, stl.facet[i].v1.y, stl.facet[i].v1.y);
		glVertex3f(stl.facet[i].v2.x, stl.facet[i].v2.y, stl.facet[i].v2.y);
		glVertex3f(stl.facet[i].v3.x, stl.facet[i].v3.y, stl.facet[i].v3.y);
	}
	glEnd();

	// Executa os comandos OpenGL
	glFlush();
}



int parseSTLFile(const char* filename, STL* stl)
{
	FILE* stlFile;
	char lineToRead[MAX_OF_CHAR_IN_ONE_LINE];

	int numberOfFacets = 0;
	//	char* filename;

	stlFile = fopen(filename, "r");
	//	stlFile = fopen("sphmms128.stl", "r");

#if VERBOSE
	printf("Opening stl!\n");
#endif

	if(stlFile == NULL)
	{
		printf("File could not be open!\n");

		return -1;
	}

	stl->numberOfFacets = 0;
	stl->numberOfVertex = 0;
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

			stl->facet[numberOfFacets].normal.x = x;
			stl->facet[numberOfFacets].normal.y = y;
			stl->facet[numberOfFacets].normal.z = z;


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
			if(x > xmax) xmax = x;
			if(x < xmin) xmin = x;

			pch = strtok (NULL, " "); //get y
			double y = atof(pch);
			if(y > ymax) ymax = y;
			if(y < ymin) ymin = y;

			pch = strtok (NULL, " "); //get z
			double z = atof(pch);
			if(z > zmax) zmax = z;
			if(z < zmin) zmin = z;

			switch(vertexCount)
			{
			case 0:

				stl->facet[numberOfFacets].v1.x = x;
				stl->facet[numberOfFacets].v1.y = y;
				stl->facet[numberOfFacets].v1.z = z;

				break;
			case 1:

				stl->facet[numberOfFacets].v2.x = x;
				stl->facet[numberOfFacets].v2.y = y;
				stl->facet[numberOfFacets].v2.z = z;

				break;
			case 2:

				stl->facet[numberOfFacets].v3.x = x;
				stl->facet[numberOfFacets].v3.y = y;
				stl->facet[numberOfFacets].v3.z = z;

				break;
			}

			vertexCount++;
			if(vertexCount >= 3)
			{
				numberOfFacets++;

				stl->numberOfFacets = numberOfFacets;

				stl->numberOfVertex += vertexCount;
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

#if VERBOSE
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
#endif


	return 0;
}

int main(int argc, char* argv[])
{



	if(parseSTLFile(argv[1], &stl) == -1)
		return 0;

	printf("Number of facets: %d\n", stl.numberOfFacets);
	printf("Number of vertexes: %d\n", stl.numberOfVertex);
	printf("Xmax = %f, Xmin = %f\n", xmax, xmin);
	printf("Ymax = %f, Ymin = %f\n", ymax, ymin);
	printf("Zmax = %f, Zmin = %f\n", zmax, zmin);

	zoomfactor = MAX(MAX(fabs(xmax-xmin), fabs(ymax-ymin)),fabs(zmax-zmin));

#if 0
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
#endif

	argv = NULL;
	argc = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("STL Parser");
	glutDisplayFunc(Draw);
	glutReshapeFunc(resize);
	Initialize();
	glutMainLoop();



	return 0;
}

#if 0
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>


 #define p_ntohl(u) ({const uint32_t Q=0xFF000000;       \
                     uint32_t S=(uint32_t)(u);           \
                   (*(uint8_t*)&Q)?S:                    \
                   ( (S<<24)|                            \
                     ((S<<8)&0x00FF0000)|                \
                     ((S>>8)&0x0000FF00)|                \
                     ((S>>24)&0xFF) );  })

main (void)
{
    uint32_t s[0x40];
    assert((unsigned char)1 == (unsigned char)(257));
    memset(s, 0, sizeof(s));
    fgets((char*)s, sizeof(s), stdin);

    switch (p_ntohl(s[0])) {
        case 'open':
        case 'read':
        case 'seek':
            puts("ok");
            break;
        case 'rm\n\0':
            puts("not authorized");
            break;
        default:
            puts("unrecognized command");
    }
    return 0;
}
#endif
