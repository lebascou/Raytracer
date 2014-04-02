#ifndef CSE168_VECTOR3_H_INCLUDED
#define CSE168_VECTOR3_H_INCLUDED

#include <cmath>
#include <math.h>
#include <float.h>
#include <iostream>

#define EPSILON 0.0001f
#define INF std::numeric_limits<float>::infinity()

#ifdef WIN32
#pragma warning(disable:4305) // disable useless warnings
#pragma warning(disable:4244)
#endif

class Vector3
{

public:
    float x, y, z;      // The x & y & z coordinates.

    Vector3() :
        x(0), y(0), z(0) {}

    Vector3(float s) :
        x(s), y(s), z(s) {}

    Vector3(float xVal, float yVal, float zVal) :
        x(xVal), y(yVal), z(zVal) {}

    explicit Vector3(float v[3]) : x(v[0]), y(v[1]), z(v[2]) {}

    //! Assignment operator.
    /*!
        Assigns the values from \a a to this Vec3.
    */
    const Vector3 & operator=(const Vector3& a) {x = a.x; y = a.y; z = a.z; return *this;}
    
    //! Assignment operator.
    /*!
        Sets all components of this Vec3 to \a a.
    */
    const Vector3 & operator=(float a) {x = y = z = a; return *this;}

    void set(float a) {x = y = z = a;}
    void set(float a, float b, float c) {x = a; y = b; z = c;}
    void set(const Vector3 & v) {x = v.x; y = v.y; z = v.z;}
    
    
    //! Access operator.        
    /*!
        Returns the ith component of the vector.
        \param i The component to return.
        \warning i must be either 0, 1, or 2 in order to get expected results.
    */
    float & operator[](int i) {return (&x)[i];}
    
    //! Constant access operator.
    /*!
        Returns the ith component of a constant vector.
        \param i The component to return.
        \warning i must be either 0, 1, or 2 in order to get expected results.
    */
    const float & operator[](int i) const {return (&x)[i];}


    //! Component-wise vector addition operator.
    Vector3 operator+(const Vector3& v) const
    {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    
    //! Component-wise vector addition-assignment operator.
    const Vector3 & operator+=(const Vector3& v)
    {
        x += v.x; y += v.y; z += v.z; return *this;
    }

    //! Scalar addition-assignment operator.
    const Vector3 & operator+=(float a) {x += a; y += a; z += a; return *this;}


    //! Component-wise vector subtraction operator.
    Vector3 operator-(const Vector3& v) const
    {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    //! Component-wise vector subtraction-assignment operator.
    const Vector3 & operator-=(const Vector3& v)
    {
        x -= v.x; y -= v.y; z -= v.z; return *this;
    }
    
    //! Component-wise scalar subtraction assignment operator.
    const Vector3 & operator-=(float a) {x -= a; y -= a; z -= a; return *this;}


    //! Scalar multiplication operator.
    Vector3 operator*(float a) const {return Vector3(x * a, y * a, z * a);}
    
    //! Component-wise vector multiplication operator.
    Vector3 operator*(const Vector3& v) const
    {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }
    
    //! Scalar multiplication-assignment operator.
    const Vector3 & operator*=(float a) {x *= a; y *= a; z *= a; return *this;}
    
    //! Component-wise vector multiplication-assignment operator.
    const Vector3 & operator*=(const Vector3& v)
    {
        x *= v.x; y *= v.y; z *= v.z; return *this;
    }
    
    //! Negation operator.
    Vector3 operator-() const {return Vector3(-x, -y, -z);}
    const Vector3 & negate() {x = -x; y = -y; z = -z; return *this;}


    //! Scalar division operator.
    Vector3 operator/(float a) const
    {
        float inv = float(1) / a;
        return Vector3(x * inv, y * inv, z * inv);
    }
    
    //! Component-wise vector division operator.
    Vector3 operator/(const Vector3 & v) const
    {
        return Vector3(x / v.x, y / v.y, z / v.z);
    }
    
    //! Scalar division-assignment operator.
    const Vector3 & operator/=(float a)
    {
        float inv = float(1) / a;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }
    
    //! Component-wise vector division-assignment operator.
    const Vector3 & operator/=(const Vector3 & v)
    {
        x /= v.x; y /= v.y; z /= v.z; return *this;
    }


    //! Vector equivalence operator.
    /*!
        Tests to see if each component of \a v is equal to each component of
        this Vector3.
    */
    bool operator==(const Vector3 & v) const
    {
        return(v.x == x && v.y == y && v.z == z);
    }
    
    //! Vector difference operator.
    /*!
        Tests to see if any component is different between the two Vec3s.
    */
    bool operator!=(const Vector3 & v) const
    {
        return(v.x != x || v.y != y || v.z != z);
    }


    //! Length<sup>2</sup>.
    /*!
        Returns the geometric length<sup>2</sup> of the vector.
    */
    float length2() const;
    
    //! Length.
    /*!
        Returns the geometric length of the vector.
    */
    float length() const {return sqrtf(length2());}
    
    //! Normalizes the vector and return its length.
    /*!
        Scales each component of the vector in order to get unit
        length without changing direction.
    
        \return The length of the vector prior to normalization.
    */
    float unitize()
    {
        float l = length();
        *this /= l;
        return l;
    }
    
    //! Normalize a vector and return a reference to it.
    /*!
        Scales each component of the vector in order to get unit
        length without changing direction.
    
        \return A reference to the vector.
    */
    const Vector3 & normalize()
    {
        return (*this /= length());
    }
    
    //! Return a normalized copy of the vector.
    Vector3 normalized() const
    {
        return( *this / length());
    }
    
    //! Return a rotated copy of the vector
    Vector3 rotated(float theta, const Vector3 & w) const;
    
    //! Rotate this vector about another vector, w, by theta radians.
    const Vector3 & rotate(float theta, const Vector3 & w)
    {
	return *this = rotated(theta, w);
    }

    float dot(Vector3 const &b) const { return x * b.x + y * b.y + z * b.z; }
    Vector3 cross(Vector3 const &b) const
    {
        return Vector3(y * b.z - z * b.y,
               z * b.x - x * b.z,
               x * b.y - y * b.x);

    }

    Vector3 min(Vector3 const &b) const {return Vector3(std::min(x, b.x), std::min(y, b.y), std::min(z, b.z));}
    Vector3 max(Vector3 const &b) const {return Vector3(std::max(x, b.x), std::max(y, b.y), std::max(z, b.z));}
    float max() const {return std::max(x, std::max(y, z));}
    float min() const {return std::min(x, std::min(y, z));}
    float sum() const {return x + y + z;}

    const float *array() const {return &x;}

    Vector3 abs() const {return Vector3(std::abs(x), std::abs(y), std::abs(z));}
    bool operator>(Vector3 const &v) const {return x > v.x && y > v.y && z > v.z;}
};


//! Multiply a scalar by a Vec3.
inline Vector3
operator*(float s, const Vector3& v)
{
    return Vector3(v.x * s, v.y * s, v.z * s);
}


//! The dot product of two Vec3s.
inline float
dot(const Vector3 & a, const Vector3 & b)
{
    return a.dot(b);
}


//! The cross product of two Vec3s.
inline Vector3
cross(const Vector3 & a, const Vector3 & b)
{
    return a.cross(b);
}


inline float
Vector3::length2() const
{
    return dot(*this);
}


//! Return a rotated copy of the vector
inline Vector3
Vector3::rotated(float theta, const Vector3 & w) const
{
    float c = cosf(theta);
    float s = sinf(theta);

    Vector3 v0 = dot(w) * w;
    Vector3 v1 = *this - v0;
    Vector3 v2 = w.cross(v1);

    return v0 + c * v1 + s * v2;
}


inline std::ostream &
operator<<(std::ostream& out, const Vector3& v)
{
    return out << v.x << " " << v.y << " " << v.z ;
}

#endif // CSE168_VECTOR3_H_INCLUDED
