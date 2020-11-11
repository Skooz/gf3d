#include "gfc_vector.h"

int collide_circle(Vector3D p1, float r1, Vector3D p2, float r2)
{
	// pythag
	float A = p1.x - p2.x;
	float B = p1.y - p2.y;
	float C = p1.z - p2.z;
	float hyp = SDL_sqrtf(A*A + B*B + C*C);

	//if ((r1 + r2) > hyp)
	if (vector3d_magnitude_squared(vector3d(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z)) < ((r2 + r1)*(r2 + r1)))
	{
		return 1;
	}

	return 0;

	/*
	if (vector2d_magnitude_squared(vector2d(p2.x - p1.x, p2.y - p1.y)) < ((r2 + r1)*(r2 + r1)) )
	{
		return 1;
	}
	return 0;
	*/
}

int collide_rect()
{

}

/*
int collide_rect(SDL_Rect r1, SDL_Rect r2)
{
	if (((r1.x + r1.w) < r2.x) ||
		((r2.x + r2.w) < r1.x) ||
		((r1.y + r1.h) < r2.y) ||
		((r2.y + r2.h) < r1.y))
	{
		return 0;
	}
	return 1;
}
*/