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

#ifndef TriMesh_h
#define TriMesh_h 1

#include "Point3D.h"
#include "SurfTriangle.h"
#include "SurfEdgeContainer.h"
//#include "SurfaceEdge.h"
#include <limits>
#include <set>

using std::vector;
using std::set;
using Clobscode::Point3D;
using SurfMesh::SurfTriangle;
using SurfMesh::SurfEdgeContainer;

namespace Clobscode
{
	class TriMesh{
		
	public:
        
        TriMesh();
        
		TriMesh(vector<Point3D> &pts, 
				vector<vector<unsigned int> > &fv);
		
		virtual ~TriMesh();
		
		virtual vector<Point3D> &getPoints();
		
		virtual vector<SurfTriangle> &getFaces();

		virtual vector<Point3D> &getVerticePseudoNormals();		
        
		virtual vector<double> &getBounds();
		
		virtual bool pointIsInMesh(const Point3D &pPoint);
		
		virtual bool pointIsInMesh(const Point3D & pPoint, 
								   list<unsigned int> &lFaces);
		
		virtual Point3D getProjection(const Point3D &pPoint);
		
		virtual Point3D getProjection(const Point3D & pPoint, 
								   list<unsigned int> &lFaces);
								 
		virtual Point3D getCentroid();
		
		virtual vector<Point3D> getNormals();
		
	protected:
		
		virtual void toCube(double per);
		
		// compute the pseudo normal at each surface node
		virtual void computeNodePseudoNormal();
		
		// compute the pseudo normal at each surface edge
		virtual void computeEdgePseudoNormal();
		
		virtual bool SignedDistToTriangle(const Point3D & pP, 
										  const unsigned int &iT, 
										  const double &current_min_dist, 
										  double & pDist, 
										  Point3D & pProjP, bool & pIsIn, 
										  int & faceEdgeNode);
		
		
	protected:
		
		vector<Point3D> mVertices;
		vector<SurfTriangle> mTriangles;
		
		//one pseudo normal per vertices (same size)
		vector<Point3D> mVerticePseudoNormals; 
		
		//one pseudo normal per edge, so 3 pseudo normals per 
		//triangle (3*size)
		vector<Point3D> mEdgePseudoNormals;
		
		//bounding box of this surface mesh
		vector<double> bounds;	
		
		//Point used by Ray Tracing Method (unstable)
		Point3D outside;
        
	};
	
	inline vector<Point3D> &TriMesh::getPoints(){
		return mVertices;
	}
	
	inline vector<double> &TriMesh::getBounds(){
        
        //cout << "tri bounds: " << bounds.size() << endl;
		return bounds;
	}
	
	inline vector<SurfTriangle> &TriMesh::getFaces(){
		return mTriangles;
	}
    
    inline vector<Point3D> &TriMesh::getVerticePseudoNormals(){
        return mVerticePseudoNormals;
    }
    
    inline Point3D TriMesh::getCentroid(){
		Point3D mCentroid;
		mCentroid.X() = (bounds[0]+bounds[3])/2;
        mCentroid.Y() = (bounds[1]+bounds[4])/2;
        mCentroid.Z() = (bounds[2]+bounds[5])/2;
		return mCentroid;
	}
	   
}
#endif
