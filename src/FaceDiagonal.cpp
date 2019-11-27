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

#include "FaceDiagonal.h"

namespace Clobscode
{
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	FaceDiagonal::FaceDiagonal(vector<unsigned int> &mypoints){
        if (mypoints.size()!=4) {
            std::cerr << "warning at FaceDiagonal constructor: not square face\n";
        }
        
        points = mypoints;
	}
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
	FaceDiagonal::~FaceDiagonal(){
	
    }
	
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    bool FaceDiagonal::getDiagonal(vector<bool> &inside_state,
                                   vector<unsigned int> &diag){
    
        //detect number of inside nodes for this face
        unsigned int n_in = 0;
        for (unsigned int i=0; i<points.size(); i++) {
            if (inside_state[points[i]]) {
                n_in++;
            }
        }
        
        //all or none of this face nodes are inside, there is no
        //diagonal in this face.
        if (n_in == 0 || n_in == 4) {
            return false;
        }
        
        if (n_in == 1) {
            //detect the one node that is insde and return previous
            //and next nodes as diagonal for this face.
            for (unsigned int i=0; i<points.size(); i++) {
                if (inside_state[points[i]]) {
                    diag[0] = points[(i+3)%4];
                    diag[1] = points[(i+1)%4];
                    return true;
                }
            }
        }
        
        if (n_in == 2) {
            //two possible cases
            unsigned int n_in1, n_in2;
            for (unsigned int i=0; i<points.size(); i++) {
                if (inside_state[points[i]]) {
                    if (inside_state[points[(i+1)%4]]) {
                        //the two nodes are consecutive.
                        return false;
                    }
                    else {
                        if (inside_state[points[(i+3)%4]]) {
                            return false;
                        }
                        else{
                            //return previous and next nodes as diagonal
                            //for this face.
                            diag[0] = points[(i+3)%4];
                            diag[1] = points[(i+1)%4];
                            return true;
                        }
                    }
                }
            }
        }
        
        if (n_in == 3) {
            //find the one node that is outside
            for (unsigned int i=0; i<points.size(); i++) {
                if (!inside_state[points[i]]) {
                    diag[0] = points[i];
                    diag[1] = points[(i+2)%4];
                    return true;
                }
            }
        }
        
        std::cerr << " warning: unexpected case at FaceDiagonal::getDiagonal\n";
        
        return false;
    }
    
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    
    

	
	/******************
	 * This class manage the different cases of 3 points inside patterns.
	 * Once the pattern is identified, the corresponding class will handle
	 * the elements to be returned (
	 *//// 
}

