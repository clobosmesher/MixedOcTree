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

#include "SurfTriangle.h"

namespace SurfMesh
{
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	SurfTriangle::SurfTriangle(vector<unsigned int> &fpts){
		mIdxV.assign(3,0);
		mIdxV[0] = fpts[0];
		mIdxV[1] = fpts[1];
		mIdxV[2] = fpts[2];
	}

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	SurfTriangle::~SurfTriangle()
	{
	}

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	void SurfTriangle::computeNormal(vector<Point3D> &pts) 
	{
		// triangle normal computation
		// vector between A and B
		Point3D  vec01;
		vec01 = pts[mIdxV[1]] - pts[mIdxV[0]];

		// vector between A and C
		Point3D  vec02;
		vec02 = pts[mIdxV[2]] - pts[mIdxV[0]];

		Point3D N  = vec01^vec02;

		mTriangleNormal = N;
		
		onepoint = pts[mIdxV[0]];
	}
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	double SurfTriangle::distance(const Point3D &p){
		Point3D vd = p - onepoint;
		return vd*mTriangleNormal;
	}
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	
	Point3D SurfTriangle::projection(const Point3D &p1, const Point3D &p2){
		
		
		double d1 = distance(p1), d2 = distance(p2), s;
		s = d1/(d1-d2);
		
		Point3D dif = p2 - p1;
		dif *= s;
		
		return p1 - dif;
	}

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	unsigned int &SurfTriangle::operator[](unsigned int pos){
		return mIdxV.at(pos);
	}
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	unsigned int SurfTriangle::operator[](unsigned int pos) const{
		return mIdxV.at(pos);
	}
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	
	bool SurfTriangle::segmentIntersection(vector<Point3D> &pts,
										   const Point3D &ep1,
										   const Point3D &ep2){
		Point3D p0 = pts[mIdxV[0]];
		Point3D p1 = pts[mIdxV[1]];
		Point3D p2 = pts[mIdxV[2]];
		
		//compute the distance to each point of the edge
		double dis_min = (ep1 - p0).dot(mTriangleNormal);
		double dis_max = (ep2 - p0).dot(mTriangleNormal);
		
		//if both, ep1 and ep2 are at the same side of the
		//plane, there is no intersection.
		if ((dis_max>0 && dis_min>0) || (dis_max<0 && dis_min<0)) {
			return false;
		}
		
		/*if (dis_min==0) {
			
		}*/
		
		//Compute the intersection between the segment
		//and triangle's plane.
		double s = dis_min/(dis_min - dis_max);
		
		Point3D point_in_plane = ep1 + (ep2-ep1)*s;
		
		Point3D u = p1 - p0;
		Point3D v = p2 - p0;
		Point3D w = point_in_plane - p0;
		
		double uu=u.dot(u);
		double vv=v.dot(v);
		double uv=u.dot(v);
		double wv=w.dot(v);
		double wu=w.dot(u);
		double d =uv*uv - uu*vv;
		
		if(d==0){
			cout << "error !!! malformed triangle\n";
			cout << "at HexTriIntersection::";
			cout << "edgeTriangleIntersection";
			return false;
		}
		
		double si= (uv*wv - vv*wu)/d;
		double ti= (uv*wu - uu*wv)/d;
		if(si>=0 && ti>=0 && (si+ti)<=1){
			return true;
		}
		
		return false;
	}
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	Point3D SurfTriangle::computeNormalAtNode(unsigned int nidx, 
										   vector<Point3D> &pts ) 
	{
		//return mTriangleNormal;
		unsigned int lidx = 0;
		bool found = true;
		
		if (nidx == mIdxV[0]) {
			lidx = 0;
		}
		else {
			if (nidx == mIdxV[1]) {
				lidx = 1;
			}
			else {
				if (nidx == mIdxV[2]) {
					lidx = 2;
				}
				else {
					found = false;
				}
			}
		}

		if (!found) {
			Point3D v;
			std::cerr << "warning at SurfMesh::SurfTriangle::computeNormalAtNode";
			std::cerr << " node index not found in triangle\n";
			return v;
		}
		
		// triangle normal computation
		// vector between A and B
		Point3D  vec1;
		vec1 = pts[mIdxV[(lidx+1)%3]] - pts[mIdxV[lidx]];
		
		// vector between A and C
		Point3D  vec2;
		vec2 = pts[mIdxV[(lidx+2)%3]] - pts[mIdxV[lidx]];
		double angle = acos((vec1*vec2)/(vec1.Norm()*vec2.Norm()));
		return angle * mTriangleNormal;
	}
}
