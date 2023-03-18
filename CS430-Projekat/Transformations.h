#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <vector>

#include "Vector2D.h"
#include "Matrix2D.h"
//#include "Transformations.h"

inline std::vector<Vector2D> world_transform(std::vector<Vector2D>& points,
    const Vector2D& pos,
    const Vector2D& forward,
    const Vector2D& side,
    const Vector2D& scale)
{
    std::vector<Vector2D> tran_vector_2d = points;

    Matrix2D mat_transform;

    if ((scale.x != 1.0) || (scale.y != 1.0))
    {
        mat_transform.scale(scale.x, scale.y);
    }

    mat_transform.rotate(forward, side);

    mat_transform.translate(pos.x, pos.y);

    mat_transform.transform_vector_2d(tran_vector_2d);

    return tran_vector_2d;
}

inline std::vector<Vector2D> world_transform(std::vector<Vector2D>& points,
    const Vector2D& pos,
    const Vector2D& forward,
    const Vector2D& side)
{
    std::vector<Vector2D> tran_vector_2d = points;

    Matrix2D mat_transform;

    mat_transform.rotate(forward, side);

    mat_transform.translate(pos.x, pos.y);

    mat_transform.transform_vector_2d(tran_vector_2d);

    return tran_vector_2d;
}

inline Vector2D point_to_world_space(const Vector2D& point,
    const Vector2D& AgentHeading,
    const Vector2D& AgentSide,
    const Vector2D& AgentPosition)
{
    Vector2D trans_point = point;

    Matrix2D mat_transform;

    mat_transform.rotate(AgentHeading, AgentSide);

    mat_transform.translate(AgentPosition.x, AgentPosition.y);

    mat_transform.transform_vector_2d(trans_point);

    return trans_point;
}

inline Vector2D vector_to_world_space(const Vector2D& vec,
    const Vector2D& AgentHeading,
    const Vector2D& AgentSide)
{
    Vector2D trans_vec = vec;

    Matrix2D mat_transform;

    mat_transform.rotate(AgentHeading, AgentSide);

    mat_transform.transform_vector_2d(trans_vec);

    return trans_vec;
}

inline Vector2D point_to_local_space(const Vector2D& point,
    const Vector2D& AgentHeading,
    const Vector2D& AgentSide,
    const Vector2D& AgentPosition)
{
    Vector2D trans_point = point;

    Matrix2D mat_transform;

    double Tx = -AgentPosition.multiply(AgentHeading);
    double Ty = -AgentPosition.multiply(AgentSide);

    mat_transform._11(AgentHeading.x); mat_transform._12(AgentSide.x);
    mat_transform._21(AgentHeading.y); mat_transform._22(AgentSide.y);
    mat_transform._31(Tx);             mat_transform._32(Ty);

    mat_transform.transform_vector_2d(trans_point);

    return trans_point;
}

inline Vector2D vector_to_local_space(const Vector2D& vec,
    const Vector2D& AgentHeading,
    const Vector2D& AgentSide)
{
    Vector2D trans_point = vec;

    Matrix2D mat_transform;

    mat_transform._11(AgentHeading.x); mat_transform._12(AgentSide.x);
    mat_transform._21(AgentHeading.y); mat_transform._22(AgentSide.y);

    mat_transform.transform_vector_2d(trans_point);

    return trans_point;
}

inline void vec_2d_rotate_around_origin(Vector2D& v, double ang)
{
    Matrix2D mat;

    mat.rotate(ang);

    mat.transform_vector_2d(v);
}
#endif