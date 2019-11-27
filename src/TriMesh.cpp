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

#include "TriMesh.h"

namespace Clobscode
{
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	TriMesh::TriMesh() {
    
    }
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	TriMesh::TriMesh(vector<Point3D> &pts, 
					 vector<vector<unsigned int> > &fv){
		
		if (pts.empty()) {
			std::cout << "Error in Mesh::Init input mesh without points\n";
			return;
		}
		
		bounds.assign(6,-1);
		
		bounds[0]=bounds[3]=pts[0][0];
		bounds[1]=bounds[4]=pts[0][1];
		bounds[2]=bounds[5]=pts[0][2];
		

		
		mVertices.reserve(pts.size());
		
		mVertices.push_back(Point3D (bounds[0],bounds[1],bounds[2]));
		
		// initialising the vertices and also, search for max and min coodinates in X, Y and Z.
		
		for (unsigned int i=1; i< pts.size(); i++) {
			
			double x = pts[i][0];
			double y = pts[i][1];
			double z = pts[i][2];
			
			mVertices.push_back(Point3D (x,y,z));
			
			if(bounds[0]>x)
				bounds[0]=x;
			else if(bounds[3]<x)
				bounds[3]=x;
			if(bounds[1]>y)
				bounds[1]=y;
			else if(bounds[4]<y)
				bounds[4]=y;
			if(bounds[2]>z)
				bounds[2]=z;
			else if(bounds[5]<z)
				bounds[5]=z;
		}
        
        /*for (unsigned int i=0; i<bounds.size(); i++) {
            cout << bounds[i] << endl;
        }*/
        
		
		outside = Point3D (bounds[3],bounds[4],bounds[5]);
		Point3D min (bounds[0],bounds[1],bounds[2]);
		
		//compute the max difference in an axe
		Point3D diff = outside - min;
		double increase = 0;
		if (diff.X()>diff.Y() && diff.X()>diff.Z()) {
			increase = diff.X()*0.1;
		}
						   
		else if (diff.Y()>diff.Z()) {
			increase = diff.Y()*0.1;
		}
		else {
			increase = diff.Z()*0.1;
		}
		//this node will be outside and will be used to accelerate
		//the inside / outside test (with the Ray-Tracing algorithm)
		outside.X()+=increase;
		outside.Y()+=increase;
		outside.Z()+=increase;
		
		
		mTriangles.reserve(fv.size());
		// initialising the faces
		for (unsigned int iF = 0; iF < fv.size(); iF++) {
			SurfTriangle t(fv[iF]);
			t.computeNormal(mVertices);
			mTriangles.push_back(t);		
		}
		
		// computing the pseudo normal at each surface node
		mVerticePseudoNormals.reserve(pts.size());
		for (unsigned int i=0; i < pts.size(); i++) {
			//insert a null normal for each node
			Point3D fn;
			mVerticePseudoNormals.push_back(fn);
		}
		computeNodePseudoNormal();
		
		// computing the pseudo normal at each surface edge
		mEdgePseudoNormals.reserve(fv.size()*3);
		for (unsigned int i=0; i<fv.size()*3; i++) {
			//insert a null normal for each node
			Point3D en;
			mEdgePseudoNormals.push_back(en);
		}
		computeEdgePseudoNormal();
		
	}
	
	TriMesh::~TriMesh(){
		
	}
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------	
	void TriMesh::computeNodePseudoNormal(){
		
		unsigned int npts = mVertices.size();
		//mVerticePseudoNormals.reserve(npts);
		vector<list<unsigned int> > tri_per_node;
		tri_per_node.reserve(npts);
		
		//create a list for each node and initialize point normals
		for (unsigned int i=0; i<npts; i++ ) {
			list<unsigned int> dummy;
			tri_per_node.push_back(dummy);
			Point3D pn;
			mVerticePseudoNormals.push_back(pn);
		}
		
		//save a reference to the triangle for each node
		for (unsigned int i=0; i<mTriangles.size(); i++) {
			tri_per_node[mTriangles[i][0]].push_back(i);
			tri_per_node[mTriangles[i][1]].push_back(i);
			tri_per_node[mTriangles[i][2]].push_back(i);
		}
		
		//compute normal of each node
		for (unsigned int i=0; i<npts; i++) {
			list<unsigned int> pfcs = tri_per_node[i];
			list<unsigned int>::iterator piter;
			for (piter=pfcs.begin(); piter!=pfcs.end(); piter++) {
				mVerticePseudoNormals[i] += mTriangles[*piter].computeNormalAtNode(i,mVertices);
			}
			
			if (tri_per_node[i].empty()) {
				continue;
			}
			//normalize
			mVerticePseudoNormals[i].normalize();
		}
	}
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	void TriMesh::computeEdgePseudoNormal(){
		
		unsigned int nfcs = mTriangles.size();
		SurfEdgeContainer ec (nfcs*3);
		//mEdgePseudoNormals.reserve(nfcs*3);
		//this vector will know edge indexes with respect to each face
		vector<vector<unsigned int> > edges_per_triangle;
		edges_per_triangle.reserve(nfcs);
		
		//create the edges
		for (unsigned int i=0; i<nfcs; i++) {
			vector<unsigned int> t_edges(3,0);
			//note: edge container is in charge of managing AB == BA
			t_edges[0] = ec.addEdge(mTriangles[i][0], mTriangles[i][1], i);
			t_edges[1] = ec.addEdge(mTriangles[i][1], mTriangles[i][2], i);
			t_edges[2] = ec.addEdge(mTriangles[i][0], mTriangles[i][2], i);
			edges_per_triangle.push_back(t_edges);
		}
        
		//compute normal of each edge
		for (unsigned int i=0; i<nfcs; i++) {
			for (unsigned int j=0; j<3; j++) {
				unsigned int edge_idx = edges_per_triangle[i][j];
				unsigned int f_idx_2 = ec.getEdge(edge_idx).getOtherFace(i);
				Point3D en = (mTriangles[i].getNormal() + mTriangles[f_idx_2].getNormal());
				en.normalize();
				mEdgePseudoNormals[i*3+j] = en;
			}
		}
	}
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    bool TriMesh::SignedDistToTriangle(const Point3D & pP, const unsigned int &iT, 
									   const double &current_min_dist, double & pDist, 
									   Point3D & pProjP, bool & pIsIn, int & faceEdgeNode)
	
    {
		// compute the shortest distance between pP and the triangle (index iT).
		
		// inputs: 
		// Point3D pP, the point to be projected
		// uInt iT, the index of the triangle to test
		
		// outputs
		// double & pDist, the signed distance between the point and the triangle
		// VPoint3D & pProjP, the projected point
		// bool pIsIn, boolean expressing if the node P is inside or outside the surface
		
		
		// declare points and normals of this triangle.
		// p* the vertices of the triangle
		// pN*, the pseudo normals at each vertices of the triangle
		// pN**, the pseudo normals at each edges of the triangle		
		Point3D pA = mVertices[mTriangles[iT][0]];
		Point3D pB = mVertices[mTriangles[iT][1]];
		Point3D pC = mVertices[mTriangles[iT][2]];
		
		Point3D pNA = mVerticePseudoNormals[mTriangles[iT][0]];
		Point3D pNB = mVerticePseudoNormals[mTriangles[iT][1]];
		Point3D pNC = mVerticePseudoNormals[mTriangles[iT][2]];
		
		Point3D pNab = mEdgePseudoNormals[iT*3];
		Point3D pNbc = mEdgePseudoNormals[iT*3+1];
		Point3D pNac = mEdgePseudoNormals[iT*3+2];
		
        // Triangle vectors
        Point3D AB = pB - pA;
        Point3D AC = pC - pA;
        Point3D BC = pC - pB;
		
        // Normal vector pointing towards positive half-space, assuming (ABC) is CCW
        Point3D N  = AB^AC;
        double N2  = N * N;
		
		// normal used to compute the sign
		Point3D pProjN;
		
		//True if the given node is co-planar to this face
		bool coplanar = false;
		
        //------------------------------
        // Projection on plane (ABC)
        //------------------------------
		
        // P1 = P projected on plane
        Point3D P1;
        Point3D AP1;
        {
            // Projection of P on ABC plane
            Point3D AP = pP - pA;
            double k = ( AP * N ) / N2;
			
			// do not trust this face when the node is co-planar 
			// to it and the result is "outside", unless all 
			// faces be co-planar to the node.
			if (fabs(k)<1E-8) {
				coplanar = true;
			}
			
            P1 = pP - k*N;
						
            // Check if P1 is in triangle
            AP1 = P1 - pA;
            double x = (AP1 ^  AC) * N / N2;
            double y = (AB  ^ AP1) * N / N2;
			
            // AP1 = x*AB + y*AC
            if( x>=0 && y>=0 && x+y<=1 )
            {
                // Normal interpolation
				// real normal of the triangle
				pProjN = N;
				
                // Signed distance
                double lAbsDist = pP.DistanceTo(P1);
                double lSgn = pProjN * ( pP - P1 );
				
                // Projection params
                pProjP = P1;
                pDist = lSgn<0 ? -lAbsDist : lAbsDist ;
				
				// computing if the node is inside the surface
				pIsIn = lSgn < 0;
				
				faceEdgeNode = 0;
                
				// return false as the node is inside the triangle
				return false;
            }
        }
		
        //-------------------------------------------------------------------
        // P1 is out of the triangle : compute projections on triangle edges
        //-------------------------------------------------------------------
		
        //-------------------------------
        // Distance to segment AB
        //-------------------------------
        {
            double AB2 = AB * AB;
            double t = ( AP1 * AB ) / AB2;
			
            if( t < 0 )
            {
                pProjP = pA;
                pProjN = pNA;
				
				faceEdgeNode = 2;
            }
            else
				if( t > 1 )
				{
					pProjP = pB;
					pProjN = pNB;
					
					faceEdgeNode = 2;
				}
				else
				{
					pProjP = pA + t * AB;
					// pseudo normal of the edge AB
					pProjN = pNab;
					faceEdgeNode = 1;
				}
			
            pDist = pProjP.DistanceTo( pP );
        }
		
        //-------------------------------
        // Distance to segment AC
        //-------------------------------
        {
            double AC2 = AC * AC;
            double t = ( AP1 * AC ) / AC2;
			
            Point3D P2;
            Point3D P2N;
            if( t < 0 )
            {
                P2 = pA;
                P2N = pNA;
            }
            else
				if( t > 1 )
				{
					P2 = pC;
					P2N = pNC;
				}
				else
				{
					P2 = pA + t * AC;
					// pseudo normal of the edge AC
					P2N = pNac;
				}
			
            double d = P2.DistanceTo( pP );
			
            // Better ?
            if( d < pDist )
            {
                // Update
                pDist = d;
                pProjP = P2;
                pProjN = P2N;
				
				if ((t>=0) && (t<=1)) faceEdgeNode = 1;
				else faceEdgeNode = 2;
            }
        }
		
        //-------------------------------
        // Distance to segment BC
        //-------------------------------
        {
            double BC2 = BC * BC;
            double t = ( (P1-pB) * BC ) / BC2;
			
            Point3D P2;
            Point3D P2N;
            if( t < 0 )
            {
                P2 = pB;
                P2N = pNB;
            }
            else
				if( t > 1 )
				{
					P2 = pC;
					P2N = pNC;
				}
				else
				{
					P2 = pB + t * BC;
					// pseudo normal of the edge BC
					P2N = pNbc;	
				}
			
            double d = P2.DistanceTo( pP );
			
            // Better ?
            if( d < pDist )
            {
                // Update
                pDist = d;
                pProjP = P2;
                pProjN = P2N;
				
				if ((t>=0) && (t<=1)) faceEdgeNode = 1;
				else faceEdgeNode = 2;
            }
        }
		
        //-------------------------------
        // Orientation
        //-------------------------------
		
        // Change distance sign
        if( pProjN * ( pP - pProjP ) < 0)
		{
            pDist = -pDist;
		}
		
		pIsIn = pDist < 1E-8;
		
		return coplanar;
		
    }

	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	bool TriMesh::pointIsInMesh(const Point3D & pPoint ){
		// define if a point is inside a mesh or not
		
		Point3D caca = pPoint;
		//cout << "testing point " << caca;
		
		// index of the closest triangle
		unsigned int closestTriangle = 0;
		// closest point on the triangle (on triangle face, on edge, or vertice)
		Point3D pProjP;
		// distance to this closest point (always positive)
		double pDist = 0;
		//current closest distance: positive infinity
		double closestDist = numeric_limits<double>::infinity();
		// true if this node is inside the surface
		bool pIsIn = false;
		bool bIsIn = false;
		// 0 if close to a face, 1 if close to an edge, 2 if close to a vertice
		int faceEdgeNode = 0;
		int iFaceEdgeNode = 0;
		
		if (mTriangles.empty()) {
			return false;
		}
		
		// browsing all the surface faces for min distance.
		for (unsigned int iSurfF = 0; iSurfF < mTriangles.size(); iSurfF++) {
			// computing the distance for this face (triangle)
			SignedDistToTriangle(pPoint,iSurfF,closestDist,pDist,pProjP,pIsIn,faceEdgeNode);
			
			pDist = fabs(pDist);
			
			if (pDist < closestDist) {
				closestTriangle = iSurfF;
				closestDist = pDist;
				bIsIn = pIsIn;
				iFaceEdgeNode = faceEdgeNode;
			}
		}
		
		return bIsIn;
	}
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	bool TriMesh::pointIsInMesh(const Point3D & pPoint, list<unsigned int> &lFaces){
		// define if a point is inside a mesh or not
		
		// index of the closest triangle
		unsigned int closestTriangle = 0;
		// closest point on the triangle (on triangle face, on edge, or vertice)
		Point3D pProjP;
		// distance to this closest point (always positive)
		double pDist;
		//current closest distance: positive infinity
		double closestDist = numeric_limits<double>::infinity();
		// true if this node is inside the surface
		bool pIsIn;
		bool bIsIn = false;
		// 0 if close to a face, 1 if close to an edge, 2 if close to a vertice
		int faceEdgeNode;
		int iFaceEdgeNode;
		
		if (mTriangles.empty() || lFaces.empty()) {
			return false;
		}
		
		list<unsigned int>::iterator iSurfF;
		
		//one_good recalls if a non co-planar face conserve the
		//current_min_dis
		bool one_good = false;
		
		// browsing all the surface faces
		for (iSurfF = lFaces.begin(); iSurfF!=lFaces.end(); iSurfF++)
		{
			
			bool coplanar = SignedDistToTriangle(pPoint,*iSurfF,closestDist,
												 pDist,pProjP,pIsIn,faceEdgeNode);
			
			if (coplanar) {
				if (!one_good) {
					pDist = fabs(pDist);
					
					if (pDist < closestDist) {
						closestTriangle = *iSurfF;
						closestDist = pDist;
						bIsIn = pIsIn;
						iFaceEdgeNode = faceEdgeNode;
					}
				}
			}
			else {
				pDist = fabs(pDist);
				
				if (!one_good || pDist < closestDist) {
					closestTriangle = *iSurfF;
					closestDist = pDist;
					bIsIn = pIsIn;
					iFaceEdgeNode = faceEdgeNode;
				}
				one_good = true;
			}
		}
		
		return bIsIn;
	}
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	Point3D TriMesh::getProjection(const Point3D & pPoint){
		// define if a point is inside a mesh or not
		
		// index of the closest triangle
		unsigned int closestTriangle = 0;
		// closest point on the triangle (on triangle face, on edge, or vertice)
		Point3D pProjP_tmp,pProjP;
		// distance to this closest point (always positive)
		double pDist;
		//current closest distance: positive infinity
		double closestDist = numeric_limits<double>::infinity();
		// true if this node is inside the surface
		bool pIsIn;
		bool bIsIn = false;
		// 0 if close to a face, 1 if close to an edge, 2 if close to a vertice
		int faceEdgeNode;
		int iFaceEdgeNode;
		
		if (mTriangles.empty()) {
			cout << "Error at TriMesh::getProjection nowhere to project a point\n";
			return pProjP;
		}
		
		// browsing all the surface faces for min distance.
		for (unsigned int iSurfF = 0; iSurfF < mTriangles.size(); iSurfF++) {
			// computing the distance for this face (triangle)
			SignedDistToTriangle(pPoint,iSurfF,closestDist,pDist,pProjP_tmp,pIsIn,faceEdgeNode);
			
			pDist = fabs(pDist);
			
			if (pDist < closestDist) {
				pProjP = pProjP_tmp;
				closestTriangle = iSurfF;
				closestDist = pDist;
				bIsIn = pIsIn;
				iFaceEdgeNode = faceEdgeNode;
			}
		}
		
		return pProjP;
	}
		
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	Point3D TriMesh::getProjection(const Point3D & pPoint, list<unsigned int> &lFaces){
		// define if a point is inside a mesh or not
		
		// index of the closest triangle
		unsigned int closestTriangle = 0;
		// closest point on the triangle (on triangle face, on edge, or vertice)
		Point3D pProjP_tmp,pProjP;
		// distance to this closest point (always positive)
		double pDist;
		//current closest distance: positive infinity
		double closestDist = numeric_limits<double>::infinity();
		// true if this node is inside the surface
		bool pIsIn;
		bool bIsIn = false;
		// 0 if close to a face, 1 if close to an edge, 2 if close to a vertice
		int faceEdgeNode;
		int iFaceEdgeNode;
		
		if (mTriangles.empty() || lFaces.empty()) {
			cout << "Error at TriMesh::getProjection nowhere to project a point\n";
			return pProjP;
		}
		
		list<unsigned int>::iterator iSurfF;
		
		bool found = false;
		
		// browsing all the surface faces
		for (iSurfF = lFaces.begin(); iSurfF!=lFaces.end(); iSurfF++)
		{
			
			// computing the distance for this face (triangle)
			SignedDistToTriangle(pPoint,*iSurfF,closestDist,pDist,pProjP_tmp,pIsIn,faceEdgeNode);
			
			pDist = fabs(pDist);
			
			if (pDist < closestDist) {
				pProjP = pProjP_tmp;
				closestTriangle = *iSurfF;
				closestDist = pDist;
				bIsIn = pIsIn;
				iFaceEdgeNode = faceEdgeNode;
				found = true;
			}
		}
		
		if (!found) {
			cout << "Error in TriMesh::getProjection";
			cout << " couldn't project node\n";
		}
		
		return pProjP;
	}
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	void TriMesh::toCube(double per){
		
		double x = bounds[3]-bounds[0],
		y = bounds[4]-bounds[1],
		z = bounds[5]-bounds[2];
		
		double side;
		if(x<0)
			x=-x;
		if(y<0)
			y=-y;
		if(z<0)
			z=-z;
		
		
		if(x>y && x>z){
			double a1,a2;
			a1 = bounds[1] + (y-x)/2;
			a2 = bounds[4] + (x-y)/2;
			bounds[1] = a1;
			bounds[4] = a2;
			a1 = bounds[2] + (z-x)/2;
			a2 = bounds[5] + (x-z)/2;
			bounds[2] = a1;
			bounds[5] = a2;
			side =x;
		}
		else if(y>x && y>z){
			double a1,a2;
			a1 = bounds[0] + (x-y)/2;
			a2 = bounds[0] + (x+y)/2;
			bounds[0] = a1;
			bounds[3] = a2;
			a1 = bounds[2] + (z-y)/2;
			a2 = bounds[2] + (z+y)/2;
			bounds[2] = a1;
			bounds[5] = a2;
			side =y;
		}
		else{
			double a1,a2;
			a1 = bounds[0] + (x-z)/2;
			a2 = bounds[0] + (x+z)/2;
			bounds[0] = a1;
			bounds[3] = a2;
			a1 = bounds[1] + (y-z)/2;
			a2 = bounds[1] + (y+z)/2;
			bounds[1] = a1;
			bounds[4] = a2;
			side =z;
		}
		side*=per;
		bounds[0]-=side;bounds[1]-=side;bounds[2]-=side;
		bounds[3]+=side;bounds[4]+=side;bounds[5]+=side;
	}
	
    vector<Point3D> TriMesh::getNormals(){
		vector<Point3D> normals;
		for (unsigned int i=0; i<getFaces().size(); i++){
            normals.push_back(getFaces()[i].getSemiNormalizedNormal());
        }
        return normals;
	} 
}
