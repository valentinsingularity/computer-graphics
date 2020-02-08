#include <iostream>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>

using namespace std;



typedef struct{
	float x;
	float y;
	float z;
} Point;

typedef struct{
	Point P0;
	Point P;
} Ray;


//length of vector
float magnitude(Point);

//add two vectors
Point addv(Point, Point);

//substract two vectors
Point subv(Point, Point);

//scale vector
Point scalev(int, Point);

//given a line and the parameter T, find the point P on the line
Point getPoint(Ray, int);

//dot product
float DotP(Point, Point);

//cross product
Point CrossP(Point, Point);

//normalize vector
Point Normalize(Point);


float SolveQuadraticEquation(float, float, float);


class Sphere{
public:
	Point center;
	float radius;

	Sphere(){}

	Sphere(Point& c, float r){
		center = c;
		radius = r;
	}
};

class Plane{
public:
	float a;
	float b;
	float c;
	float d;
	//construct a plane given 3 points on it
	friend Plane Construct_From_Points(Point, Point, Point);
	//construct a plane given its normal vector and a point inside it
	friend Plane Construct_From_Normal(Point, Point);
	Point getNormal();//return the normal vector of the plane
};


//find the projection of a point onto a plane given by its normal vector
//and a point in the plane
Point Projection(Point, Point, Point);


//find the intersection of a line given by two points
//and a plane given by its normal vector and a point in the plane
Point LPIntersection(Point, Point, Point, Point);

class Frustum{//view frustum is made of 6 planes
public:
	Plane Front;
	Plane Back;
	Plane Left;
	Plane Right;
	Plane Top;
	Plane Bottom;
	Frustum(){}
	Frustum(Point, Point, Point, Point, Point, int);
};


class Scene{
public:
	Point v;//viewpoint

	//screen
	Point p1;
	Point p2;
	Point p3;
	Point p4;

	int range_of_sight;

	Sphere* Spheres;

	float width, height;

	Frustum f;//view frustum

	pair<Point, int> Intersec(Ray R);//return the first intersection
	//of a ray with the spheres

	bool isinFrustum(Point, Frustum);//check if point is inside
	//the view frustum

	vector<vector <pair <Point, int> > > Intersections(int m, int n);
	//2d array with the intersection points for each ray that goes
	//through the center of each pixel


	void renderImage(int m, int n);//create the desired image

	Scene(Point vp, Point p1, Point p2, Point p3, Point p4, int r, Sphere S[]){
		v = vp;
		this->p1 = p1; //Q: this->p1 = p1; ...
		this->p2 = p2;
		this->p3 = p3;
		this->p4 = p4;
		range_of_sight = r;
		Spheres = new Sphere[1];
		width = magnitude(subv(p1, p2));
		height = magnitude(subv(p1, p4));
		for (int i = 0; i<1; i++)
			Spheres[i] = S[i];
		f = Frustum(vp, p1, p2, p3, p4, r);
	}
	~Scene(){ delete[] Spheres; }

};




