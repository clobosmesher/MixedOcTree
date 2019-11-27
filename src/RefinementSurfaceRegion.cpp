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

#include "RefinementSurfaceRegion.h"

namespace Clobscode
{	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	RefinementSurfaceRegion::RefinementSurfaceRegion(TriMesh &tm, const unsigned short &level)
	{
		this->tm = tm;
		refine_level = level;
        
        local_rot = true;
        input_rot = true;
	}

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	RefinementSurfaceRegion::~RefinementSurfaceRegion()
	{
		
	}

    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	void RefinementSurfaceRegion::rotate(GeometricTransform &gt) {
		
		for (unsigned int i=0; i<tm.getPoints().size(); i++){
			gt.apply(tm.getPoints()[i]);
        }
		
		vector<vector<unsigned int> > faces (tm.getFaces().size(), vector<unsigned int>(3, 0));
        for (unsigned int i=0; i<tm.getFaces().size(); i++) {
            faces[i] = tm.getFaces()[i].getPoints();
        }
        //creating a new trimesh with the new vertices rotated
        TriMesh tm2 (tm.getPoints(), faces);
        this->tm = tm2;
    }

    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    GeometricTransform RefinementSurfaceRegion::rotateWithinYou(TriMesh &input) {
        
        GeometricTransform gt;
        gt.calculateAnglesAndCentroid(tm);
        gt.rotateSurfaceMesh(this->tm);
        TriMesh aux = tm;
        this->tm = input;
        rotate(gt);
        input = this->tm;
        this->tm = aux;
        return gt;
        
		/*GeometricTransform gt;
        gt.calculateAnglesAndCentroid(tm);
        gt.rotateSurfaceMesh(tm);
        gt.rotateSurfaceMesh(input);
        return gt;*/
    }
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------

	bool RefinementSurfaceRegion::intersectsOctant(vector<MeshPoint> &points, Octant &oct)
    {
        
        //(Point3D &p1, Point3D &p2)
        
        Point3D p1 = points[oct.getPoints()[0]].getPoint();
        Point3D p2 = points[oct.getPoints()[6]].getPoint();
        
        //test if any node of the octant is Inside the Mesh.
        if (tm.pointIsInMesh(p1)) {
            return true;
        }
        
        if (tm.pointIsInMesh(p2)) {
            return true;
        }
        
        Point3D aux;
        
        aux[0] = p1[0];
        aux[1] = p1[1];
        aux[2] = p2[2];
        if (tm.pointIsInMesh(aux)) {
            return true;
        }
        aux[0] = p1[0];
        aux[1] = p2[1];
        aux[2] = p1[2];
        if (tm.pointIsInMesh(aux)) {
            return true;
        }
        aux[0] = p2[0];
        aux[1] = p1[1];
        aux[2] = p1[2];
        if (tm.pointIsInMesh(aux)) {
            return true;
        }
        aux[0] = p2[0];
        aux[1] = p2[1];
        aux[2] = p1[2];
        if (tm.pointIsInMesh(aux)) {
            return true;
        }
        aux[0] = p2[0];
        aux[1] = p1[1];
        aux[2] = p2[2];
        if (tm.pointIsInMesh(aux)) {
            return true;
        }
        aux[0] = p1[0];
        aux[1] = p2[1];
        aux[2] = p2[2];
        if (tm.pointIsInMesh(aux)) {
            return true;
        }
        
        
        //test if any node of the mesh is inside the octant
        vector<Point3D> tmpts = tm.getPoints();
        for (unsigned int i=0; i<tmpts.size(); i++) {
            if (tmpts[i][0]<p1[0] || p2[0]<tmpts[i][0]) {
                continue;
            }
            if (tmpts[i][1]<p1[1] || p2[1]<tmpts[i][1]) {
                continue;
            }
            if (tmpts[i][2]<p1[2] || p2[2]<tmpts[i][2]) {
                continue;
            }
            return true;
        }
        
        //to implement: adapt from Octant - triangle intersection test in order to
        //test if an edge (of Region Surface Mesh) intersects the octant.
        /*set<SurfaceEdge> r_edges = tm.getEdges();
        set<SurfaceEdge>::iterator editer;
        for (editer = r_edges.begin(); editer!=r_edges.end(); editer++) {
            Point3D p3 = tm.getPoints()[(*editer)[0]];
            Point3D p4 = tm.getPoints()[(*editer)[1]];
            if (edgeIntersection(p1,p2,p3,p4)) {
                return true;
            }
        }*/
                
        return false;
	}
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	bool RefinementSurfaceRegion::edgeIntersection(Point3D &oct_p1, Point3D &oct_p2,
                                                   Point3D &seg_p1, Point3D &seg_p2)
	{
        //This code is based on the Cohen-Sutherland algorithm
		//for image clipping over a given window. In this case
		//the "window" is the octant (cube), and the "line"
		//to be clipped is the segment seg_p1 - seg_p2.
		vector<unsigned int> sides(2,0);
		

        sides[0] = computePosition(seg_p1,oct_p1,oct_p2);
        if (sides[0] == 0) {
				return true;
        }
		
		sides[1] = computePosition(seg_p2,oct_p1,oct_p2);
        if (sides[1] == 0) {
            return true;
        }
		
		//Compute logic and between bits associated to each
		//segment node. If all of them are at same side (e.g.
		//all at left), the result will be different from cero,
		//meaning that there is no intersection.
        
		//unsigned int all_and = sides[0] & sides[1];
		
        //If all are at the same side, return false.
		if ((sides[0]&sides[1]) != 0) {
			return false;
		}
		
		//TEST EDGES//
		
		//Find out if they cross the hex. Two cases: both are
		//"centered", but one is on the left and the other on
		//the right (same for Top/bottom and Front/Back).
		//Tested with logic or. The other case is that their
		//are in exact opposite "3D corners". Tested with XoR.

        unsigned int result_or = sides[0]|sides[1];
        unsigned int result_xor = sides[0]^sides[1];
        if (result_or == 48) {
            //48 <> 11 00 00
            return true;
        }
        if (result_or == 12) {
            //12 <> 00 11 00
            return true;
        }
        if (result_or == 3) {
            //3 <> 00 00 11
            return true;
        }
        if (result_xor == 63) {
            //63 <> 11 11 11
            return true;
		}
		
		//General case of clipping a triangle's edge against
		//the octant.
		/*for (unsigned int i=0; i<n_pts; i++) {
			//cout << pts[face_pts_idx[i]] << " code is ";
			Point3D p1 = input_pts[face[i]];
			Point3D p2 = input_pts[face[(i+1)%n_pts]];
			
			if (clipGeneralCase(p1,p2,pmin,pmax)) {
				return true;
			}
		}*/
        
        
        
        //compute the parametric equations of the given segment
		double dx = seg_p2[0]-seg_p1[0];
		double dy = seg_p2[1]-seg_p1[1];
		double dz = seg_p2[2]-seg_p1[2];
		double X,Y,Z,t;
		
		//Each 't' value is computed regarding a particular
		//plane. For instance, X_min (pmin[0]) is YZ plane
		//defined at X_min. When the 't' value is between 0
		//and 1, it means that the point where the segment
		//cuts the plane is between the two points that define
		//the segment. In any other case, the intersection is
		//produced at some other point outside the limits of
		//this segment (p1,p2), therefore the clipping can be
		//directly discard.
		
		
		
		////////////////////////////////////////////////////
		// Important note: if tangent segment should be   //
		// considered as intersecting the octant, replace //
		// each < by a <= from this point to the end of   //
		// this method.                                   //
		////////////////////////////////////////////////////
		
		//test X axis over X_min
		t = (oct_p1[0]-seg_p1[0])/dx;
		
		if (0<=t && t<=1) {
			//test Y axis for this 't'
			Y = seg_p1[1] + t*dy;
			if (oct_p1[1]<=Y && Y<=oct_p2[1]) {
				//test Z axis for this 't'
				Z = seg_p1[2] + t*dz;
				if (oct_p1[2]<=Z && Z<=oct_p2[2]) {
					return true;
				}
			}
		}
		
		//test X axis over X_max
		t = (oct_p2[0]-seg_p1[0])/dx;
		
		if (0<=t && t<=1) {
			//test Y axis for this 't'
			Y = seg_p1[1] + t*dy;
			if (oct_p1[1]<=Y && Y<=oct_p2[1]) {
				//test Z axis for this 't'
				Z = seg_p1[2] + t*dz;
				if (oct_p1[2]<=Z && Z<=oct_p2[2]) {
					return true;
				}
			}
		}
		
		
		//test Y axis over Y_min
		t = (oct_p1[1]-seg_p1[1])/dy;
		
		if (0<=t && t<=1) {
			//test Y axis for this 't'
			X = seg_p1[0] + t*dx;
			if (oct_p1[0]<=X && X<=oct_p2[0]) {
				//test Z axis for this 't'
				Z = seg_p1[2] + t*dz;
				if (oct_p1[2]<=Z && Z<=oct_p2[2]) {
					return true;
				}
			}
		}
		
		//test Y axis over Y_max
		t = (oct_p2[1]-seg_p1[1])/dy;
		if (0<=t && t<=1) {
			//test Y axis for this 't'
			X = seg_p1[0] + t*dx;
			if (oct_p1[0]<=X && X<=oct_p2[0]) {
				//test Z axis for this 't'
				Z = seg_p1[2] + t*dz;
				if (oct_p1[2]<=Z && Z<=oct_p2[2]) {
					return true;
				}
			}
		}
		
		//test Z axis over Z_min
		t = (oct_p1[2]-seg_p1[2])/dz;
		if (0<=t && t<=1) {
			//test Y axis for this 't'
			X = seg_p1[0] + t*dx;
			if (oct_p1[0]<=X && X<=oct_p2[0]) {
				//test Z axis for this 't'
				Y = seg_p1[1] + t*dy;
				if (oct_p1[1]<=Y && Y<=oct_p2[1]) {
					return true;
				}
			}
		}
		
		//test Z axis over Z_max
		t = (oct_p2[2]-seg_p1[2])/dz;
		if (0<=t && t<=1) {
			//test Y axis for this 't'
			X = seg_p1[0] + t*dx;
			if (oct_p1[0]<=X && X<=oct_p2[0]) {
				//test Z axis for this 't'
				Y = seg_p1[1] + t*dy;
				if (oct_p1[1]<=Y && Y<=oct_p2[1]) {
					return true;
				}
			}
		}

		//If the above test didn't succeed, the segment
		//is not intersected by this octant
		return false;

    }
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	unsigned int RefinementSurfaceRegion::computePosition(const Point3D &p,
                                                          const Point3D &pmin,
                                                          const Point3D &pmax){
		unsigned int sides = 0;
		
		//decide left/right X axis.
		if (p[0]<pmin[0]) {
			sides += 1;
		}
		else if (pmax[0] < p[0]) {
			sides += 2;
		}
		
		//decide bottom/top Y axis.
		if (p[1]<pmin[1]) {
			sides += 4;
		}
		else if (pmax[1] < p[1]) {
			sides += 8;
		}
		
		//decide back/front Z axis.
		if (p[2]<pmin[2]) {
			sides += 16;
		}
		else if (pmax[2]<p[2]) {
			sides += 32;
		}
		
		return sides;
	}
}
