#ifndef CSE168_VECTOR4_H_INCLUDED
#define CSE168_VECTOR4_H_INCLUDED

#include <math.h>
#include <float.h>
#include <iostream>

#ifdef WIN32
#pragma warning(disable:4305) // disable useless warnings
#pragma warning(disable:4244)
#endif


class Vector4
{

public:
    float x, y, z, w;       // The x & y & z & w coordinates.


    Vector4() :
        x(0), y(0), z(0), w(0) {}

    Vector4(float s) :
        x(s), y(s), z(s), w(1) {}

    Vector4(float x, float y, float z, float w) :
        x(x), y(y), z(z), w(w) {}

    Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1) {}


    //! Assignment operator.
    /*!
        Assigns the values from \a a to this Vector4.
    */
    const Vector4 & operator=(const Vector4& a) {x = a.x; y = a.y; z = a.z; w = a.w; return *this;}
    
    //! Assignment operator.
    /*!
        Sets all components of this Vector4 to \a a.
    */
    const Vector4 & operator=(float a) {x = y = z = w = a; return *this;}
    //@}
    
    void set(float a) {x = y = z = w = a;}
    void set(float a, float b, float c) {x = a; y = b; z = c;}
    void set(float a, float b, float c, float d) {x = a; y = b; z = c; w = d;}
    void set(const Vector4 v) {x = v.x; y = v.y; z = v.z; w = v.w;}
    

    //! Access operator.        
    /*!
        Returns the ith component of the vector.
        \param i The component to return.
        \warning i must be either 0, 1, 2 or 3 in order to get expected results.
    */
    float & operator[](int i) {return(&x)[i];}
    
    //! Constant access operator.
    /*!
        Returns the ith component of a constant vector.
        \param i The component to return.
        \warning i must be either 0, 1, 2 or 3 in order to get expected results.
    */
    const float & operator[](int i) const {return(&x)[i];}



    //! Component-wise vector addition operator.
    Vector4 operator+(const Vector4& v) const
    {
        return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
    }
    
    //! Component-wise vector addition-assignment operator.
    const Vector4 & operator+=(const Vector4& v)
    {
        x += v.x; y += v.y; z += v.z; w += v.w; return *this;
    }

    //! Scalar addition-assignment operator.
    const Vector4 & operator+=(float a) {x += a; y += a; z += a; w += a; return *this;}
    //@}


    //! Component-wise vector subtraction operator.
    Vector4 operator-(const Vector4& v) const
    {
        return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
    }
    
    //! Component-wise vector subtraction-assignment operator.
    const Vector4 & operator-=(const Vector4& v)
    {
        x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;
    }
    
    //! Component-wise scalar subtraction assignment operator.
    const Vector4 & operator-=(float a) {x -= a; y -= a; z -= a; w -= w; return *this;}
    //@}


    //! Scalar multiplication operator.
    Vector4 operator*(float a) const {return Vector4(x * a, y * a, z * a, w * a);}
    
    //! Component-wise vector multiplication operator.
    Vector4 operator*(const Vector4& v) const
    {
        return Vector4(x * v.x, y * v.y, z * v.z, z * v.z);
    }
    
    //! Scalar multiplication-assignment operator.
    const Vector4 & operator*=(float a) {x *= a; y *= a; z *= a; w *= a; return *this;}
    
    //! Component-wise vector multiplication-assignment operator.
    const Vector4 & operator*=(const Vector4& v)
    {
        x *= v.x; y *= v.y; z *= v.z; z *= v.z; w *= v.w; return *this;
    }
    
    //! Negation operator.
    Vector4 operator-() const {return Vector4(-x, -y, -z, -w);}
    const Vector4 & negate() {x = -x; y = -y; z = -z; w = -w; return *this;}
    //@}


    //! Scalar division operator.
    Vector4 operator/(float a) const
    {
        float inv = float(1) / a;
        return Vector4(x * inv, y * inv, z * inv, w * inv);
    }
    
    //! Component-wise vector division operator.
    Vector4 operator/(const Vector4 & v) const
    {
        return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
    }
    
    //! Scalar division-assignment operator.
    const Vector4 & operator/=(float a)
    {
        float inv = float(1) / a;
        x *= inv; y *= inv; z *= inv; w *= inv;
        return *this;
    }
    
    //! Component-wise vector division-assignment operator.
    const Vector4 & operator/=(const Vector4 & v)
    {
        x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this;
    }


    //! Vector equivalence operator.
    /*!
        Tests to see if each component of \a v is equal to each component of
        this Vector4.
    */
    bool operator==(const Vector4 & v) const
    {
        return(v.x == x && v.y == y && v.z == z && v.w == w);
    }
    
    //! Vector difference operator.
    /*!
        Tests to see if any component is different between the two Vector4s.
    */
    bool operator!=(const Vector4 & v) const
    {
        return(v.x != x || v.y != y || v.z != z || v.w != w);
    }
    //@}


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
    const Vector4 & normalize()
    {
        return(*this /= length());
    }
    
    //! Return a normalized copy of the vector.
    Vector4 normalized() const
    {
        return(*this / length());
    }
};


//! Multiply a scalar by a Vector4.
inline Vector4
operator*(float s, const Vector4 & v)
{
    return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}


//! The dot product of two Vector4.
inline float
dot(const Vector4& a, const Vector4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


inline float
Vector4::length2() const
{
    return dot(*this, *this);
}


inline std::ostream &
operator<<(std::ostream& out, const Vector4& v)
{
    return out << v.x << " " << v.y << " " << v.z << " " << v.w;
}

#endif // CSE168_VECTOR4_H_INCLUDED
