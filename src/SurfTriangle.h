/*
 <Mix-mesher: region type. This program generates a mixed-elements mesh>
 
 Copyright (C) <2013,2017>  <Claudio Lobos>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/gpl.txt>
 */

#ifndef SurfTriangle_h
#define SurfTriangle_h 1

#include "Point3D.h"
#include <vector>
#include <iostream>

using Clobscode::Point3D;
using std::vector;
using std::cout;

namespace SurfMesh 
{
	class SurfTriangle
	{
	public:
		
		// Construction / destruction
		SurfTriangle(vector<unsigned int> &fpts);
		
		virtual ~SurfTriangle();
		
		virtual void computeNormal(vector<Point3D> &pts);
		
		virtual Point3D computeNormalAtNode(unsigned int nidx, 
											vector<Point3D> &pts);
		
		virtual Point3D projection(const Point3D &p1, 
								   const Point3D &p2);
		
		virtual vector<unsigned int> &getPoints();
		
		virtual Point3D &getNormal();
		
		virtual Point3D getNormalizedNormal();
		
		virtual Point3D getSemiNormalizedNormal();
		
		virtual unsigned int &operator[](unsigned int pos);
		
		virtual unsigned int operator[](unsigned int pos) const;
		
		virtual bool segmentIntersection(vector<Point3D> &pts,
										 const Point3D &ep1,
										 const Point3D &ep2);
	
		//computes the distant of pPoint to the plane formed 
		//by this triangle.
		virtual double distance(const Point3D &pPoint);
		
		
	protected:
		// Data
		vector<unsigned int> mIdxV;
		Point3D mTriangleNormal, onepoint;
		
	};
	
	inline vector<unsigned int> &SurfTriangle::getPoints(){
		return mIdxV;
	}
	
	inline Point3D &SurfTriangle::getNormal(){
		return mTriangleNormal;
	}	
	
	inline Point3D SurfTriangle::getNormalizedNormal(){
		Point3D normal = mTriangleNormal/(mTriangleNormal.Norm());
		return normal;
	}
	
	inline Point3D SurfTriangle::getSemiNormalizedNormal(){
		Point3D normal = mTriangleNormal/10000;
		return normal;
	}
}

#endif
