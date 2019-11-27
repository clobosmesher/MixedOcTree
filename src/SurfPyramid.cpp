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

#include "SurfPyramid.h"

namespace Clobscode
{
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	SurfPyramid::SurfPyramid(vector<unsigned int> &mypoints){
        if (mypoints.size()!=5) {
            std::cerr << "warning at SurfPyramid constructor: not 6 node element\n";
        }
        
        points = mypoints;
        
        scaled_jac_constant = 1/sqrt(2);
	}
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	SurfPyramid::~SurfPyramid(){
	
    }
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    vector<vector<unsigned int> > SurfPyramid::getSubElements(vector<bool> &inside_state){
    
        vector<vector<unsigned int> > sub_ele, digaonals;
        
        //detect diagonals over
        vector<unsigned int> base(4,0);
        for (unsigned int i=0; i<4; i++) {
            base[i] = i;
        }
        
        vector<unsigned int> diag(2,0);
        
        FaceDiagonal base_di (base);
        if (!base_di.getDiagonal(inside_state,diag)) {
            sub_ele.push_back(points);
            return sub_ele;
        }
        
        unsigned int d = 0;
        
        for (unsigned int i=0; i<4; i++) {
            if (i!=diag[0] && i!=diag[1]) {
                d = i;
                break;
            }
        }
        
        vector<unsigned int> tet1 (4,0), tet2 (4,0);
        
        tet1[0] = points[d];
        tet1[1] = points[(d+1)%4];
        tet1[2] = points[(d+3)%4];
        tet1[3] = points[4];
        
        sub_ele.push_back(tet1);
        
        tet2[0] = points[(d+2)%4];
        tet2[1] = points[(d+3)%4];
        tet2[2] = points[(d+1)%4];
        tet2[3] = points[4];
        
        sub_ele.push_back(tet2);
    
        return sub_ele;
    }
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    double SurfPyramid::getQuality(vector<MeshPoint> &allpoints) {
        
        vector<vector<unsigned int> > allt = getTetras();
        double quality = 2, value = 2;
        bool one_neg = false;
        
        //we should optimize this computation: compute only once
        //all the vectors normalized and then multiply them by -1
        //if needed in the other direction. ToDo list.
        
        for (unsigned int i=0; i<allt.size(); i++) {
            
            Point3D n0 = allpoints[allt[i][0]].getPoint();
            Point3D n1 = allpoints[allt[i][1]].getPoint();
            Point3D n2 = allpoints[allt[i][2]].getPoint();
            Point3D n3 = allpoints[allt[i][3]].getPoint();
            
            //this is due to tetra definition
            Point3D d1 = (n1 - n0);
            Point3D d2 = (n2 - n0);
            Point3D d3 = (n3 - n0);
            
            d1.normalize();
            d2.normalize();
            d3.normalize();
            
            value = d1.dot(d2.cross(d3));
            
            if (value<0) {
                one_neg = true;
            }
            
            if (one_neg) {
                if (value>0) {
                    continue;
                }
                
                if (value < -scaled_jac_constant) {
                    quality = -1 - scaled_jac_constant - value;
                }

                else {
                    quality = value/scaled_jac_constant;
                }
            }
            
            else {
                if (value > scaled_jac_constant) {
                    quality = 1 + scaled_jac_constant - value;
                }
                else {
                    quality = value/scaled_jac_constant;
                }
            }
        }
        
        return quality;
    }
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    vector<vector<unsigned int> > SurfPyramid::getTetras(){
        vector<vector<unsigned int> > tetras;
        tetras.reserve(8);
        
        vector<unsigned int> t0(4,0);
        t0[0]=points[0];
        t0[1]=points[1];
        t0[2]=points[3];
        t0[3]=points[4];
        tetras.push_back(t0);
        
        vector<unsigned int> t1(4,0);
        t1[0]=points[1];
        t1[1]=points[2];
        t1[2]=points[0];
        t1[3]=points[4];
        tetras.push_back(t1);
        
        vector<unsigned int> t2(4,0);
        t2[0]=points[2];
        t2[1]=points[3];
        t2[2]=points[1];
        t2[3]=points[4];
        tetras.push_back(t2);
        
        vector<unsigned int> t3(4,0);
        t3[0]=points[3];
        t3[1]=points[0];
        t3[2]=points[2];
        t3[3]=points[4];
        tetras.push_back(t3);
        
        vector<unsigned int> t4(4,0);
        t4[0]=points[4];
        t4[1]=points[0];
        t4[2]=points[2];
        t4[3]=points[1];
        tetras.push_back(t4);
        
        vector<unsigned int> t5(4,0);
        t5[0]=points[4];
        t5[1]=points[1];
        t5[2]=points[3];
        t5[3]=points[2];
        tetras.push_back(t5);
        
        vector<unsigned int> t6(4,-0);
        t6[0]=points[4];
        t6[1]=points[2];
        t6[2]=points[0];
        t6[3]=points[3];
        tetras.push_back(t6);
        
        vector<unsigned int> t7(4,0);
        t7[0]=points[4];
        t7[1]=points[3];
        t7[2]=points[1];
        t7[3]=points[0];
        tetras.push_back(t7);
        
        return tetras;
    }
}

