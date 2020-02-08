#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "Objects.h"


float magnitude(Point P){
	return sqrt((P.x*P.x) + (P.y*P.y) + (P.z*P.z));
}

float SolveQuadraticEquation(float a, float b, float c){
	float discr = b*b - 4 * a*c;
	if (discr<0) return 999; //good enough in this case 
	else if (discr == 0) return -b / (2.0*a);
	else{
		float x1 = (-b + sqrt(discr)) / (2.0*a);
		float x2 = (-b - sqrt(discr)) / (2.0*a);
		if (x1<x2) return x1;
		else return x2;
	}
}



Point addv(Point p1, Point p2){
	Point p;
	p.x = p1.x + p2.x;
	p.y = p1.y + p2.y;
	p.z = p1.z + p2.z;
	return p;
}

Point subv(Point p1, Point p2){
	Point p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	p.z = p1.z - p2.z;
	return p;
}


Point scalev(int k, Point p){
	p.x *= k;
	p.y *= k;
	p.z *= k;
	return p;
}

Point getPoint(Ray R, int t){
	if (t == 999) //t=999 no intersection
		return R.P0; //signal no intersection
	Point res;
	res = addv(R.P0, scalev(t, subv(R.P, R.P0)));//P=P0 + tV
	return res;
}

float DotP(Point p1, Point p2){
	float res = 0;
	res += p1.x*p2.x;
	res += p1.y*p2.y;
	res += p1.z*p2.z;
	return res;
}


Point CrossP(Point A, Point B){
	Point res;
	res.x = A.y*B.z - A.z*B.y;
	res.y = A.z*B.x - A.x*B.z;
	res.z = A.x*B.y - A.y*B.x;
	return res;
}

Point Normalize(Point P){
	float length = magnitude(P);
	P.x /= length;
	P.y /= length;
	P.z /= length;
	return P;
}


Plane Construct_From_Normal(Point P, Point N){
	Plane Result;
	Result.a = N.x;
	Result.b = N.y;
	Result.c = N.z;
	Result.d = -DotP(P, N);
	return Result;
}


Plane Construct_From_Points(Point A, Point B, Point C){
	Point N = Normalize(CrossP(subv(B, A), subv(C, A)));
	return Construct_From_Normal(A, N);
}


Point Plane::getNormal(){
	Point N;
	N.x = a;
	N.y = b;
	N.z = c;
	return N;
}

Point Projection(Point N, Point A, Point P){
	float t = DotP(N, A) - DotP(N, P);
	Point res = addv(P, scalev(t, N));
	return res;
}


Point LPIntersection(Point P1, Point P2, Point N, Point O){
	float t = DotP(subv(O, P1), N) / DotP(subv(P2, P1), N);
	Point res = addv(P1, scalev(t, subv(P2, P1)));
	return res;
}

Frustum::Frustum(Point vp, Point p1, Point p2, Point p3, Point p4, int range){
	Front = Construct_From_Points(p1, p2, p3);//front plane is the screen plane

	/*constructing the back plane
	The back plane is parallel to the front plane therefore it will have
	the same normal vector */

	//N is the projection of the viewpoint on the front plane
	Point N = Projection(Front.getNormal(), p1, vp);

	//F is the projection of the viewpoint on the back plane
	//Because front and back plane are parallel F and N are 
	//on the same line. Therefore we can find F
	Point F = addv(vp, scalev(range, Normalize(subv(N, vp))));

	//We can now construct the back plane because we know its normal vector
	//and a point thats lying on the plane
	Back = Construct_From_Normal(F, Front.getNormal());



	//finding the points of the far-clipping plane
	//Points of intersections are between the lines that go from the viewpoint
	//through each corner of the screen(front plane) and the backplane
	Point F1 = LPIntersection(vp, p1, Back.getNormal(), F);
	Point F2 = LPIntersection(vp, p2, Back.getNormal(), F);
	Point F3 = LPIntersection(vp, p3, Back.getNormal(), F);
	Point F4 = LPIntersection(vp, p4, Back.getNormal(), F);


	//creating the other planes;
	Top = Construct_From_Points(p1, p2, F1);
	Bottom = Construct_From_Points(p3, p4, F3);
	Right = Construct_From_Points(p2, p3, F2);
	Left = Construct_From_Points(p1, p4, F4);


}


bool Scene::isinFrustum(Point P, Frustum A){
	//check if the point is inside the view frustum
	if ((DotP(P, A.Front.getNormal()) + A.Front.d) <= 0)
		return false;
	if ((DotP(P, A.Back.getNormal()) + A.Back.d) >= 0)
		return false;
	if ((DotP(P, A.Top.getNormal()) + A.Top.d) >= 0)
		return false;
	if ((DotP(P, A.Bottom.getNormal()) + A.Bottom.d) >= 0)
		return false;
	if ((DotP(P, A.Right.getNormal()) + A.Right.d) >= 0)
		return false;
	if ((DotP(P, A.Left.getNormal()) + A.Left.d) <= 0)
		return false;
	return true;
}

pair<Point, int> Scene::Intersec(Ray R){
	vector<float> T(3);
	//store the intersections of each ray with the three spheres
	//999 if no intersection
	float a = DotP(subv(R.P, R.P0), subv(R.P, R.P0));
	for (int i = 0; i < 3; i++){
		float b = 2 * DotP(subv(R.P0, this->Spheres[i].center), subv(R.P, R.P0));
		float c = DotP(subv(R.P0, this->Spheres[i].center),
			subv(R.P0, this->Spheres[i].center)) - pow(this->Spheres[i].radius, 2);
		T[i] = SolveQuadraticEquation(a, b, c);
	}

	int index = 0;
	for (int i = 1; i< 3; i++)
		if (T[i]<T[index])
			index = i;

	float t_min = T[index];


	//get the minimal t and check if the point is inside the view frustum
	Point A = getPoint(R, t_min);
	if (!isinFrustum(A, this->f))
		index = 3;

	//return a pair of the point and the integer that tells with
	//which sphere the ray intersected
	pair<Point, int> result = make_pair(A, index);
	return result;
}

vector< vector<pair<Point, int> > > Scene::Intersections(int m, int n){
	vector< vector<pair<Point, int> > > Result;
	pair<Point, int> initial = make_pair(this->v, 3);
	Result.resize(m, vector<pair<Point, int> >(n, initial));
	//float pix_width = this->width/(float)m;
	//float pix_height = this->height/(float)n;
	// Q:
	float pix_width = this->width / (float)n; // Q: n is the number of columns
	float pix_height = this->height / (float)m; // Q: m is the number of rows
	//std::cout << (this->p4).x << " " << (this->p4).y << endl;
	for (int i = 0; i<m; i++){ // i is the index of row;
		for (int j = 0; j<n; j++){ // j is the index of column;
			Point P;//center of each pixel
			//P.x = this->p1.x+pix_width*j+pix_width/2;
			//P.y = this->p1.y+pix_height*i+pix_height/2;
			//Q:
			P.x = this->p4.x + pix_width*(float)j + pix_width / 2.0f; // p4 = (-1, 1, 0);
			P.y = this->p4.y - pix_height*(float)i - pix_height / 2.0f;
			P.z = 0.0f;
			Ray R = { this->v, P };
			Result[i][j] = Intersec(R);
		}
	}

	return Result;
}


void Scene::renderImage(int m, int n){
	FILE* fp = fopen("image.ppm", "wb");
	vector< vector<pair<Point, int> > >  points = this->Intersections(m, n);
	(void)fprintf(fp, "P6\n%d %d\n255\n", n, m);
	for (int i = 0; i<m; i++){
		for (int j = 0; j<n; j++){
			static unsigned char color[3];
			//intersection with the sphere
			if (points[i][j].second == 0){

				//apply Phong illumination model
				I_Phong = I_amb * k_a + I_p1*min(1 / (r_p1*r_p1), 1)*(k_d1*max(DotP(n_p1, l_p1) / (magnitude(n_p1)*magnitude(l_p1)), 0) + k_s1*max(DotP(r_p1, v_p1) / (magnitude(r_p1)*magnitude(v_p1)), 0))
					+ I_p2*min(1 / (r_p2*r_p2), 1)*(k_d2*max(DotP(n_p2, l_p2) / (magnitude(n_p2)*magnitude(l_p2)), 0) + k_s2*max(DotP(r_p2, v_p2) / (magnitude(r_p2)*magnitude(v_p2)), 0))

				color[0] = (unsigned char)floor(255.0*I_Phong_r);
				color[1] = (unsigned char)floor(255.0*I_Phong_g);
				color[2] = (unsigned char)floor(255.0*I_Phong_b);


			}

			//no intersection
			else {
				color[0] = color[1] = color[2] = 0;
			}

			fwrite(color, 1, 3, fp);


		}
	}
	fclose(fp);
}