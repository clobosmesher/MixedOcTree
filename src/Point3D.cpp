/*
 <Mix-mesher: region type. This program generates a mixed-elements 2D mesh>

 Copyright (C) <2013,2018>  <Claudio Lobos> All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU  Lesser General Public License as published by
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
* @file Point3D.cpp
* @author Claudio Lobos, Fabrice Jaillet
* @version 0.1
* @brief
**/

#include "Point3D.h"

namespace Clobscode
{

    string Point3D::print() const {
        ostringstream o;
        string out;

        o << x << " ";
        o << y << " ";
        o << z;

        out = o.str();
        return out;
    }

    std::ostream& operator<<(std::ostream& o, const Point3D &p){
        o << p.print().c_str();
        return o;
    }

    void Point3D::translateRotate(const Point3D &t, double xangle,
                                  double yangle, double zangle) {
        x-=t[0];
        y-=t[1];
        z-=t[2];

        if (xangle!=0) {
            xAxisRotation(xangle);
        }
        if (yangle!=0) {
            yAxisRotation(yangle);
        }
        if (zangle!=0) {
            zAxisRotation(zangle);
        }

    }

    void Point3D::rotateTranslate(const Point3D &t, double xangle,
                                  double yangle, double zangle) {

        if (zangle!=0) {
            zAxisRotation(-zangle);
        }
        if (yangle!=0) {
            yAxisRotation(-yangle);
        }
        if (xangle!=0) {
            xAxisRotation(-xangle);
        }

        x+=t[0];
        y+=t[1];
        z+=t[2];
    }


//	Point3D::Point3D(){
//        x=y=z=0.0;
//	}

//    Point3D::Point3D(double x, double y, double z) : x(x),y(y),z(z) {

//    }

//    Point3D::~Point3D(){

//    }

//    Point3D Point3D::cross(const Point3D &p){
//		Point3D ret;
//		ret[0]=y*p[2] - z*p[1];
//		ret[1]=z*p[0] - x*p[2];
//		ret[2]=x*p[1] - y*p[0];
//		return ret;
//	}

//	//cross product operator
//    Point3D Point3D::operator^(Point3D p){
//		Point3D ret;
//		ret[0]=y*p[2] - z*p[1];
//		ret[1]=z*p[0] - x*p[2];
//		ret[2]=x*p[1] - y*p[0];
//		return ret;
//	}

//    double Point3D::dot(Point3D p){
//		return x*p[0] + y*p[1] + z*p[2];
//	}

//    double Point3D::operator*(Point3D p){
//		return x*p[0] + y*p[1] + z*p[2];
//	}

//    double Point3D::distance(Point3D p){
//		return ((*this)-p).Norm();
//	}

//    double Point3D::DistanceTo(const Point3D &p){
//		return ((*this)-p).Norm();
//	}

//	double Point3D::DistanceTo(const Point3D &p) const{
//		return ((*this)-p).Norm();
//	}

//	void Point3D::normalize(){
//		double nor=Norm();
//		if(nor!=0){
//			x/=nor;
//			y/=nor;
//			z/=nor;
//		}
//		else
//			x=y=z=0;
//	}

//    double Point3D::Norm(){
//		return sqrt(x*x + y*y + z*z);
//	}

//    string Point3D::print(){
//		ostringstream o;
//		string out;

//		o << x << " ";
//		o << y << " ";
//		o << z;

//		out = o.str();
//		return out;
//	}

//    std::ostream& operator<<(std::ostream& o,Point3D &p){
//		o << p.print().c_str();
//		return o;
//	}

//	bool operator==(Point3D &p1,Point3D &p2){
//		Point3D p=p1-p2;
//		double dx=p[0],dy=p[1],dz=p[2];
//		double epsilon=1E-6;
//		if(dx<0) dx*=-1;
//		if(dy<0) dy*=-1;
//		if(dz<0) dz*=-1;
//		if(dx>epsilon)
//			return false;
//		if(dy>epsilon)
//			return false;
//		if(dz>epsilon)
//			return false;
//		return true;
//	}

//	bool operator!=(Point3D &p1,Point3D &p2){
//		return !(p1==p2);
//	}

//	Point3D Point3D::operator-(){
//		Point3D p;
//		p[0]=-x;
//		p[1]=-y;
//		p[2]=-z;
//		return p;
//	}

//	Point3D Point3D::operator-(Point3D p2){
//		Point3D p;
//		p[0]=x-p2[0];
//		p[1]=y-p2[1];
//		p[2]=z-p2[2];
//		return p;
//	}

//	Point3D Point3D::operator+(Point3D p2){
//		Point3D p;
//		p[0]=x+p2[0];
//		p[1]=y+p2[1];
//		p[2]=z+p2[2];
//		return p;
//	}

//	void Point3D::operator/=(double div){
//		x/=div;
//		y/=div;
//		z/=div;
//	}

//	/*void Point3D::operator+=(Point3D p2){
//		x+=p2[0];
//		y+=p2[1];
//		z+=p2[2];
//	}*/

//    //FJA ?
//    Point3D& Point3D::operator=(const Point3D& p) {
//        x=p[0];
//        y=p[1];
//        z=p[2];
//        return *this;
//    }

////    void Point3D::operator=(Point3D p){
////		x=p[0];
////		y=p[1];
////		z=p[2];
////	}

//	double &Point3D::operator[](int pos){
//		if(pos==0)
//			return x;
//		if(pos==1)
//			return y;
//		return z;
//	}

//	Point3D Point3D::operator*(double escalar){
//		Point3D ret;
//		ret[0] = x*escalar;
//		ret[1] = y*escalar;
//		ret[2] = z*escalar;
//		return ret;
//	}

//    void Point3D::translateRotate(const Point3D &t, double xangle,
//                                  double yangle, double zangle) {
//        x-=t[0];
//        y-=t[1];
//        z-=t[2];

//        if (xangle!=0) {
//            xAxisRotation(xangle);
//        }
//        if (yangle!=0) {
//            yAxisRotation(yangle);
//        }
//        if (zangle!=0) {
//            zAxisRotation(zangle);
//        }

//    }

//    void Point3D::rotateTranslate(const Point3D &t, double xangle,
//                                  double yangle, double zangle) {

//        if (zangle!=0) {
//            zAxisRotation(-zangle);
//        }
//        if (yangle!=0) {
//            yAxisRotation(-yangle);
//        }
//        if (xangle!=0) {
//            xAxisRotation(-xangle);
//        }

//        x+=t[0];
//        y+=t[1];
//        z+=t[2];
//    }

}
