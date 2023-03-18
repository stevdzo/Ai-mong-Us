#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <math.h>
#include <vector>

#include "Vector2D.h"
#include "Utils.h"

class Matrix2D {
private:

    struct Matrix {
        double _11, _12, _13;
        double _21, _22, _23;
        double _31, _32, _33;

        Matrix() {
            _11 = 0.0; _12 = 0.0; _13 = 0.0;
            _21 = 0.0; _22 = 0.0; _23 = 0.0;
            _31 = 0.0; _32 = 0.0; _33 = 0.0;
        }
    };
    Matrix m_Matrix;

    inline void  matrix_multiply(Matrix& mIn);

public:

    Matrix2D() {

        identity();
    }

    inline void identity();

    inline void translate(double x, double y);

    inline void scale(double xScale, double yScale);

    inline void  rotate(double rotation);

    inline void  rotate(const Vector2D& fwd, const Vector2D& side);

    inline void transform_vector_2d(std::vector<Vector2D>& vPoints);

    inline void transform_vector_2d(Vector2D& vPoint);

    void _11(double val) { m_Matrix._11 = val; }
    void _12(double val) { m_Matrix._12 = val; }
    void _13(double val) { m_Matrix._13 = val; }

    void _21(double val) { m_Matrix._21 = val; }
    void _22(double val) { m_Matrix._22 = val; }
    void _23(double val) { m_Matrix._23 = val; }

    void _31(double val) { m_Matrix._31 = val; }
    void _32(double val) { m_Matrix._32 = val; }
    void _33(double val) { m_Matrix._33 = val; }

};

inline void Matrix2D::matrix_multiply(Matrix& mIn) {

    Matrix2D::Matrix mat_temp;

    mat_temp._11 = (m_Matrix._11 * mIn._11) + (m_Matrix._12 * mIn._21) + (m_Matrix._13 * mIn._31);
    mat_temp._12 = (m_Matrix._11 * mIn._12) + (m_Matrix._12 * mIn._22) + (m_Matrix._13 * mIn._32);
    mat_temp._13 = (m_Matrix._11 * mIn._13) + (m_Matrix._12 * mIn._23) + (m_Matrix._13 * mIn._33);

    mat_temp._21 = (m_Matrix._21 * mIn._11) + (m_Matrix._22 * mIn._21) + (m_Matrix._23 * mIn._31);
    mat_temp._22 = (m_Matrix._21 * mIn._12) + (m_Matrix._22 * mIn._22) + (m_Matrix._23 * mIn._32);
    mat_temp._23 = (m_Matrix._21 * mIn._13) + (m_Matrix._22 * mIn._23) + (m_Matrix._23 * mIn._33);

    mat_temp._31 = (m_Matrix._31 * mIn._11) + (m_Matrix._32 * mIn._21) + (m_Matrix._33 * mIn._31);
    mat_temp._32 = (m_Matrix._31 * mIn._12) + (m_Matrix._32 * mIn._22) + (m_Matrix._33 * mIn._32);
    mat_temp._33 = (m_Matrix._31 * mIn._13) + (m_Matrix._32 * mIn._23) + (m_Matrix._33 * mIn._33);

    m_Matrix = mat_temp;
}

inline void Matrix2D::transform_vector_2d(std::vector<Vector2D>& vPoint) {
    for (unsigned int i = 0; i < vPoint.size(); ++i)
    {
        double tempX = (m_Matrix._11 * vPoint[i].x) + (m_Matrix._21 * vPoint[i].y) + (m_Matrix._31);

        double tempY = (m_Matrix._12 * vPoint[i].x) + (m_Matrix._22 * vPoint[i].y) + (m_Matrix._32);

        vPoint[i].x = tempX;

        vPoint[i].y = tempY;

    }
}

inline void Matrix2D::transform_vector_2d(Vector2D& vPoint) {

    double tempX = (m_Matrix._11 * vPoint.x) + (m_Matrix._21 * vPoint.y) + (m_Matrix._31);

    double tempY = (m_Matrix._12 * vPoint.x) + (m_Matrix._22 * vPoint.y) + (m_Matrix._32);

    vPoint.x = tempX;

    vPoint.y = tempY;
}

inline void Matrix2D::identity() {
    m_Matrix._11 = 1; m_Matrix._12 = 0; m_Matrix._13 = 0;

    m_Matrix._21 = 0; m_Matrix._22 = 1; m_Matrix._23 = 0;

    m_Matrix._31 = 0; m_Matrix._32 = 0; m_Matrix._33 = 1;

}

inline void Matrix2D::translate(double x, double y) {
    Matrix mat;

    mat._11 = 1; mat._12 = 0; mat._13 = 0;

    mat._21 = 0; mat._22 = 1; mat._23 = 0;

    mat._31 = x;    mat._32 = y;    mat._33 = 1;

    matrix_multiply(mat);
}

inline void Matrix2D::scale(double xScale, double yScale) {
    Matrix2D::Matrix mat;

    mat._11 = xScale; mat._12 = 0; mat._13 = 0;

    mat._21 = 0; mat._22 = yScale; mat._23 = 0;

    mat._31 = 0; mat._32 = 0; mat._33 = 1;

    matrix_multiply(mat);
}

inline void Matrix2D::rotate(double rot) {
    Matrix2D::Matrix mat;

    double Sin = sin(rot);
    double Cos = cos(rot);

    mat._11 = Cos;  mat._12 = Sin; mat._13 = 0;

    mat._21 = -Sin; mat._22 = Cos; mat._23 = 0;

    mat._31 = 0; mat._32 = 0; mat._33 = 1;

    matrix_multiply(mat);
}

inline void Matrix2D::rotate(const Vector2D& fwd, const Vector2D& side) {
    Matrix2D::Matrix mat;

    mat._11 = fwd.x;  mat._12 = fwd.y; mat._13 = 0;

    mat._21 = side.x; mat._22 = side.y; mat._23 = 0;

    mat._31 = 0; mat._32 = 0; mat._33 = 1;

    matrix_multiply(mat);
}
#endif