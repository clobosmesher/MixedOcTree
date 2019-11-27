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

#include "RefinementCubeRegion.h"

namespace Clobscode
{
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	RefinementCubeRegion::RefinementCubeRegion(Point3D &p1, Point3D &p2,
									   const unsigned short &level)
	{
		this->pts.reserve(2);
		this->pts.push_back(p1);
		this->pts.push_back(p2);
		refine_level = level;
        
        local_rot = true;
        input_rot = false;
        
        rotated = false;
	}
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	RefinementCubeRegion::~RefinementCubeRegion()
	{
		
	}
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    GeometricTransform RefinementCubeRegion::rotateWithinYou(TriMesh &input) {
		//~ GeometricTransform gt;
        //~ gt.rotateSurfaceMesh(input);
        //~ rotate(gt);
        //~ rotated = true;
        //rotate input and save it in gt variable (see .h). then return gt.
        return gt;
        //gt defined in parent class
    }

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    
    void RefinementCubeRegion::rotate(GeometricTransform &gt){
		
		//~ gt.apply(this->pts[0]);
		//~ gt.apply(this->pts[1]);
        rotated = true;

        this->gt = gt;
        
    }
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	bool RefinementCubeRegion::intersectsOctant(vector<MeshPoint> &points, Octant &oct)
    {
        
        Point3D p1 = points[oct.getPoints()[0]].getPoint();
        Point3D p2 = points[oct.getPoints()[6]].getPoint();
     
        //if the input mesh was rotated w.r.t. this cube, apply inverse
        
        if (rotated) {
            gt.applyInverse(p1);
            gt.applyInverse(p2);
        }
        
        if (p2[0]<=pts[0][0] || pts[1][0] <= p1[0] ||
            p2[1]<=pts[0][1] || pts[1][1] <= p1[1] ||
            p2[2]<=pts[0][2] || pts[1][2] <= p1[2]) {
            return false;
        }
        
        return true;
	}
}
