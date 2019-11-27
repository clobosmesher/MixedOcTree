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

#include "GeometricTransform.h"
#include <algorithm>

namespace Clobscode
{
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	GeometricTransform::GeometricTransform(){
        x=y=z=0;
	}
	
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	GeometricTransform::GeometricTransform(Point3D &p, double &x, double &y, double &z){
		centroid = p;
        this->x=x;
        this->y=y;
        this->z=z;
	}
	
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	GeometricTransform::~GeometricTransform(){
		
	}
    
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    void GeometricTransform::rotateSurfaceMesh(TriMesh &tm) {
		//calculateAnglesAndCentroid(tm);
		
		for (unsigned int i=0; i<tm.getPoints().size(); i++){
			apply(tm.getPoints()[i]);
        }
		
		vector<vector<unsigned int> > faces (tm.getFaces().size(), vector<unsigned int>(3, 0));
        for (unsigned int i=0; i<tm.getFaces().size(); i++) {
            faces[i] = tm.getFaces()[i].getPoints();
        }
        //creating a new trimesh with the new vertices rotated
        TriMesh tm2 (tm.getPoints(), faces);
        tm = tm2;
        
    }
    
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    void GeometricTransform::rotateSurfaceMeshInverse(TriMesh &tm) {
        //to implement here
    }
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    void GeometricTransform::calculateAnglesAndCentroid(TriMesh &tm) {
		
        vector<Point3D> vertices, normals, maxNormals;
		vertices = tm.getPoints();
        
        double alpha = 0;//77/57.2957795;
        double beta = 0;//35/57.2957795;
        double gamma = 0;//56/57.2957795;
        double epsilon = 5/57.2957795, angle;
        double maxNorm = 0;
        int maxIndex = 0;
        
        normals = tm.getNormals();
        maxNormals.push_back(normals[0]);
        //getting a distribution of mean maximum normals
        for (unsigned int i=1; i<normals.size(); i++){
            for (unsigned int j=0; j<maxNormals.size(); j++){
                angle = acos((normals[i]*maxNormals[j])/(normals[i].Norm()*maxNormals[j].Norm()));
                if(angle<epsilon){
                    maxNormals[j] += normals[i];
                    break;
                }
                else if(angle>175/57.2957795){
                    maxNormals[j] += normals[i]*(-1);
                    break;
                }
                else if(j==maxNormals.size()-1){
                    maxNormals.push_back(normals[i]);
                    break;
                }
            }
        }
        
        
        //sorting the maxnormals from the greatest norm to lower one
        std::sort (maxNormals.begin(), maxNormals.end(), largestNormal);
        
        Point3D xAxis (1, 0, 0);
        Point3D zAxis (0, 0, 1);
        Point3D yAxis (0, 1, 0);
        Point3D aux =  maxNormals[0];
        //finding a orthogonal maxnormal for the greatest maxnormal
        for (unsigned int j=1; j<maxNormals.size(); j++){
            if(maxNormals[j].Norm()/maxNormals[0].Norm() > 0.2){
                maxNorm = acos((maxNormals[0]*maxNormals[j])/(maxNormals[0].Norm()*maxNormals[j].Norm()))*57.2957795;
                if(maxNorm > 80 && maxNorm < 100){
                    maxIndex = j;
                    break;
                }
            }
        }

        //finding the rotation angles
        aux.X() = 0;
        alpha = acos(zAxis*aux/(zAxis.Norm()*aux.Norm()));
        if(aux.Y() < 0) alpha*=-1;
        
        maxNormals[0].xAxisRotation(alpha);
        
        aux =  maxNormals[0];
        aux.Y() = 0;
        beta = acos(zAxis*aux/(zAxis.Norm()*aux.Norm()));
        if(aux.X() > 0) beta*=-1;
        maxNormals[maxIndex].xAxisRotation(alpha);
        maxNormals[maxIndex].yAxisRotation(beta);
        aux = maxNormals[maxIndex];
        aux.Z() = 0;
        if(maxIndex==0 || acos(xAxis*aux/(xAxis.Norm()*aux.Norm()))==acos(-1)) {
            gamma=0;
        }
        else{
			gamma = acos(xAxis*aux/(xAxis.Norm()*aux.Norm()));
            if(aux.Y() > 0) {
                gamma*=-1;
            }
		}
		
		this->x=alpha;
		this->y=beta;
		this->z=gamma;
		this->centroid=tm.getCentroid();
	}
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    void GeometricTransform::apply(Point3D &p) {
        p -= centroid;
        p.xAxisRotation(x);
        p.yAxisRotation(y);
        p.zAxisRotation(z);
    }
    
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
    void GeometricTransform::applyInverse(Point3D &p) {
       
        p.zAxisRotation(-z);
        p.yAxisRotation(-y);
        p.xAxisRotation(-x);
        p += centroid;
    }
}
