//
//  geometry.h
//  nicoPBRT
//
//  Created by Lito Nicolai on 7/22/13.
//  Copyright (c) 2013 Lito Nicolai. All rights reserved.
//

#ifndef __nicoPBRT__geometry__
#define __nicoPBRT__geometry__
#include "pbrt.h"
#include <math.h>

class Vector {
public:
    float x, y, z;
    
    Vector() {x = y = z = 0.f;}
    
    //check for NaNs
    Vector(float xx, float yy, float zz): x(xx), y(yy), z(zz){
        Assert(!HasNaNs());
    }
    
    bool HasNaNs() const {
        return isnan(x) || isnan(y) || isnan(z);
    }
    
    // Addition
    Vector operator+(const Vector &v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }
    Vector& operator+(const Vector &v) { // why not const return? also, wherefore reference?
            x += v.x; y += v.y; z +=v.z;
            return *this;
    }
    // Scalar Multiplication
    Vector operator*(float f) const{
            return Vector (f*x, f*y, f*z);
    }
    Vector &operator*=(float f) {
        x*=f; y*=f; z*=f;
        return *this;
    }

    // Divison
    Vector operator/(float f) const{
        Assert(f != 0);
        float inv = 1.f/f;
        return Vector(x*inv, y*inv, z*inv);
    }
    // Negation
    Vector operator-() const {
        return Vector (-x, -y, -z);
    }
    
    Vector operator[](int i) const {
        Assert(i >= 0 && i <= 2);
        return (&x)[i];
    }
        
        
    // Length
    float LengthSquared() const {
        return x*x + y*y + z*z;
    }
    
    float Length() const {
        return sqrtf(LengthSquared());
    }
    // Normal conversion
    explicit Vector(const Normal &n);
};




class Point {
public:
    float x, y, z;
    
    Point() {
        x = y = z = 0.f;
    }
    
    Point(float xx, float yy, float zz)
    : x(xx), y(yy), z(zz) {
    }
    
    Point operator+(const Vector &v) const {
        return Point(x + v.x, y + v.y, z + v.z);
    }
    
    Point &operator+=(const Vector &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    
    Vector operator-(const Point &p) const {
        return Vector(x + p.x, y + p.y, z + p.z);
    }
    
    Point operator-(const Vector &v) const {
        return Point(x + v.x, y + v.y, z + v.z);
    }
    
    Point &operator-=(const Vector &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
};

class Normal {
public:
    float x, y, z;
    Normal() {
        x = y = z = 0;
    }
    Normal( float xx, float yy, float zz)
    : x(xx), y(yy), z(zz){}
    
    explicit Normal(const Vector &v):
    x(v.x), y(v.y), z(v.z){}
    
    Normal operator+(const Normal &n) const {
        return Normal (x + n.x, y + n.y, z + n.z);
    }
    // Scalar Multiplication
    Normal operator*(float f) const{
        return Normal (f*x, f*y, f*z);
    }
    Normal &operator*=(float f) {
        x*=f; y*=f; z*=f;
        return *this;
    }
    
    // Divison
    Normal operator/(float f) const{
        Assert(f != 0);
        float inv = 1.f/f;
        return Normal(x*inv, y*inv, z*inv);
    }
    // Negation
    Normal operator-() const {
        return Normal (-x, -y, -z);
    }
    
    Normal operator[](int i) const {
        Assert(i >= 0 && i <= 2);
        return (&x)[i];
    }
    
    // Length
    float LengthSquared() const {
        return x*x + y*y + z*z;
    }
    
    float Length() const {
        return sqrtf(LengthSquared());
    }

}

class Ray {
public:
    Point o;
    Vector d;
    mutable float mint, maxt;
    float time;
    int depth;
    
    Ray(): mint(0.f), maxt(INFINITY), time(0.f), depth(0) {}
    Ray const Point &origin, const Vector &direction, float start, float end = INFINITY, float t = 0.f, int d = 0)
    : o(origin), d(direction), mint(start), maxt(end), time(t),depth(d) {}
    
    // Ray that inherits properties from a parent ray
    Ray const Point &origin, const Vector &direction, const Ray &paretn, float start, float end = INFINITY)
    : o(origin), d(direction), mint(start), maxt(end), time(parent.time), depth(parent.depth + 1) {}
    
Point operator()(float t) const {
    return o + d * t;
}

}

class RayDifferential : public Ray { // Used in antialiasing
public:
    bool hasDifferentials
    Point rxOrigin, ryOrigin;
    Vector rxDirection, ryDirection;
    
    
    RayDifferential() { hasDifferentials = false;}
    
    RayDifferential(const Point &org, const Vector &dir, float start,
                    float end = INFINITY, float t = 0.f, int d = 0)
    : Ray(org, dir, start, end, t, d) {
        hasDifferentials = false;
    }
    // Parented differential
    RayDifferential(const Point &org, const Vector &dir, const Ray &parent,
                    float start, float end = INFINITY)
    : Ray(org, dir, start, end, parent.time, parent.depth+1) {
        hasDifferentials = false;
    }
    explicit RayDifferential( const Ray &ray) : Ray(ray) {
        hasDifferentials = false;
    }
    /* this adjusts for actual sample spacing- cameras assume 1px spacing automatically */
    void ScaleDifferentials(float s) { // Where s is sample spacing
        rxOrigin = o + (rxOrigin - o) * s;
        ryOrigin = o + (ryOrigin - o) * s;
        rxDirection = d + (rxDirection - d) * s;
        ryDirection = d + (ryDirection - d) * s;
    }
    
}

class BBox {
public:
    Point pMin, pMax;
    
    BBox(){
        pMin = Point(INFINITY, INFINITY, INFINITY)
        pMax = Point(-INFINITY, -INFINITY, -INFINITY)
    }
    BBox(const Point &p) : pMin(p), pMax(p){}
    
    BBox(const Point &p1, const Point &p2){
        pMin = Point(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
        pMax = Point(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
    }
    BBox Union(const BBox &b, const Point &p) {
        BBox ret = b;
        ret.pMin.x = min(b.pMin.x, p.x);
        ret.pMin.y = min(b.pMin.y, p.y);
        ret.pMin.z = min(b.pMin.z, p.z);
        ret.pMax.x = max(b.pMax.x, p.x);
        ret.pMax.y = max(b.pMax.y, p.y);
        ret.pMax.z = max(b.pMax.z, p.z);
        return ret;
    }
    
    friend BBox Union(const BBox &b, const BBox &b2)
    
    bool Overlaps(const BBox &b) const {
        bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x)
        bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y)
        bool y = (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z)
        return (x && y && z);
    }
    
    bool Inside(const Point &pt) const {
        return (pt.x >= pMin.x && pt.x <= pMax.x &&
                pt.y >= pMin.y && pt.y <= pMax.y &&
                pt.z >= pMin.z && pt.z <= pMax.z);
    }
    
    void Expand(float delta) {
        pMin -= Vector(delta, delta, delta);
        pMax += Vector(delta, delta, delta);
    }
    
    float SurfaceArea() const {
        Vector d = pMax - pMin;
        return 2.f * (d.x*d.y + d.x*d.z + d.y*d.z);
    }
    
    float Volume() const {
        Vector d = pMax - pMin;
        return d.x * d.y * d.z;
    }
    
    int MaximumExtent() const {
        Vector d = pMax - pMin;
        if (d.x > d.y && d.x > d.z) {
            return 0;
        }
        else if (d.y > d.z) {
            return 1;
        }
        else {
            return 2;
        }
    }

    const Point &operator[](int i) const;
    Point &operator[](int i); //why two of them
    
    Point Lerp(float tx, float ty, float tx) const {
        return Point(::Lerp(tx, pMin.x, pMax.x), Lerp(ty, pMin.y, pMax.y), ::Lerp(tz, pMin.z, pMax.z));
    }
    
    Vector Offset(const Point &p) const{
        return Vector((p.x - pMin.x) / (pMax.x - pMin.x),
                      (p.y - pMin.y) / (pMax.y - pMin.y),
                      (p.z - pMin.z) / (pMax.z - pMin.z));
    }
    
    void BBox::BoundingSphere(Point *c, float *rad) const {
        *c = .5f * pMin + .5f * pMax;
        *rad = Inside(*c) ? Distance(*c, pMax) : 0.f; //why are pointers used here
    }
}

class Transform {
public:
    Transform() {}
    Transform(const float mat[4][4]){
        m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                      mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                      mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                      mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        mInv = Inverse(m);
    }
    
    Transform(const Matrix4x4 &mat) :
    m(mat), mInv(Inverse(mat)){}
    
    Transform (const Matrix4x4 &mat, const Matrix4x4 &matinv):
    m(mat), mInv(matinv){}
    
    friend Transform Inverse(const Transform &t) {
        return Transform(t.mInv, t.m);
    }
    
    Transform Translate(const Vector &delta) {
        Matrix4x4 m(1, 0, 0, delta.x,
                    0, 1, 0, delta.y,
                    0, 0, 1, delta.z,
                    0,0,0, 1);
        Matrix4x4 minv(1, 0, 0, -delta.x,
                       0, 1, 0, -delta.y,
                       0, 0, 1, -delta.z,
                       0, 0, 0, 1);
        return Transform(m, minv);
    }
    
    Transform Scale(float x, float y, float z) {
        Matrix4x4 m(x, 0, 0, 0,
                    0, y, 0, 0,
                    0, 0, z, 0,
                    0, 0, 0, 1);
        Matrix4x4 minv(1.f/x,     0,
                       0,     1.f/y,
                       0,         0,
                       0,     0,
                       0,     0,
                       1.f/z, 0,
                       0,         0,     0,     1);
        return Transform(m, minv);
    }
    
    ⟨Transform Method Definitions⟩ +≡ Transform RotateX(float angle) {
        float sin_t = sinf(Radians(angle));
        float cos_t = cosf(Radians(angle));
        Matrix4x4 m(1,     0,      0, 0,
                    0, cos_t, -sin_t, 0,
                    0, sin_t,  cos_t, 0,
                    0,     0,      0, 1);
        return Transform(m, Transpose(m));
    }
    
private:
    Matrix4x4 m, mInv;
}


/* Vector Inline Operators */

inline Vector operator*(float f, const Vector &v) {
    return v*f;
}

// Dot Product
inline float Dot(const Vector &v1, const Vector &v2){
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
inline float AbsDot(const Vector &v1, const Vector &v2){
    return fabsf(Dot(v1,v2));
}

// Cross Product
inline Vector Cross(const Vector &v1, const Vector &v2) {
    return Vector((v1.y * v2.z) - (v1.z * v2.y),
                  (v1.z * v2.x) - (v1.x * v2.z),
                  (v1.x * v2.y) - (v1.y * v2.x));
}

// Normalize
inline Vector Normalize(const Vector &v) {
    return v / v.Length();
}
    
inline Normal Normalize(const Normal &n) {
    return n / n.Length();
}

inline Vector::Vector(const Normal &n):
x(n.x), y(n.y), z(n.z){}


// Construct Coordinate System
inline void CoordinateSystem(const Vector &v1, const Vector &v2, const Vector &v3) {
    if (fabsf(v1.x) > fabs(v1.y)) {
        float invlen = 1.f / sqrtf(v1.x*v1.x + v1.z * v1.z);
        *v2 = Vector(-v1.z * invlen, 0.f, v1.x*invlen);
    }
    else {
        float invlen = 1.f / sqrtf(v1.y*v1.y + v1.z*v1.z);
        *v2 = Vector(0.f, -v1.z * invlen, v1.y*invlen);
    }
    *v3 = Cross(v1, *v2);
}

/* Point Inline Operators */

inline float Distance(const Point &p1, const Point &p2) {
    return (p1-p2).Length();
}
inline float DistanceSquared(const Point &p1, const Point &p2) {
    return (p1-p2).LengthSquared();
}

/* Normal Inline Operators */

inline Normal Faceforward(const normal &n, const vector &v) {
    return Dot(n, v) < 0.f ? -n : n; // if dot less than 0, -n; else n
}




#endif /* defined(__nicoPBRT__geometry__) */
