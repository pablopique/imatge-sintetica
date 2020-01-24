#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "shapes/sphere.h"
#include "cameras/ortographic.h"
#include "cameras/perspective.h"

void transformationsExercise()
{
    /* *************** */
    /* TRANSFORMATIONS */
    /* *************** */
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";

    std::cout << separatorStar << "Construct and visualize transforms" << separatorStar << std::endl;

    // Identity Matrix
    Matrix4x4 idMat; // By default, a matrix4x4 is initialized with the identity matrix
    std::cout << "The content of matrix idMat is: \n" << std::endl;
    std::cout << idMat << separator << std::endl;

    // Translation Matrix
    double deltaX, deltaY, deltaZ;
    deltaX = 1; deltaY = -2; deltaZ = 3;
    Vector3D delta(deltaX, deltaY, deltaZ);
    Matrix4x4 translationMat = Matrix4x4::translate(delta);
    std::cout << "The content of matrix translationMat is: \n" << std::endl;
    std::cout << translationMat << separator << std::endl;

    // Scale Matrix
	double sX, sY, sZ;
	sX = 2; sY = 1; sZ = -1;
	Vector3D s(sX, sY, sZ);
	Matrix4x4 scaleMatrix = Matrix4x4::scale(s);
    std::cout << "The content of matrix scaleMatrix is: \n" << std::endl;
    std::cout << scaleMatrix << separator << std::endl;

    // Rotate around X Matrix
    double angleInDegrees = 60;
    double thetaInRadians = Utils::degreesToRadians(angleInDegrees);
	double rX, rY, rZ;
	rX = 1; rY = 0; rZ = 0;
	Vector3D rotateX(rX, rY, rZ);
	Matrix4x4 rotateXMatrix = Matrix4x4::rotate(thetaInRadians, rotateX);
    std::cout << "The content of matrix rotateXMatrix is: \n" << std::endl;
    std::cout << rotateXMatrix << separator << std::endl;

    // Rotate around an arbitrary axis Matrix
	double angleInDegreesArtitrary = 30;
	double thetaInRadiansArtitrary = Utils::degreesToRadians(angleInDegreesArtitrary);
	double raX, raY, raZ;
	raX = 1; raY = 1; raZ = 1;
	Vector3D rotateArtitrary(raX, raY, raZ);
	Matrix4x4 rotateArtitraryAxisMatrix = Matrix4x4::rotate(thetaInRadiansArtitrary, rotateArtitrary);
    std::cout << "The content of matrix rotateArtitraryAxisMatrix is: \n" << std::endl;
    std::cout << rotateArtitraryAxisMatrix << separator << std::endl;

    // Transposed and Inversion
    std::cout << separatorStar << "Inverting and Transposing a Matrix" << separatorStar << std::endl;
    Matrix4x4 translationMatTransposed;
    translationMat.transpose(translationMatTransposed);
    std::cout << "The transpose of matrix \n\n" << translationMat << "\n is \n\n"
              << translationMatTransposed << separator << std::endl;

    Matrix4x4 inverseTranslationMat;
	translationMat.inverse(inverseTranslationMat);
    std::cout << "The inverse of matrix \n\n" << translationMat << "\n is \n\n" << inverseTranslationMat << std::endl;
    std::cout << "And their multiplication should thus give the identity matrix:\n\n";
	std::cout << inverseTranslationMat.operator*(translationMat) << std::endl;

    // Combine here some transforms, and visualize the result
    std::cout << separatorStar << "Combine transforms and visualize the result" << separatorStar << std::endl;

    Matrix4x4 scaleTranslate = scaleMatrix.operator*(translationMat);
    std::cout << "The content of matrix scaleTranslate is: \n" << std::endl;
    std::cout << scaleTranslate << separator << std::endl;

    Matrix4x4 translateScale = translationMat.operator*(scaleMatrix);
    std::cout << "The content of matrix translateScale is: \n" << std::endl;
    std::cout << translateScale << separator << std::endl;
}

void normalTransformExercise()
{
    std::string separator = "\n----------------------------------------------\n";

    // NOTE: points, vectors and normals are represented, in this framework, using
    //  the Vector3D class. It is up to the programmer to take care of whether the
    //  Vector3D is representing a point, a vector or a normal. For example, when
    //  applying a transform using a Matrix4x4, the programmer must choose which
    //  "applyTransform" method of the namespace Matrix4x4 should be used.

    Vector3D v(1, 1, 0);
    Vector3D n(-1.0/sqrt(2), 1.0/sqrt(2), 0);
    Matrix4x4 S = Matrix4x4::scale(Vector3D(2, 1, 1));

    std::cout << "Vector v = " << v << "\n" << std::endl;
    std::cout << "Normal n = " << n << "\n" << std::endl;
    std::cout << "Scale Matrix: \n" << std::endl;
    std::cout << S << std::endl;

    Vector3D vTransformed = S.transformVector(v);
    std::cout << "Vector v\' = " << vTransformed << "\n" << std::endl;

	Vector3D nTransformed = S.transformVector(n);
	std::cout << "Vector n\' = " << nTransformed << "\n" << std::endl;
	std::cout << "dot(n\', v\') = " << dot(nTransformed, vTransformed) << "\n" << std::endl;

	Matrix4x4 sTransposed, sTransposedInverse; 
	S.transpose(sTransposed); //Transpose S matrix
	sTransposed.inverse(sTransposedInverse); //Inverse of the transpose S matrix
	Vector3D nTransformedCorrectly = sTransposedInverse.transformVector(n);
	std::cout << "Vector n\' = " << nTransformedCorrectly << "\n" << std::endl;
	std::cout << "dot(n\', v\') = " << dot(nTransformedCorrectly, vTransformed) << "\n" << std::endl;
}

void paintingAnImageExercise()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);

    for(unsigned int col = 0; col < resX; col++)
        {
            for(unsigned int row = 0; row < resY; row++)
            {
                Vector3D color((col + 0.5) / resX, (row + 0.5) / resY, 0);
                film.setPixelValue(col,row, color);
            }
        }

    // Save the final result to file
    film.save();
}

void filteringAnImageExercise()
{
	// Create two instances of the film class with the same resolution
	int resX, resY;
	resX = 512;
	resY = 512;
	Film f1(resX, resY);
	Film f2(resX, resY);

	// Create the original image
	//  Draw a circle centered at centerX, centerY (in pixels, image space)
	//   and with ray r (also in pixels)
	int centerX = resX / 2;
	int centerY = resY / 2;
	int r = std::min(centerX, centerY) / 2;
	for (int lin = 0; lin < resX; lin++)
	{
		for (int col = 0; col < resY; col++)
		{
			// Use the equation of the sphere to determine the pixel color
			if ((lin - centerX)*(lin - centerX) + (col - centerY)*(col - centerY) < r*r)
				f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
		}
	}

	// Filter-related variables
	// Declare here your filter-related variables
	const int filterSize = 3;
	int iterations = 20;
	int pixelsFilter = pow(filterSize, 2);
	int offset = filterSize / 2;
	Film *filmToFilter = &f1;
	Film *filmFiltered = &f2;
	double red = 0, green = 0, blue = 0;



	// Implement here your image filtering algorithm
	for (int i = 0; i < iterations; i++) {
		for (int lin = offset; lin < resX - offset; lin++)
		{
			for (int col = offset; col < resY - offset; col++)
			{
				red = 0;
				green = 0;
				blue = 0;
				for (int x = lin - offset; x <= lin + offset; x++) {
					for (int y = col - offset; y <= col + offset; y++) {
						Vector3D colorPixel = filmToFilter->getPixelValue(x, y);
						red += colorPixel.x;
						green += colorPixel.y;
						blue += colorPixel.z;
					}
				}
				Vector3D resultPixel = Vector3D(red / pixelsFilter, green / pixelsFilter, blue / pixelsFilter);
				filmFiltered->setPixelValue(lin, col, resultPixel);
			}
		}
		if (i != iterations - 1) {
			if (i % 2 == 0) {
				filmToFilter = &f2;
				filmFiltered = &f1;
			}
			else {
				filmToFilter = &f1;
				filmFiltered = &f2;
			}
		}
	}

	// DO NOT FORGET TO SAVE YOUR IMAGE!
	filmFiltered->save();
}


void completeSphereClassExercise()
{
	Sphere sphere(1, Matrix4x4::translate(Vector3D(0, 0, 3)));
	Ray r1(Vector3D(0, 0, 0), Vector3D(0, 0, 1));
	std::cout << "This is our sphere: " + sphere.toString() << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << "This is our ray1: " + r1.toString() << std::endl;
	if (sphere.rayIntersectP(r1)) 
	{
		std::cout << "Ray intersects with sphere" << std::endl;
	}
	else 
	{
		std::cout << "Ray does not intersect with sphere" << std::endl;
	}
	std::cout << "------------------------------------------------" << std::endl;
	Ray r2(Vector3D(0, 0, 0), Vector3D(0, 1, 0));
	std::cout << "This is our ray2: " + r2.toString() << std::endl;
	if (sphere.rayIntersectP(r2))
	{
		std::cout << "Ray intersects with sphere" << std::endl;
	}
	else
	{
		std::cout << "Ray does not intersect with sphere" << std::endl;
	}
	
}

void eqSolverExercise()
{
    EqSolver solver;
    rootValues roots1;
	rootValues roots2;

    double A1 = 5, B1 = 6, C1 = 1;
	double A2 = 5, B2 = 2, C2 = 1;

    bool hasRoots1 = solver.rootQuadEq(A1, B1, C1, roots1);
	bool hasRoots2 = solver.rootQuadEq(A2, B2, C2, roots2);

	std::cout << "Equation --> A = " + std::to_string(A1) + " B = " + std::to_string(B1) + " C = " + std::to_string(C1) << std::endl;
    if(!hasRoots1)
    {
        std::cout << "Equation has no real roots!" << std::endl;
    }
    else
    {
		std::cout << "Equation has " + std::to_string(roots1.nValues) + " roots!" << std::endl;
		for (int i = 0; i < roots1.nValues; i++) 
		{
			std::cout << roots1.values[i] << std::endl;
		}
		
    }
	std::cout << "Equation --> A = " + std::to_string(A2) + " B = " + std::to_string(B2) + " C = " + std::to_string(C2) << std::endl;
	if (!hasRoots2)
	{
		std::cout << "Equation has no real roots!" << std::endl;
	}
	else
	{
		std::cout << "Equation has " + std::to_string(roots2.nValues) + " roots!" << std::endl;
		for (int i = 0; i < roots2.nValues; i++)
		{
			std::cout << roots2.values[i] << std::endl;
		}

	}
}

void raytrace()
{
    // Define the film (i.e., image) resolution
    size_t resX, resY;
    resX = 512;
    resY = 512;
    Film film(resX, resY);
	bool isCameraOrtho = false; //True for Orthographic camera and false for Perspective camera
	Ray r; //Ray

	//Generation of the sphere 
	Sphere sphere(1, Matrix4x4::translate(Vector3D(0, 0, 3)));

    /* ******************* */
    /* Orthographic Camera */
    /* ******************* */
    Matrix4x4 cameraToWorld; // By default, this gives an ID transform
                             // meaning that the camera space = world space
    OrtographicCamera camOrtho(cameraToWorld, film);
	

	
    /* ******************* */
    /* Perspective Camera */
    /* ******************* */
    double fovRadians = Utils::degreesToRadians(60);
    PerspectiveCamera camPersp(cameraToWorld, fovRadians, film);

	for (int x = 0; x < resX; x++) {
		for (int y = 0; y < resY; y++) {
			if (isCameraOrtho) {
				r = camOrtho.generateRay((x + 0.5) / float(resX), (y + 0.5) / float(resY));
			}
			else {
				r = camPersp.generateRay((x + 0.5) / float(resX), (y + 0.5) / float(resY));
			}
			
			if (sphere.rayIntersectP(r)) {
				film.setPixelValue(x, y, Vector3D(1, 0, 0));
			}
			else {
				film.setPixelValue(x, y, Vector3D(0, 0, 0));
			}
		}
	}
    // Save the final result to file
    film.save();
}

int main()
{
    std::string separator = "\n----------------------------------------------\n";

	std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // ASSIGNMENT 1
    //transformationsExercise();
    //normalTransformExercise();
    //paintingAnImageExercise();
    //filteringAnImageExercise();

    // ASSIGNMENT 2
    //eqSolverExercise();
    //completeSphereClassExercise();
    raytrace();

    std::cout << "\n\n" << std::endl;
    return 0;
}
