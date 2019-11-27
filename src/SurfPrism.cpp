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

#include "SurfPrism.h"

using std::cout;

namespace Clobscode
{
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	SurfPrism::SurfPrism(vector<unsigned int> &mypoints){
        if (mypoints.size()!=6) {
            std::cerr << "warning at SurfPrism constructor: not 6 node element\n";
        }
        
        points = mypoints;
        
        scaled_jac_constant = sqrt(0.75);

	}
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	SurfPrism::~SurfPrism(){
	
    }
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    double SurfPrism::getQuality(vector<MeshPoint> &allpoints) {
        
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
    vector<vector<unsigned int> > SurfPrism::getSubElements(vector<bool> &inside_state){
    
        vector<vector<unsigned int> > sub_ele, digaonals;
        
        
        
        //detect diagonals over
        vector<unsigned int> f1(4,0),f2(4,0),f3(4,0);
        f1[0] = 0; f1[1] = 1; f1[2] = 4; f1[3] = 3;
        f2[0] = 0; f2[1] = 3; f2[2] = 5; f2[3] = 2;
        f3[0] = 1; f3[1] = 2; f3[2] = 5; f3[3] = 4;
        
        vector<unsigned int> diag_in_1(2,0), diag_in_2(2,0), diag_in_3(2,0);
        list<unsigned int> diag_nodes, diag_nodes_sorted;
        list<unsigned int>::iterator diag_iter;
        
        FaceDiagonal fd1 (f1);
        if (fd1.getDiagonal(inside_state,diag_in_1)) {
            diag_nodes.push_back(diag_in_1[0]);
            diag_nodes.push_back(diag_in_1[1]);
        }
        
        FaceDiagonal fd2 (f2);
        if (fd2.getDiagonal(inside_state,diag_in_2)) {
            diag_nodes.push_back(diag_in_2[0]);
            diag_nodes.push_back(diag_in_2[1]);
        }
        
        FaceDiagonal fd3 (f3);
        if (fd3.getDiagonal(inside_state,diag_in_3)) {
            diag_nodes.push_back(diag_in_3[0]);
            diag_nodes.push_back(diag_in_3[1]);
        }
        
        vector<unsigned int> sorted_idx = sortDiagonals(diag_nodes);
        
        if (sorted_idx.size()==0) {
            sub_ele.push_back(points);
        }
        
        //if the number of "diags" is 4, then we have to retrun
        //a pyramid and tetahedron.
        if (sorted_idx.size()==3) {
            
            vector<unsigned int> pyramid (5,0), base(4,0);
            vector<unsigned int> tetahedron (4,0);
            
            oppositeFace(sorted_idx[1],base);
            
            for (unsigned int i=0; i<4; i++) {
                pyramid[i] = points[base[i]];
            }
            
            pyramid[4] = points[sorted_idx[1]];
            sub_ele.push_back(pyramid);
            
            for (unsigned int i=0; i<3; i++) {
                tetahedron[i] = points[sorted_idx[i]];
            }
            
            tetahedron[3] = points[opposite(sorted_idx[1])];
            
            sub_ele.push_back(tetahedron);
            
        }
        
        if (sorted_idx.size()==4) {
            vector<unsigned int> tet1 (4,0), tet2 (4,0), tet3 (4,0);
            
            tet1[0] = points[sorted_idx[0]];
            tet1[1] = points[sorted_idx[2]];
            tet1[2] = points[sorted_idx[1]];
            tet1[3] = points[opposite(sorted_idx[1])];
            
            sub_ele.push_back(tet1);
            
            
            tet2[0] = points[sorted_idx[1]];
            tet2[1] = points[sorted_idx[2]];
            tet2[2] = points[sorted_idx[3]];
            tet2[3] = points[opposite(sorted_idx[2])];
            
            sub_ele.push_back(tet2);
            
            tet3[0] = points[sorted_idx[0]];
            tet3[1] = points[sorted_idx[3]];
            tet3[2] = points[sorted_idx[1]];
            tet3[3] = points[sorted_idx[2]];
            
            sub_ele.push_back(tet3);
            
        }
        
        return sub_ele;
    }
    
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    vector<unsigned int> SurfPrism::sortDiagonals(list<unsigned int> &original){
        list<unsigned int>::iterator iter;
        vector<unsigned int> org_vec, result;
        
        for (iter=original.begin(); iter!=original.end(); iter++) {
            org_vec.push_back(*iter);
        }
        
        org_vec.reserve(original.size());
        if (original.size()==4) {
            result.assign(3,0);
            
            if (!oneRepeated(org_vec,result)){
                std::cerr << "warning malformed SurfPrism found at SurfPrism::sortDiagonals\n";
            }
            return result;
            
        }
        else{
            if (original.size()==6) {
                result.assign(4,0);
                
                list<unsigned int> tmp;
                vector<unsigned int> first(3,0);
                
                if (oneRepeated(org_vec,first)) {
                    if (first[0]==org_vec[4]) {
                        result[0] = org_vec[5];
                        result[1] = first[0];
                        result[2] = first[1];
                        result[3] = first[2];
                        return result;
                    }
                    if (first[0]==org_vec[5]) {
                        result[0] = org_vec[4];
                        result[1] = first[0];
                        result[2] = first[1];
                        result[3] = first[2];
                        return result;
                    }
                    if (first[2]==org_vec[4]) {
                        result[0] = first[0];
                        result[1] = first[1];
                        result[2] = first[2];
                        result[3] = org_vec[5];
                        return result;
                    }
                    if (first[2]==org_vec[5]) {
                        result[0] = first[0];
                        result[1] = first[1];
                        result[2] = first[2];
                        result[3] = org_vec[4];
                        return result;
                    }
                }
                else{
                    vector<unsigned int> aux(4,0);
                    for (unsigned int i=0; i<4; i++) {
                        aux[i] = org_vec[i+2];
                    }
                    if (!oneRepeated(aux,first)) {
                        std::cerr << "warning malformed SurfPrism found at SurfPrism::sortDiagonals\n";
                    }
                    
                    if (first[2]==org_vec[0]) {
                        result[0] = first[0];
                        result[1] = first[1];
                        result[2] = first[2];
                        result[3] = org_vec[1];
                        return result;
                    }
                    
                    if (first[2]==org_vec[1]) {
                        result[0] = first[0];
                        result[1] = first[1];
                        result[2] = first[2];
                        result[3] = org_vec[0];
                        return result;
                    }
                    
                    else{
                        std::cerr << "warning malformed SurfPrism found at SurfPrism::sortDiagonals\n";
                    }
                }
            }
        }
        
        return result;
        
    }
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    bool SurfPrism::oneRepeated(vector<unsigned int> &original, vector<unsigned int> &result){
        
        //sort data

        if (original[0] == 4 && original[1] == 0) {
            original[0] = 0;
            original[1] = 4;
        }
        if (original[0] == 3 && original[1] == 1) {
            original[0] = 1;
            original[1] = 3;
        }
        
        if (original[0] == 5 && original[1] == 0) {
            original[0] = 0;
            original[1] = 5;
        }
        if (original[0] == 3 && original[1] == 2) {
            original[0] = 2;
            original[1] = 3;
        }
        
        if (original[2] == 5 && original[3] == 0) {
            original[2] = 0;
            original[3] = 5;
        }
        if (original[2] == 3 && original[3] == 2) {
            original[2] = 2;
            original[3] = 3;
        }
        
        if (original[2] == 5 && original[3] == 1) {
            original[2] = 1;
            original[3] = 5;
        }
        if (original[2] == 4 && original[3] == 2) {
            original[2] = 2;
            original[3] = 4;
        }
        

        if (original[0] == original[2]) {
            if (original[0]==0) {
                result[0] = 5;
                result[1] = 0;
                result[2] = 4;
                return true;
            }
            if (original[0]==1) {
                result[0] = 3;
                result[1] = 1;
                result[2] = 5;
                return true;
            }
            if (original[0]==2) {
                result[0] = 4;
                result[1] = 2;
                result[2] = 3;
                return true;
            }
        }
        
        if (original[1] == original[3]) {
            if (original[1]==4) {
                result[0] = 2;
                result[1] = 4;
                result[2] = 0;
                return true;
            }
            if (original[1]==3) {
                result[0] = 1;
                result[1] = 3;
                result[2] = 2;
                return true;
            }
            if (original[1]==5) {
                result[0] = 0;
                result[1] = 5;
                result[2] = 1;
                return true;
            }
        }
        
        return false;
    }
    
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    unsigned int SurfPrism::opposite(unsigned int idx){
        return (idx+3)%6;
    }

    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    void SurfPrism::oppositeFace(unsigned int idx, vector<unsigned int> &base){
        if (idx==0 || idx==3) {
            base[0] = 1;
            base[1] = 4;
            base[2] = 5;
            base[3] = 2;
            return;
        }
        if (idx==1 || idx==4) {
            base[0] = 0;
            base[1] = 2;
            base[2] = 5;
            base[3] = 3;
            return;
        }
        if (idx==2 || idx==5) {
            base[0] = 0;
            base[1] = 3;
            base[2] = 4;
            base[3] = 1;
            return;
        }
    }
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    vector<vector<unsigned int> > SurfPrism::getTetras(){
        vector<vector<unsigned int> > tetras;
        tetras.reserve(6);
        
        vector<unsigned int> t0(4,-1);
        t0[0]=points[0];
        t0[1]=points[2];
        t0[2]=points[3];
        t0[3]=points[1];
        tetras.push_back(t0);
        
        vector<unsigned int> t1(4,-1);
        t1[0]=points[1];
        t1[1]=points[2];
        t1[2]=points[0];
        t1[3]=points[4];
        tetras.push_back(t1);
        
        vector<unsigned int> t2(4,-1);
        t2[0]=points[2];
        t2[1]=points[0];
        t2[2]=points[1];
        t2[3]=points[5];
        tetras.push_back(t2);
        
        vector<unsigned int> t3(4,-1);
        t3[0]=points[3];
        t3[1]=points[5];
        t3[2]=points[4];
        t3[3]=points[0];
        tetras.push_back(t3);
        
        vector<unsigned int> t4(4,-1);
        t4[0]=points[4];
        t4[1]=points[3];
        t4[2]=points[5];
        t4[3]=points[1];
        tetras.push_back(t4);
        
        vector<unsigned int> t5(4,-1);
        t5[0]=points[5];
        t5[1]=points[4];
        t5[2]=points[3];
        t5[3]=points[2];
        tetras.push_back(t5);
        
        return tetras;
    }
    
	
	/******************
	 * This class manage the different cases of 3 points inside patterns.
	 * Once the pattern is identified, the corresponding class will handle
	 * the elements to be returned (
	 *//// 
}

