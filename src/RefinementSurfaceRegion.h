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

#ifndef RefinementSurfaceRegion_h
#define RefinementSurfaceRegion_h 1

#include <vector>
#include <set>
#include <iostream>
#include "RefinementRegion.h"
#include "TriMesh.h"
#include "Octant.h"

using Clobscode::Point3D;
using std::vector;
using std::cout;
using std::set;
using Clobscode::RefinementRegion;
using Clobscode::TriMesh;
using Clobscode::Octant;
using Clobscode::GeometricTransform;


namespace Clobscode
{
	class RefinementSurfaceRegion : public RefinementRegion
	{
	public:
		
		// Construction / destruction
		RefinementSurfaceRegion(TriMesh &tm, const unsigned short &level);
		
		virtual ~RefinementSurfaceRegion();
        
        virtual GeometricTransform rotateWithinYou(TriMesh &input);
		
        virtual void rotate(GeometricTransform &gt);
        
        virtual vector<Point3D> &getPoints();
		
        virtual bool intersectsOctant(vector<MeshPoint> &points, Octant &oct);
        
    protected:
        
        virtual bool edgeIntersection(Point3D &oct_p1, Point3D &oct_p2,
                                      Point3D &seg_p1, Point3D &seg_p2);
        
        virtual unsigned int computePosition(const Point3D &p, const Point3D &pmin,
                                             const Point3D &pmax);

		
	protected:
		// Data
        TriMesh tm;
	};
	
	inline vector<Point3D> &RefinementSurfaceRegion::getPoints(){
		return tm.getPoints();
	}
	
}

#endif
