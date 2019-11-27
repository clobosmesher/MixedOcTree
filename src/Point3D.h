/*
 <Mix-mesher: region type. This program generates a mixed-elements 2D mesh>
 
 Copyright (C) <2013,2018>  <Claudio Lobos> All rights reserved.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/lgpl.txt>
 */
/**
* @file Point3D.h
* @author Claudio Lobos, Fabrice Jaillet
* @version 0.1
* @brief
**/

#ifndef Point3D_h
#define Point3D_h 1

#include <iostream>
#include <sstream>
#include <math.h>
#include <list>
#include <vector>

namespace Clobscode
{
	using namespace std;
	
	class Point3D{
		
	public:
        Point3D() : x(0.0),y(0.0),z(0.0) {}
		
        Point3D(double x, double y, double z) : x(x),y(y),z(z) {}
		
        virtual ~Point3D() {}
		
		virtual double X() const;
		
		virtual double Y() const;
		
		virtual double Z() const;
		
		virtual double &X();
		
		virtual double &Y();
		
		virtual double &Z();
		
        virtual Point3D& normalize();
		
        virtual double Norm() const;
		
        virtual Point3D cross(const Point3D &p) const;
		
		//cross product operator
		virtual Point3D operator^(const Point3D &p) const;
		
        virtual double dot(const Point3D &p) const;
		
		//dot product operator
//        virtual double operator*(Point3D p);
		
		virtual double operator*(const Point3D &p) const;
		
        virtual double distance(const Point3D &p) const;
		
		virtual double DistanceTo(const Point3D &p) const;
				
		virtual Point3D operator-(const Point3D &p) const;
		
		virtual Point3D operator-() const;
		
		virtual Point3D operator+(const Point3D &p) const;
		
		//virtual void operator+=(Point3D p2);
		
		virtual const Point3D &operator+=(const Point3D &p);
		
		virtual const Point3D &operator-=(const Point3D &p);
		
		virtual const Point3D &operator*=(double mul);
		
        virtual const Point3D &operator/=(double div);
		
		virtual Point3D operator/(double div) const;
		
		virtual Point3D operator*(double mul) const;
		
        //modification
		virtual double &operator[](int pos);
        //only access
		virtual double operator[](int pos) const;
		
        virtual Point3D& operator=(const Point3D& p);
				
		virtual void update(Point3D *Point3D);
		
		virtual void update(Point3D Point3D);
		
        virtual string print() const;
		
		virtual void xAxisRotation(double angle);
		
		virtual void yAxisRotation(double angle);
		
		virtual void zAxisRotation(double angle);
        
        virtual void translateRotate(const Point3D &t, double xangle,
                                     double yangle, double zangle);
        
        virtual void rotateTranslate(const Point3D &t, double xangle,
                                     double yangle, double zangle);
		
        friend std::ostream& operator<<(std::ostream& o,const Point3D &p);
		
		friend bool operator==(Point3D &p1,Point3D &p2);
		
		friend bool operator!=(Point3D &p1,Point3D &p2);
		
		friend Point3D operator/(double div, const Point3D &p);
		
		friend Point3D operator*(double mul, const Point3D &p);
		
	protected:
		
		double x,y,z;
		
	};
	
	inline double Point3D::X() const {
		return x;
	}
	
	inline double Point3D::Y() const {
		return y;
	}
	
	inline double Point3D::Z() const {
		return z;
	}
	
	inline double &Point3D::X() {
		return x;
	}
	
	inline double &Point3D::Y() {
		return y;
	}
	
	inline double &Point3D::Z() {
		return z;
	}

    inline Point3D& Point3D::normalize(){
        double nor=Norm();
        if(nor!=0){
            x/=nor;
            y/=nor;
            z/=nor;
        }
        else
            x=y=z=0;
        return *this;
    }

    inline double Point3D::Norm() const {
        return sqrt(x*x + y*y + z*z);
    }

    // why 2 distance/DistanceTo
    inline double Point3D::distance(const Point3D &p) const {
        return ((*this)-p).Norm();
    }

    inline double Point3D::DistanceTo(const Point3D &p) const{
        return ((*this)-p).Norm();
    }


    inline Point3D Point3D::cross(const Point3D &p) const {
        Point3D ret;
        ret[0]=y*p[2] - z*p[1];
        ret[1]=z*p[0] - x*p[2];
        ret[2]=x*p[1] - y*p[0];
        return ret;
    }

	inline Point3D Point3D::operator^(const Point3D &p) const {
		Point3D ret;
		ret[0]=y*p[2] - z*p[1];
		ret[1]=z*p[0] - x*p[2];
		ret[2]=x*p[1] - y*p[0];
		return ret;
	}

    inline double Point3D::dot(const Point3D &p) const{
        return x*p[0] + y*p[1] + z*p[2];
    }

	inline double Point3D::operator*(const Point3D &p) const {
		return x*p[0]+y*p[1]+z*p[2];
	}
	
	inline Point3D Point3D::operator/(double div) const {
		return Point3D(x/div,y/div,z/div);
	}
	
	inline Point3D Point3D::operator-(const Point3D &p) const {
		return Point3D(x-p[0],y-p[1],z-p[2]);
	}
	
	inline Point3D Point3D::operator-() const {
		return Point3D(-x,-y,-z);
	}

	inline Point3D Point3D::operator+(const Point3D &p) const {
		return Point3D(x+p[0],y+p[1],z+p[2]);
	}
	
	inline Point3D operator/(double div, const Point3D &p) {
		return p/div;
	}
	
	inline const Point3D &Point3D::operator+=(const Point3D &p) {
		x+=p[0]; y+=p[1]; z+=p[2];
		return *this;
	}
	
	inline const Point3D &Point3D::operator-=(const Point3D &p) {
		x-=p[0]; y-=p[1]; z-=p[2];
		return *this;
	}
	
	inline const Point3D &Point3D::operator*=(double mul) {
		x*=mul; y*=mul; z*=mul;
		return *this;
	}

    inline const Point3D &Point3D::operator/=(double div){
        x/=div;
        y/=div;
        z/=div;
        return *this;
    }

	inline Point3D Point3D::operator*(double mul) const {
		return Point3D (x*mul,y*mul,z*mul);
	}
	
	inline Point3D operator*(double mul, const Point3D&p){
		return Point3D(p.X()*mul,p.Y()*mul,p.Z()*mul);
	}
	
	inline void Point3D::update(Point3D *Point3D){
		x = (*Point3D)[0];
		y = (*Point3D)[1];
		z = (*Point3D)[2]; 
	}
	
	inline void Point3D::update(Point3D Point3D){
		x = Point3D[0];
		y = Point3D[1];
		z = Point3D[2]; 
	}

    inline double &Point3D::operator[](int pos){
        if(pos==0)
            return x;
        if(pos==1)
            return y;
        return z;
    }

	inline double Point3D::operator[](int pos) const{
		if(pos==0)
			return x;
		if(pos==1)
			return y;
		return z;
	}

    inline Point3D& Point3D::operator=(const Point3D& p) {
        x=p[0];
        y=p[1];
        z=p[2];
        return *this;
    }

	inline void Point3D::xAxisRotation(double angle){
        double oy = y, oz = z;
        double can = cos(angle), san = sin(angle);
		y = can*oy - san*oz;
        z = san*oy + can*oz;
    }
     
    inline void Point3D::yAxisRotation(double angle){
        double ox = x, oz = z;
        double can = cos(angle), san = sin(angle);
        x = can*ox + san*oz;
        z = can*oz - san*ox;
    }
     
    inline void Point3D::zAxisRotation(double angle){
        double ox = x, oy = y;
        double can = cos(angle), san = sin(angle);
		//Point3D pivot ( x, y, z);
		x = can*ox - san*oy;
        y = san*ox + can*oy;
     }
}    
#endif
