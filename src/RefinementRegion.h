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

#ifndef RefinementRegion_h
#define RefinementRegion_h 1

#include "SurfTriangle.h"
#include "MeshPoint.h"
#include "GeometricTransform.h"
#include "Octant.h"
#include <vector>
#include <iostream>

using SurfMesh::SurfTriangle;
using Clobscode::Point3D;
using std::vector;
using std::cout;
using Clobscode::Octant;
using Clobscode::GeometricTransform;

namespace Clobscode
{	
	class RefinementRegion
	{
	public:
		
		// Construction / destruction
		RefinementRegion();
				
		virtual ~RefinementRegion();

		virtual vector<Point3D> &getPoints();
        
        virtual void updatePoints(vector<Point3D> &npts);
        
        //virtual void rotate(Point3D &centroid, double &xAxisRotationAngle,
        //                    double &yAxisRotationAngle, double &zAxisRotationAngle)=0;
        
        virtual void rotate(GeometricTransform &gt)=0;
        
        virtual GeometricTransform rotateWithinYou(TriMesh &input)=0;
        
        virtual bool intersectsOctant(vector<MeshPoint> &points, Octant &oct)=0;
        
        virtual bool needsLocalRotation();
        
        virtual bool needsInputRotation();
        
        virtual void forceInputRotation();
		
		virtual unsigned short getRefinementLevel();
		
	protected:
		// Data
		unsigned short refine_level;
        bool local_rot, input_rot;
        vector<Point3D> pts;
        GeometricTransform gt;
	};
	
	inline unsigned short RefinementRegion::getRefinementLevel(){
		return refine_level;
	}
    
    inline vector<Point3D> &RefinementRegion::getPoints() {
        return pts;
    }
    
    inline void RefinementRegion::updatePoints(vector<Point3D> &npts) {
        
        pts.clear();
        pts.reserve(npts.size());
        
        for (unsigned int i=0; i<npts.size(); i++) {
            pts.push_back(npts[i]);
        }
    }
    
    inline bool RefinementRegion::needsLocalRotation() {
        return local_rot;
    }
    
    inline bool RefinementRegion::needsInputRotation() {
        return input_rot;
    }
    
    inline void RefinementRegion::forceInputRotation() {
        input_rot = true;
    }
}

#endif
