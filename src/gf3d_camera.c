#include "gfc_matrix.h"

#include <string.h>

Matrix4 gf3d_camera = {0};
Matrix4 gf3d_camera_init = { 0 };
float gf3d_camera_height = 0.5;
Vector3D gf3d_camera_pos;

void gf3d_camera_get_view(Matrix4 *view)
{
    if (!view)return;
    memcpy(view,gf3d_camera,sizeof(Matrix4));
}

void gf3d_camera_set_view(Matrix4 *view)
{
    if (!view)return;
    memcpy(gf3d_camera,view,sizeof(Matrix4));
}

void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
)
{
    gfc_matrix_view(
        gf3d_camera,
        position,
        target,
        up
    );
}

void gf3d_camera_set(Matrix4 start)
{
	gfc_matrix_copy(
		gf3d_camera_init,
		start);
}

void gf3d_camera_set_position(Vector3D position)
{
    gf3d_camera[0][3] = position.x;
    gf3d_camera[1][3] = position.y;
    gf3d_camera[2][3] = position.z;
}

Vector3D gf3d_camera_get_pos()
{
	gf3d_camera_pos.x = gf3d_camera[3][0];
	gf3d_camera_pos.y = gf3d_camera[3][1];
	gf3d_camera_pos.z = gf3d_camera[3][2];

	return gf3d_camera_pos;
}

/*
void gf3d_camera_move(Vector3D move)
{
    gf3d_camera[0][3] += move.x;
    gf3d_camera[1][3] += move.y;
    gf3d_camera[2][3] += move.z;
}
*/

void gf3d_camera_update(Vector3D pos, Vector3D rotate)
{
	float magicNumber = -5;
	float s = SDL_sinf(rotate.z - 2.35619);
	float c = SDL_cosf(rotate.z - 2.35619);
	Vector2D cpos;
	gfc_matrix_identity(gf3d_camera);

	cpos.x = ((magicNumber)* c) - ((magicNumber)* s);
	cpos.y = ((magicNumber)* s) + ((magicNumber)* c);

	/*
	if (gf3d_camera_height > -10 && rotate.y > 0)
	{
		gf3d_camera_height -= rotate.y;
	}
	else if (gf3d_camera_height < 25 && rotate.y < 0)
	{
		gf3d_camera_height -= rotate.y;
	}
	*/

	gf3d_camera_look_at(
		vector3d(cpos.x + pos.x, cpos.y + pos.y, pos.z + gf3d_camera_height),
		pos,
		vector3d(0, 0, 1));

	gf3d_vgraphics_setubo(gf3d_camera);
}

float gf3d_camera_get_height()
{
	return gf3d_camera_height;
}
/*eol@eof*/
