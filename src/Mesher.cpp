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

#include "Mesher.h"
#include <math.h>

namespace Clobscode
{
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	Mesher::Mesher(){
        
	}
    
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	Mesher::~Mesher(){
		
	}
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    //Create a grid mesh regarding the Bounding Box of input mesh.
    //This will produce several cubes as roots of an octree structure.
    //Then split each initial element 8^rl times (where rl stands
    //for Refinement Level).
    FEMesh Mesher::refineMesh(TriMesh &input, const unsigned short &rl,
                              const string &name, list<unsigned int> &roctli,
                              list<RefinementRegion *> &all_reg,
                              GeometricTransform &gt, const unsigned short &minrl,
                              const unsigned short &omaxrl){
        
        //Note: rotation are not enabled when refining an already produced mesh.
        bool rotated = !gt.Default();
        if(rotated) {
            /*cout << "rotating input surface mesh\n";
            cout << gt.getCentroid() << "\n";
            cout << gt.getXAxis() << " " << gt.getYAxis();
            cout << " " << gt.getZAxis() << "\n";*/
            gt.rotateSurfaceMesh(input);
        }
        
        //split octants until the refinement level (rl) is achieved.
        //The output will be a one-irregular mesh.
        if (!roctli.empty()) {
            splitOctants(rl,input,roctli,all_reg,name,minrl,omaxrl);
        }
        else {
            generateOctreeMesh(rl,input,all_reg,name,minrl);
        }
        
        
        //Now that we have all the elements, we can save the octant mesh.
        Services::WriteOctreeMesh(name,points,octants,MapEdges,gt);
        //Some Octants will be then removed due to proximity with the surface.
        //However we must preserve them if the oct mesh to avoid congruency
        //problems. For this reason we will keep track of removed octants
        //so we can easily link elements to octant index when reading an oct
        //mesh.
        map<unsigned int, bool> removedoct;
        list<unsigned int> octmeshidx;
        for (auto o: octants) {
            removedoct[o.getIndex()] = true;
            octmeshidx.push_back(o.getIndex());
        }

        //link element and node info for code optimization.
        linkElementsToNodes();
        detectInsideNodes(input);
        
        projectCloseToBoundaryNodes(input);
        removeOnSurface();
        
        //linkElementsToNodes();
        //apply the surface Patterns
        applySurfacePatterns(input);
        removeOnSurface();
        
        detectInsideNodes(input);
        
        //update element and node info.
        linkElementsToNodes();
        
        //shrink outside nodes to the input domain boundary
        shrinkToBoundary(input);
        
        if (rotated) {
            for (unsigned int i=0; i<points.size(); i++) {
                gt.applyInverse(points[i].getPoint());
            }
        }
        
        //the allmighty output mesh
        FEMesh mesh;
        
        //save the data of the mesh
        saveOutputMesh(mesh);
        
        //Write element-octant info the file
        Services::addOctElemntInfo(name,octants,removedoct,octmeshidx);
        
        return mesh;
    }
    
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	//Create a grid mesh regarding the Bounding Box of input mesh.
	//This will produce several cubes as roots of an octree structure.
	//Then split each initial element 8^rl times (where rl stands
	//for Refinement Level).
	FEMesh Mesher::generateMesh(TriMesh &input, const unsigned short &rl,
								const string &name, list<RefinementRegion *> &all_reg){
        
        //ATTENTION: geometric transform causes invalid input rotation when the
        //input is a cube.
		GeometricTransform gt;
        
		//rotate: This method is written below and its mostly comented because
        //it causes conflicts when the input is a cube. Must be checked.
        bool rotated = false; //rotateGridMesh(input, all_reg, gt);
        
		//generate root octants
		generateGridMesh(input);
        
		//split octants until the refinement level (rl) is achieved.
		//The output will be a one-irregular mesh.
		generateOctreeMesh(rl,input,all_reg,name,0);
        
        //Any mesh generated from this one will start from the same
        //Octants as the current state.
        Services::WriteOctreeMesh(name,points,octants,MapEdges,gt);
        //Some Octants will be then removed due to proximity with the surface.
        //However we must preserve them if the oct mesh to avoid congruency
        //problems. For this reason we will keep track of removed octants
        //so we can easily link elements to octant index when reading an oct
        //mesh.
        map<unsigned int, bool> removedoct;
        list<unsigned int> octmeshidx;
        for (auto o: octants) {
            removedoct[o.getIndex()] = true;
            octmeshidx.push_back(o.getIndex());
        }
        
		//link element and node info for code optimization.
		linkElementsToNodes();
		detectInsideNodes(input);
        
        projectCloseToBoundaryNodes(input);
   		removeOnSurface();
		
		//apply the surface Patterns
		applySurfacePatterns(input);
        removeOnSurface();

        
        //projectCloseToBoundaryNodes(input);
		//removeOnSurface();
        detectInsideNodes(input);
        
		//update element and node info.
		linkElementsToNodes();
        
		//shrink outside nodes to the input domain boundary
		shrinkToBoundary(input);
        
        if (rotated) {
            for (unsigned int i=0; i<points.size(); i++) {
                gt.applyInverse(points[i].getPoint());
            }
        }
		
        //the almighty output mesh
        FEMesh mesh;
        
		//save the data of the mesh in its final state
		saveOutputMesh(mesh);
        
        //Write element-octant info the file
        Services::addOctElemntInfo(name,octants,removedoct,octmeshidx);
        
		return mesh;
	}
    
    //--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	bool Mesher::rotateGridMesh(TriMesh &input, list<RefinementRegion *> &all_reg,
								GeometricTransform &gt){
        
        list<RefinementRegion *>::iterator it, rrot;
        bool inputHasbeenRotated = false;

        for (it = all_reg.begin(); it!=all_reg.end(); it++) {
            //in case of input roi
            if((*it)->needsInputRotation()){
                if(!inputHasbeenRotated){
                    gt = (*it)->rotateWithinYou(input);
                    inputHasbeenRotated = true;
                    rrot=it;
                    break;
                }
            }
        }
        
        if (inputHasbeenRotated) {
            for (it = all_reg.begin(); it!=all_reg.end(); it++) {
                if (it!=rrot) {
                    if ((*it)->needsLocalRotation()) {
                        (*it)->rotate(gt);
                    }
                }
            }
        }

        return inputHasbeenRotated;
	}
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    void Mesher::generateGridMesh(TriMesh &input){
        //vectors with each coordinate per axis
        vector<double> all_x, all_y, all_z;
        vector<vector<unsigned int> > elements;
        
        auto start_time = chrono::high_resolution_clock::now();

        GridMesher gm;
        gm.generatePoints(input.getBounds(),all_x,all_y,all_z);
        gm.generateMesh(all_x,all_y,all_z,points,elements);
        
        octants.reserve(elements.size());
        
        //create the root octants
        for (unsigned int i=0; i<elements.size(); i++) {
            Octant o (elements[i], 0, i);
            //Only when the octant intersects the input
            //add it to the list of current octants. As
            //This is the first time octants are checked
            //for intersections they must be made w.r.t.
            //all input faces.
            IntersectionsVisitor iv(false);
            //if (o.checkIntersections(input,points)) {
            iv.setTriMesh(input);
            iv.setPoints(points);
            if (o.accept(&iv)) {
                EdgeVisitor::insertEdges(&o, MapEdges);
                octants.push_back(o);
            }
        }

        auto end_time = chrono::high_resolution_clock::now();
        cout << "    * generateGridMesh in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
        cout << " ms"<< endl;
    }
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    void Mesher::setInitialState(vector<MeshPoint> &epts, vector<Octant> &eocts,
                                 map<OctreeEdge, EdgeInfo> &medgs) {
        
        octants.assign(make_move_iterator(eocts.begin()),
                       make_move_iterator(eocts.end()));
        
        //Erase previous octants to save memory
        eocts.erase(eocts.begin(),eocts.end());
        
        points.assign(make_move_iterator(epts.begin()),make_move_iterator(epts.end()));
        epts.erase(epts.begin(),epts.end());
        
        MapEdges = medgs;
        //recompute Octant indexes and update edges with those indexes
        for (unsigned int i=0;i<octants.size();i++) {
            EdgeVisitor::insertAllEdges(&octants[i], MapEdges);
        }
    }
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    void Mesher::splitOctants(const unsigned short &rl, TriMesh &input,
                              list<unsigned int> &roctli,
                              list<RefinementRegion *> &all_reg, const string &name,
                              const unsigned short &minrl, const unsigned short &givenmaxrl){
        
        //The list of candidate Octants to refine and the tmp version of
        //adding those how are still candidates for the next iteration.
        list<Octant> candidates, new_candidates, clean_processed, refine_tmp;
        
        //The Octs that don't need further refinement.
        vector<Octant> processed;
        
        //A map that connect the index of processed Octs with their position in the
        //vector of processed Octs.
        map<unsigned int, unsigned int> idx_pos_map;
        
        //list of the points added at this refinement iteration:
        list<Point3D> new_pts;
        
        //list<Octrant>::iterator iter;
        
        //A set containing the index of Octs to be refined to
        //maintain balancing
        list<pair<unsigned int,unsigned int> > toBalance;
        
        //initialising the vector and the map
        candidates.assign(make_move_iterator(octants.begin()),
                          make_move_iterator(octants.end()));
        
        //The starting point for assignaiting indexes
        unsigned int new_o_idx = candidates.size();
        
        //Erase previous Octrants to save memory
        octants.erase(octants.begin(),octants.end());
        
        //create visitors and give them variables
        SplitVisitor sv;
        sv.setPoints(points);
        sv.setMapEdges(MapEdges);
        sv.setNewPts(new_pts);
        sv.setProcessedOctVector(processed);
        sv.setMapProcessed(idx_pos_map);
        sv.setToBalanceList(toBalance);
        
        unsigned short max_rl;
        if (givenmaxrl==0) {
            max_rl = rl;
        }
        else {
            max_rl = givenmaxrl;
        }
        
        
        bool localmax = false;
        
        //----------------------------------------------------------
        //refine once each Octrant in the list
        //----------------------------------------------------------
        
        //note: roctli must be a sorted index list.
        if (!roctli.empty()) {
            
            list<unsigned int>::iterator octidx = roctli.begin();
            
            unsigned int qua_pos = 0;
            while (!candidates.empty()) {
                
                Octant oct = *(candidates.begin());
                candidates.pop_front();
                
                if (octidx == roctli.end() || qua_pos!=*octidx) {
                    idx_pos_map[oct.getIndex()] = processed.size();
                    processed.push_back(oct);
                }
                else {
                    //we advance to next Octrant in the list for the next
                    //iteration.
                    octidx++;
                    refine_tmp.push_back(oct);
                }
                qua_pos++;
            }
            
            for (auto oct: refine_tmp) {
                    
                //start refinement process for current Octant.
                list<unsigned int> inter_faces = oct.getIntersectedFaces();
                unsigned short orl = oct.getRefinementLevel();
                
                if (orl==max_rl) {
                    localmax = true;
                }
                
                vector<vector<Point3D> > clipping_coords;
                sv.setClipping(clipping_coords);
                
                vector<vector<unsigned int> > split_elements;
                sv.setNewEles(split_elements);
                sv.setStartIndex(new_o_idx);
                
                oct.accept(&sv);
                
                if (inter_faces.empty()) {
                    for (unsigned int j=0; j<split_elements.size(); j++) {
                        Octant o (split_elements[j],orl+1,new_o_idx++);
                        new_candidates.push_back(o);
                    }
                }
                else {
                    for (unsigned int j=0; j<split_elements.size(); j++) {
                        Octant o (split_elements[j],orl+1,new_o_idx++);
                        
                        //the new points are inserted in bash at the end of this
                        //iteration. For this reason, the coordinates must be passed
                        //"manually" at this point (clipping_coords).
                        IntersectionsVisitor iv(true);
                        //if (o.checkIntersections(input,inter_faces,clipping_coords[j]))
                        iv.setTriMesh(input);
                        iv.setFaces(inter_faces);
                        iv.setCoords(clipping_coords[j]);
                        
                        if (o.accept(&iv)) {
                            new_candidates.push_back(o);
                        }
                        else {
                            //The element doesn't intersect any input face.
                            //It must be checked if it's inside or outside.
                            //Only in the first case add it to new_octants.
                            //Test this with parent octant faces only.
                            
                            //Comment the following lines of this 'else' if
                            //only intersecting octants are meant to be
                            //displayed.
                            
                            //note: inter_faces is quite enough to check if
                            //element is inside input, no octant needed,
                            //so i moved the method to mesher  --setriva
                            
                            if (isItIn(input,inter_faces,clipping_coords[j])) {
                                new_candidates.push_back(o);
                            }
                            else {
                                //we must update neighbor information at the edges
                                EdgeVisitor::removeOctFromEdges(&o, MapEdges);
                            }
                        }
                    }
                }
            }

            //Erase the list to refine
            refine_tmp.erase(refine_tmp.begin(),refine_tmp.end());
            
            while (!toBalance.empty()) {
                
                list<pair<unsigned int, unsigned int> > tmp_toBalance;
                std::swap(toBalance,tmp_toBalance);
                tmp_toBalance.sort();
                tmp_toBalance.unique();
                
                
                while (!tmp_toBalance.empty()) {
                    unsigned int key = tmp_toBalance.begin()->first;
                    unsigned int val = tmp_toBalance.begin()->second;
                    
                    Octant oct = processed[val];
                    tmp_toBalance.pop_front();
                    list<unsigned int> &inter_faces = oct.getIntersectedFaces();
                    unsigned short qrl = oct.getRefinementLevel();
                    
                    vector<vector<Point3D> > clipping_coords;
                    sv.setClipping(clipping_coords);
                    
                    vector<vector<unsigned int> > split_elements;
                    sv.setNewEles(split_elements);
                    sv.setStartIndex(new_o_idx);
                    
                    oct.accept(&sv);
                    
                    if (inter_faces.empty()) {
                        for (unsigned int j=0; j<split_elements.size(); j++) {
                            Octant o (split_elements[j], qrl+1, new_o_idx++);
                            idx_pos_map[o.getIndex()] = processed.size();
                            processed.push_back(o);
                        }
                    }
                    else {
                        for (unsigned int j=0; j<split_elements.size(); j++) {
                            Octant o (split_elements[j],qrl+1,new_o_idx++);
                            //the new points are inserted in bash at the end of this
                            //iteration. For this reason, the coordinates must be passed
                            //"manually" at this point (clipping_coords).
                            
                            //select_faces = true
                            IntersectionsVisitor iv(true);
                            //if (o.checkIntersections(input,inter_edges,clipping_coords[j]))
                            iv.setTriMesh(input);
                            iv.setFaces(inter_faces);
                            iv.setCoords(clipping_coords[j]);
                            
                            if (o.accept(&iv)) {
                                idx_pos_map[o.getIndex()] = processed.size();
                                processed.push_back(o);
                                
                            }
                            else {
                                //The element doesn't intersect any input edge.
                                //It must be checked if it's inside or outside.
                                //Only in the first case add it to new_Octrants.
                                //Test this with parent Octrant faces only.
                                if (isItIn(input,inter_faces,clipping_coords[j])) {
                                    idx_pos_map[o.getIndex()] = processed.size();
                                    processed.push_back(o);
                                }
                                else {
                                    //we must update neighbor information at the edges
                                    EdgeVisitor::removeOctFromEdges(&o, MapEdges);
                                }
                            }
                        }
                    }
                    //To mantain congruency in the map, we must erase all
                    //Octrants (index) that have been split due to balancing.
                    auto delOct = idx_pos_map.find(key);
                    idx_pos_map.erase(delOct);
                }
            }
            
            // don't forget to update list
            std::swap(candidates,new_candidates);
            
            if (!new_pts.empty()) {
                //add the new points to the vector
                points.reserve(points.size() + new_pts.size());
                points.insert(points.end(),new_pts.begin(),new_pts.end());
            }
            
            //clean non used Octs.
            for (auto used_Oct: idx_pos_map) {
                clean_processed.push_back(processed[used_Oct.second]);
                //pro_Octs++;
            }
            
            processed.erase(processed.begin(),processed.end());
            
        }
        
        //----------------------------------------------------------
        // apply transition patterns
        //----------------------------------------------------------
        //clear the new points in case Transition add some.
        new_pts.clear();
        
        //TransitionPatternVisitor section
        TransitionPatternVisitor tpv;
        tpv.setPoints(points);
        tpv.setNewPoints(new_pts);
        tpv.setMapEdges(MapEdges);
        if (localmax) {
            tpv.setMaxRefLevel(max_rl+1);
        }
        else {
            tpv.setMaxRefLevel(max_rl);
        }
        
        //Apply transition patterns to remaining Octs
        for (auto &to: clean_processed) {
            if (!to.accept(&tpv)) {
                std::cerr << "Error at Mesher::generateOcttreeMesh";
                std::cerr << " Transition Pattern not found\n";
            }
        }
        
        //Apply transition patterns to candidates because if balanced
        //neighbors were both refined, it will still be necessary to
        //apply the transition patterns to them.
        for (auto &to: candidates) {
            if (!to.accept(&tpv)) {
                std::cerr << "Error at Mesher::generateOcttreeMesh";
                std::cerr << " Transition Pattern not found\n";
            }
        }
        
        
        //if no points were added at this iteration, it is no longer
        //necessary to continue the refinement.
        if (!new_pts.empty()) {
            //add the new points to the vector
            points.reserve(points.size() + new_pts.size());
            points.insert(points.end(),new_pts.begin(),new_pts.end());
        }
        
        //insert will reserve space as well
        octants.insert(octants.end(),make_move_iterator(candidates.begin()),make_move_iterator(candidates.end()));
        // better to erase as let in a indeterminate state by move
        candidates.erase(candidates.begin(),candidates.end());
        octants.insert(octants.end(),make_move_iterator(clean_processed.begin()),make_move_iterator(clean_processed.end()));
        clean_processed.erase(clean_processed.begin(),clean_processed.end());
        
        //If there are more refinement regions now treat them
        if (!all_reg.empty()) {
            generateOctreeMesh(rl,input,all_reg,name,minrl,givenmaxrl);
        }
        
    }
    
    
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	void Mesher::generateOctreeMesh(const unsigned short &rl, TriMesh &input,
									list<RefinementRegion *> &all_reg,
                                    const string &name, const unsigned short &minrl,
                                    const unsigned short &givenmaxrl){
		
        auto start_time = chrono::high_resolution_clock::now();

        //The list of candidate Octs to refine and the tmp version of
        //adding those how are still candidates for the next iteration.
        list<Octant> candidates, new_candidates, clean_processed, refine_tmp;
        
        //The Octs that don't need further refinement.
        vector<Octant> processed;
        
        //A map that connect the index of processed Octs with their position in the
        //vector of processed Octs.
        map<unsigned int, unsigned int> idx_pos_map;
        
        //list of the points added at this refinement iteration:
        list<Point3D> new_pts;
        
        //A set containing the index of Octs to be refined to
        //maintain balancing
        list<pair<unsigned int,unsigned int> > toBalance;
        
        //initialising the vector and the map
        candidates.assign(make_move_iterator(octants.begin()),
                          make_move_iterator(octants.end()));
        
        //The starting point for assignating indexes
        unsigned int new_o_idx = candidates.size();
        
        //Erase previous Octrants to save memory
        octants.clear();
        
        //create visitors and give them variables
        SplitVisitor sv;
        sv.setPoints(points);
        sv.setMapEdges(MapEdges);
        sv.setNewPts(new_pts);
        sv.setProcessedOctVector(processed);
        sv.setMapProcessed(idx_pos_map);
        sv.setToBalanceList(toBalance);

        auto start_refine_octant_time = chrono::high_resolution_clock::now();

        auto start_refine_rl_time = chrono::high_resolution_clock::now();

        unsigned short max_rl;
        if (givenmaxrl==0) {
            max_rl = rl;
        }
        else {
            max_rl = givenmaxrl;
        }
        
        //cout << "minrl " << minrl << " maxrl " << max_rl << "\n";
        
        
        //list<unsigned int> ref_octs, bal_octs;
        
        
        
        //unsigned int oct_ref = 0, oct_bal = 0;

        auto end_refine_rl_time = chrono::high_resolution_clock::now();
        cout << "         * boundary max " << rl << " in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_refine_rl_time-start_refine_rl_time).count();
        cout << " ms"<< endl;

        //----------------------------------------------------------
        //refine each Octant until the Refinement Level is reached
        //----------------------------------------------------------
        
        //when producing a new mesh, minrl will be always 0. But as this
        //method can also be called from "refineMesh" in this last case
        //the starting refinement level will not be 0, but the min rl
        //among the Octrants in the starting mesh.
        for (unsigned short i=minrl; i<rl; i++) {
            auto start_refine_rl_time = chrono::high_resolution_clock::now();

            list<RefinementRegion *>::const_iterator reg_iter;
            
            //detect Octants to refine
            while (!candidates.empty()) {
                Octant oct = *(candidates.begin());
                candidates.pop_front();
                
                bool to_refine = false;
                
                for (reg_iter=all_reg.begin(); reg_iter!=all_reg.end(); ++reg_iter) {
                    
                    unsigned short region_rl = (*reg_iter)->getRefinementLevel();
                    if (region_rl<i) {
                        continue;
                    }
                    
                    //If the Octrant has a greater RL than the region needs, continue
                    if (region_rl<= oct.getRefinementLevel()) {
                        continue;
                    }
                    
                    //in the case of a Region Refinement (a surface) this will change
                    //the state of inregion = true.
                    if ((*reg_iter)->intersectsOctant(points,oct)) {
                        
                        
                        //oct_ref++;
                        //cout << " " << oct.getIndex() << "\n";
                        //EdgeVisitor::printInfo(&oct,MapEdges);
                        to_refine = true;
                    }
                }
                
                //now if refinement is not needed, we add the Octrant as it was.
                if (!to_refine) {
                    idx_pos_map[oct.getIndex()] = processed.size();
                    processed.push_back(oct);
                }
                else {
                    refine_tmp.push_back(oct);
                }
            }
            
            //now we can start to refine those needing it.
            for (auto oct: refine_tmp) {
                    
                list<unsigned int> &inter_faces = oct.getIntersectedFaces();
                unsigned short orl = oct.getRefinementLevel();
                
                vector<vector<Point3D> > clipping_coords;
                sv.setClipping(clipping_coords);
                
                vector<vector<unsigned int> > split_elements;
                sv.setNewEles(split_elements);
                sv.setStartIndex(new_o_idx);
                
                oct.accept(&sv);
                
                //bool reg_state = Oct.isInRegion();
                
                if (inter_faces.empty()) {
                    for (unsigned int j=0; j<split_elements.size(); j++) {
                        Octant o (split_elements[j], orl+1, new_o_idx++);
                        //o.setInRegionState(reg_state);
                        new_candidates.push_back(o);
                    }
                }
                else {
                    for (unsigned int j=0; j<split_elements.size(); j++) {
                        Octant o (split_elements[j],orl+1,new_o_idx++);
                        //the new points are inserted in bash at the end of this
                        //iteration. For this reason, the coordinates must be passed
                        //"manually" at this point (clipping_coords).
                        
                        //select_faces = true
                        IntersectionsVisitor iv(true);
                        //if (o.checkIntersections(input,inter_edges,clipping_coords[j]))
                        iv.setTriMesh(input);
                        iv.setFaces(inter_faces);
                        iv.setCoords(clipping_coords[j]);
                        
                        if (o.accept(&iv)) {
                            //o.setInRegionState(reg_state);
                            new_candidates.push_back(o);
                        }
                        else {
                            //The element doesn't intersect any input edge.
                            //It must be checked if it's inside or outside.
                            //Only in the first case add it to new_Octrants.
                            //Test this with parent Octrant faces only.
                            if (isItIn(input,inter_faces,clipping_coords[j])) {
                                //o.setInRegionState(reg_state);
                                new_candidates.push_back(o);
                            }
                            else {
                                //we must update neighbor information at the edges
                                EdgeVisitor::removeOctFromEdges(&o, MapEdges);
                                
                            }
                        }
                    }
                }
            }

            auto end_refine_rl_time = chrono::high_resolution_clock::now();
            cout << "         * level " << i << " (#" << new_candidates.size() << ") in "
            << std::chrono::duration_cast<chrono::milliseconds>(end_refine_rl_time-start_refine_rl_time).count();
            cout << " ms";

            //Erase the list to refine
            refine_tmp.erase(refine_tmp.begin(),refine_tmp.end());
            
            //Refine non balanced Octs
            while (!toBalance.empty()) {
                
                list<pair<unsigned int, unsigned int> > tmp_toBalance;
                std::swap(toBalance,tmp_toBalance);
                tmp_toBalance.sort();
                tmp_toBalance.unique();
                
                while (!tmp_toBalance.empty()) {
                    unsigned int key = tmp_toBalance.begin()->first;
                    unsigned int val = tmp_toBalance.begin()->second;
                    tmp_toBalance.pop_front();
                    
                    auto delOct = idx_pos_map.find(key);
                    if (delOct == idx_pos_map.end()) {
                        //Note: let us say that Oct N is in the tmp_to_balance list at current
                        //iteration. Before arriving to it, a neighbor of N is refined to such
                        //level that adds N to list to_balance (for the next iteration). However
                        //as N is in the current tmp_to_balance it will be removed from the
                        //map and in next iteration it will be attempt to be split and removed
                        //once more in the mesh. Therefore, this if avoids this case.
                        continue;
                    }

                    
                    Octant oct = processed[val];
                    list<unsigned int> &inter_faces = oct.getIntersectedFaces();
                    unsigned short orl = oct.getRefinementLevel();
                    
                    vector<vector<Point3D> > clipping_coords;
                    sv.setClipping(clipping_coords);
                    
                    vector<vector<unsigned int> > split_elements;
                    sv.setNewEles(split_elements);
                    sv.setStartIndex(new_o_idx);
                    
                    oct.accept(&sv);
                    
                    if (inter_faces.empty()) {
                        for (unsigned int j=0; j<split_elements.size(); j++) {
                            Octant o (split_elements[j], orl+1, new_o_idx++);
                            idx_pos_map[o.getIndex()] = processed.size();
                            processed.push_back(o);
                        }
                    }
                    else {
                        for (unsigned int j=0; j<split_elements.size(); j++) {
                            Octant o (split_elements[j],orl+1,new_o_idx++);
                            //the new points are inserted in bash at the end of this
                            //iteration. For this reason, the coordinates must be passed
                            //"manually" at this point (clipping_coords).
                            
                            //select_faces = true
                            IntersectionsVisitor iv(true);
                            //if (o.checkIntersections(input,inter_edges,clipping_coords[j]))
                            iv.setTriMesh(input);
                            iv.setFaces(inter_faces);
                            iv.setCoords(clipping_coords[j]);
                            
                            if (o.accept(&iv)) {
                                idx_pos_map[o.getIndex()] = processed.size();
                                processed.push_back(o);
                                
                            }
                            else {
                                //The element doesn't intersect any input edge.
                                //It must be checked if it's inside or outside.
                                //Only in the first case add it to new_Octrants.
                                //Test this with parent Octrant faces only.
                                if (isItIn(input,inter_faces,clipping_coords[j])) {
                                    idx_pos_map[o.getIndex()] = processed.size();
                                    processed.push_back(o);
                                }
                                else {
                                    //we must update neighbor information at the edges
                                    EdgeVisitor::removeOctFromEdges(&o, MapEdges);

                                }
                            }
                        }
                    }
                    //To mantain congruency in the map, we must erase all
                    //Octrants (index) that have been split due to balancing.
                    if (delOct != idx_pos_map.end()) {
                        idx_pos_map.erase(delOct);
                    }
                }
            }
            
            // don't forget to update list
            std::swap(candidates,new_candidates);
            
            //if no points were added at this iteration, it is no longer
            //necessary to continue the refinement.
            if (new_pts.empty()) {
                cout << "warning at Mesher::generateOcttreeMesh no new points!!!\n";
                auto end_balanced_time = chrono::high_resolution_clock::now();
                cout << " - Balanced in "
                << std::chrono::duration_cast<chrono::milliseconds>(end_balanced_time-end_refine_rl_time).count();
                cout << " ms"<< endl;
                //cout << "i: " << i << " minrl " << minrl << " maxrl " << max_rl << "\n";
                break;
            }
            
            //add the new points to the vector
            points.reserve(points.size() + new_pts.size());
            points.insert(points.end(),new_pts.begin(),new_pts.end());
            
            //the new_pts is a list that holds the coordinates of
            //new points inserted at this iteration. At the end of
            //this bucle, they are inserted in the point vector
            //new_pts.erase(new_pts.begin(),new_pts.end());
            new_pts.clear();
            
            
            /*auto bug = MapEdges.find(OctreeEdge (933,987));
            
            if (bug!=MapEdges.end()) {
                cout << " State of (933,987)";
                for (unsigned int m=0;m<5;m++) {
                    cout << " " << bug->second[m];
                }
                cout << "\n";
            }
            
            //Check incorrect balancing.
            OneIrregularVisitor oiv;
            oiv.setMapEdges(MapEdges);
            oiv.setMaxRefLevel(i);
            
            cout << "  * checking balance at iteration " << i << "\n";
            
            for (auto used_Oct: idx_pos_map) {
                Octant oe = processed[used_Oct.second];
                //Check balancing
                oe.accept(&oiv);
            }
            for (auto oe: candidates) {
                if (oe.getIndex()==82 || oe.getIndex()==158) {
                    cout << "for octant " << oe.getIndex();
                    for (unsigned int m=0; m<8; m++) {
                        cout << " " << oe.getPoints()[m];
                    }
                    cout << "\n";
                }
            }*/

            auto end_balanced_time = chrono::high_resolution_clock::now();
            cout << " - Balanced in "
            << std::chrono::duration_cast<chrono::milliseconds>(end_balanced_time-end_refine_rl_time).count();
            cout << " ms"<< endl;
        }
        
        
        //cout << "Refined octants: " << oct_ref << "\n";
        //cout << "Balanced octants: " << oct_ref << "\n";
        
        
        //clean non used Octs.
        for (auto used_Oct: idx_pos_map) {
            clean_processed.push_back(processed[used_Oct.second]);
        }
        
        processed.erase(processed.begin(),processed.end());
        
        auto end_refine_octant_time = chrono::high_resolution_clock::now();
        cout << "       * Refine Octant (#" << candidates.size() << ") in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_refine_octant_time-start_refine_octant_time).count();
        cout << " ms"<< endl;

        //----------------------------------------------------------
        // apply transition patterns
        //----------------------------------------------------------
        
        //TransitionPatternVisitor section
        TransitionPatternVisitor tpv;
        tpv.setPoints(points);
        tpv.setNewPoints(new_pts);
        tpv.setMapEdges(MapEdges);
        tpv.setMaxRefLevel(max_rl);


        //Apply transition patterns to remaining Octs
        for (auto &to: clean_processed) {
            if (!to.accept(&tpv)) {
                std::cerr << "Error at Mesher::generateOcttreeMesh";
                std::cerr << " Transition Pattern not found\n";
            }
        }
        
        //if no points were added at this iteration, it is no longer
        //necessary to continue the refinement.
        if (!new_pts.empty()) {
            //add the new points to the vector
            points.reserve(points.size() + new_pts.size());
            points.insert(points.end(),new_pts.begin(),new_pts.end());
        }
        
        //insert will reserve space as well
        octants.insert(octants.end(),make_move_iterator(candidates.begin()),make_move_iterator(candidates.end()));
        // better to erase as let in a indeterminate state by move
        candidates.erase(candidates.begin(),candidates.end());
        octants.insert(octants.end(),make_move_iterator(clean_processed.begin()),make_move_iterator(clean_processed.end()));
        clean_processed.erase(clean_processed.begin(),clean_processed.end());

        auto end_time = chrono::high_resolution_clock::now();
        cout << "       * Transition Patterns (#" << octants.size() << ") in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-end_refine_octant_time).count();
        cout << " ms"<< endl;
        cout << "    * generateOctreeMesh in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
        cout << " ms"<< endl;
    }

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    bool Mesher::isItIn(TriMesh &mesh, list<unsigned int> &faces, vector<Point3D> &coords) {
        //this method is meant to be used by octants that don't
        //intersect input domains. If they are inside of at least
        //one input mesh, then they must remain in the output mesh.
        bool first = mesh.pointIsInMesh(coords[0],faces);
        bool second = mesh.pointIsInMesh(coords[1],faces);
        if (first==second) {
            return first;
        }

        //cout << "one incosistency detected -> hard test\n";
        //return mesh.pointIsInMesh(coords[0],faces);
        return mesh.pointIsInMesh(coords[0]);
    }
    
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	unsigned int Mesher::saveOutputMesh(FEMesh &mesh){
        
        vector<Point3D> out_pts;
        vector<vector<unsigned int> > out_els;
        list<vector<unsigned int> >tmp_els;
        
        //new_idxs will hold the index of used nodes in the outside vector for points.
        //If the a node is not used by any element, its index will be 0 in this vector,
        //therefore the actual index is shiffted in 1. In other words, node 0 is node 1,
        //and node n is node n+1.
        vector<unsigned int> new_idxs (points.size(),0);
        unsigned int out_node_count = 0;
        list<Point3D> out_points_tmp;
        
        //recompute node indexes and update elements with them.
        for (unsigned int i=0; i<octants.size(); i++) {
            vector<vector<unsigned int> > sub_els= octants[i].getSubElements();
            for (unsigned int j=0; j<sub_els.size(); j++) {
                
                vector<unsigned int> sub_ele_new_idxs = sub_els[j];
                for (unsigned int k=0; k<sub_ele_new_idxs.size();k++) {
                    
                    unsigned int p_idx = sub_ele_new_idxs[k];
                    
                    if (new_idxs[p_idx]==0) {
                        sub_ele_new_idxs[k] = out_node_count++;
                        new_idxs[p_idx]=out_node_count;
                        out_points_tmp.push_back(points[p_idx].getPoint());
                    }
                    else {
                        sub_ele_new_idxs[k] = new_idxs[p_idx]-1;
                    }
                }
                tmp_els.push_back(sub_ele_new_idxs);
            }
        }
        
        //write output elements
        out_els.reserve(tmp_els.size());
        list<vector<unsigned int> >::iterator iter;
        for (iter=tmp_els.begin(); iter!=tmp_els.end(); iter++) {
            out_els.push_back(*iter);
        }
        
        //write output points
        list<Point3D>::iterator opi;
        out_pts.reserve(out_points_tmp.size());
        for (opi=out_points_tmp.begin(); opi!=out_points_tmp.end(); opi++) {
            out_pts.push_back(*opi);
        }
        
        mesh.setPoints(out_pts);
        mesh.setElements(out_els);
        return out_els.size();
	}
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    unsigned int Mesher::saveOutputMesh(const shared_ptr<FEMesh> &mesh){
        
        vector<Point3D> out_pts;
        vector<vector<unsigned int> > out_els;
        list<vector<unsigned int> >tmp_els;
        
        //new_idxs will hold the index of used nodes in the outside vector for points.
        //If the a node is not used by any element, its index will be 0 in this vector,
        //therefore the actual index is shiffted in 1. In other words, node 0 is node 1,
        //and node n is node n+1.
        vector<unsigned int> new_idxs (points.size(),0);
        unsigned int out_node_count = 0;
        list<Point3D> out_points_tmp;
        
        //recompute node indexes and update elements with them.
        for (unsigned int i=0; i<octants.size(); i++) {
            vector<vector<unsigned int> > sub_els= octants[i].getSubElements();
            for (unsigned int j=0; j<sub_els.size(); j++) {
                
                vector<unsigned int> sub_ele_new_idxs = sub_els[j];
                for (unsigned int k=0; k<sub_ele_new_idxs.size();k++) {
                    
                    unsigned int p_idx = sub_ele_new_idxs[k];
                    
                    if (new_idxs[p_idx]==0) {
                        sub_ele_new_idxs[k] = out_node_count++;
                        new_idxs[p_idx]=out_node_count;
                        out_points_tmp.push_back(points[p_idx].getPoint());
                    }
                    else {
                        sub_ele_new_idxs[k] = new_idxs[p_idx]-1;
                    }
                }
                tmp_els.push_back(sub_ele_new_idxs);
            }
        }
        
        //write output elements
        out_els.reserve(tmp_els.size());
        list<vector<unsigned int> >::iterator iter;
        for (iter=tmp_els.begin(); iter!=tmp_els.end(); iter++) {
            out_els.push_back(*iter);
        }
        
        //write output points
        list<Point3D>::iterator opi;
        out_pts.reserve(out_points_tmp.size());
        for (opi=out_points_tmp.begin(); opi!=out_points_tmp.end(); opi++) {
            out_pts.push_back(*opi);
        }
        
        mesh->setPoints(out_pts);
        mesh->setElements(out_els);
        return out_els.size();
    }
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    unsigned int Mesher::saveOutputMesh(FEMesh &mesh,vector<MeshPoint> &tmp_points,
                                        list<Octant> &tmp_octants){
        
        vector<Point3D> out_pts;
        list<vector<unsigned int> > tmp_elements;
        vector<vector<unsigned int> > out_els;
        
        unsigned int n = tmp_points.size();
        out_pts.reserve(n);
        for (unsigned int i=0; i<n; i++) {
            out_pts.push_back(points[i].getPoint());
        }
        
        list<Octant>::iterator o_iter;
        
        for (o_iter=tmp_octants.begin(); o_iter!=tmp_octants.end(); o_iter++) {
            
            vector<vector<unsigned int> > sub_els= o_iter->getSubElements();
            for (unsigned int j=0; j<sub_els.size(); j++) {
                tmp_elements.push_back(sub_els[j]);
            }
        }
        
        out_els.reserve(tmp_elements.size());
        list<vector<unsigned int> >::iterator e_iter;
        
        for (e_iter=tmp_elements.begin(); e_iter!=tmp_elements.end(); e_iter++) {
            out_els.push_back(*e_iter);
        }
        
        mesh.setPoints(out_pts);
        mesh.setElements(out_els);
        return out_els.size();
    }
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	unsigned int Mesher::saveOutputMesh(const shared_ptr<FEMesh> &mesh,
                                        vector<MeshPoint> &tmp_points,
                                        list<Octant> &tmp_octants, vector<Octant> &rest_octs){
		
		vector<Point3D> out_pts;
        list<vector<unsigned int> > tmp_elements;
		vector<vector<unsigned int> > out_els;
        
        unsigned int tne = tmp_octants.size() + rest_octs.size();
        
        vector<unsigned short > deco_rl, deco_surf, deco_deb (tne,0);
        
        deco_surf.reserve(tne);
        deco_rl.reserve(tne);
        deco_deb.assign(tmp_octants.size(),1);
		
		unsigned int n = tmp_points.size();
		out_pts.reserve(n);
		for (unsigned int i=0; i<n; i++) {
			out_pts.push_back(points[i].getPoint());
		}
		
        //Write the elements of the list first.
        for (auto oct: tmp_octants) {
            vector<vector<unsigned int> > sub_els= oct.getSubElements();
            for (unsigned int j=0; j<sub_els.size(); j++) {
                tmp_elements.push_back(sub_els[j]);
            }
            if (oct.getIntersectedFaces().size()!=0) {
                deco_surf.push_back(1);
            }
            else {
                deco_surf.push_back(0);
            }
            deco_rl.push_back(oct.getRefinementLevel());
		}
        
        //Now continue with the rest
        for (auto oct: rest_octs) {
            vector<vector<unsigned int> > sub_els= oct.getSubElements();
            for (unsigned int j=0; j<sub_els.size(); j++) {
                tmp_elements.push_back(sub_els[j]);
            }
            if (oct.getIntersectedFaces().size()!=0) {
                deco_surf.push_back(1);
            }
            else {
                deco_surf.push_back(0);
            }
            deco_rl.push_back(oct.getRefinementLevel());
        }
        
        out_els.reserve(tmp_elements.size());
        list<vector<unsigned int> >::iterator e_iter;
		
        for (e_iter=tmp_elements.begin(); e_iter!=tmp_elements.end(); e_iter++) {
            out_els.push_back(*e_iter);
        }
        
		mesh->setPoints(out_pts);
		mesh->setElements(out_els);
        mesh->setRefLevels(deco_rl);
        mesh->setSurfState(deco_surf);
        mesh->setDebugging(deco_deb);
        
        return out_els.size();
	}
    
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
    unsigned int Mesher::saveOutputMesh(const shared_ptr<FEMesh> &mesh,
                                        vector<MeshPoint> &tmp_points,
                                        list<Octant> &tmp_octants, list<Octant> &rest_octs){
        
        vector<Point3D> out_pts;
        list<vector<unsigned int> > tmp_elements;
        vector<vector<unsigned int> > out_els;
        
        unsigned int tne = tmp_octants.size() + rest_octs.size();
        
        vector<unsigned short > deco_rl, deco_surf, deco_deb (tne,0);
        
        deco_surf.reserve(tne);
        deco_rl.reserve(tne);
        deco_deb.assign(tmp_octants.size(),1);
        
        unsigned int n = tmp_points.size();
        out_pts.reserve(n);
        for (unsigned int i=0; i<n; i++) {
            out_pts.push_back(points[i].getPoint());
        }
        
        //Write the elements of the list first.
        for (auto oct: tmp_octants) {
            vector<vector<unsigned int> > sub_els= oct.getSubElements();
            for (unsigned int j=0; j<sub_els.size(); j++) {
                tmp_elements.push_back(sub_els[j]);
            }
            if (oct.getIntersectedFaces().size()!=0) {
                deco_surf.push_back(1);
            }
            else {
                deco_surf.push_back(0);
            }
            deco_rl.push_back(oct.getRefinementLevel());
        }
        
        //Now continue with the rest
        for (auto oct: rest_octs) {
            vector<vector<unsigned int> > sub_els= oct.getSubElements();
            for (unsigned int j=0; j<sub_els.size(); j++) {
                tmp_elements.push_back(sub_els[j]);
            }
            if (oct.getIntersectedFaces().size()!=0) {
                deco_surf.push_back(1);
            }
            else {
                deco_surf.push_back(0);
            }
            deco_rl.push_back(oct.getRefinementLevel());
        }
        
        out_els.reserve(tmp_elements.size());
        list<vector<unsigned int> >::iterator e_iter;
        
        for (e_iter=tmp_elements.begin(); e_iter!=tmp_elements.end(); e_iter++) {
            out_els.push_back(*e_iter);
        }
        
        mesh->setPoints(out_pts);
        mesh->setElements(out_els);
        mesh->setRefLevels(deco_rl);
        mesh->setSurfState(deco_surf);
        mesh->setDebugging(deco_deb);
        
        return out_els.size();
    }
    
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    
	void Mesher::linkElementsToNodes(){
        auto start_time = chrono::high_resolution_clock::now();

        //clear previous information
		for (unsigned int i=0; i<points.size(); i++) {
			points.at(i).clearElements();
		}
        
        //cout << "number of octants " << octants.size() << "\n";
		
		//link element info to nodes
		for (unsigned int i=0; i<octants.size(); i++) {
			vector <unsigned int> o_pts = octants[i].getPoints();
			
			for (unsigned int j=0; j<o_pts.size(); j++) {
				points.at(o_pts[j]).addElement(i);
			}
		}
        auto end_time = chrono::high_resolution_clock::now();
        cout << "    * linkElementsToNodes in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
        cout << " ms"<< endl;
    }
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	void Mesher::detectInsideNodes(TriMesh &input){
        auto start_time = chrono::high_resolution_clock::now();

        for (unsigned int i=0; i<points.size(); i++) {
			if (points[i].wasOutsideChecked()) {
				continue;
			}
			
			list<unsigned int> p_eles = points[i].getElements(),p_faces;
			points[i].outsideChecked();
			if (p_eles.empty()) {
				continue;
			}
			list<unsigned int>::iterator iter;
			for (iter=p_eles.begin(); iter!=p_eles.end(); iter++) {
				list<unsigned int> o_faces= octants[*iter].getIntersectedFaces();
				list<unsigned int>::iterator of_iter;
				if (o_faces.empty()) {
					continue;
				}
				for (of_iter=o_faces.begin(); of_iter!=o_faces.end(); of_iter++) {
					p_faces.push_back(*of_iter);
				}
			}
			
			p_faces.sort();
			p_faces.unique();
			
			if (p_faces.empty() || input.pointIsInMesh(points[i].getPoint(),p_faces)) {
				points[i].setInside();
			}
		}
        auto end_time = chrono::high_resolution_clock::now();
        cout << "    * detectInsideNodes in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
        cout << " ms"<< endl;
    }
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	void Mesher::removeOnSurface(){
        auto start_time = chrono::high_resolution_clock::now();

		list<Octant> newele,removed;
		list<Octant>::iterator eiter;
        RemoveSubElementsVisitor rsv;
        rsv.setPoints(points);
		//remove elements without an inside node.
		for (unsigned int i=0; i<octants.size(); i++) {
			if (octants[i].isInside()) {
				newele.push_back(octants[i]);
				continue;
			}

            //if (octants[i].removeOutsideSubElements(points)) {
            if (octants[i].accept(&rsv)) {
                removed.push_back(octants[i]);
            }
            else {
                newele.push_back(octants[i]);
            }
            
			/*bool onein = false;
             vector<unsigned int> epts = octants[i].getPoints();
             
             for (unsigned int j=0; j< epts.size(); j++) {
             if (points.at(epts[j]).isInside()) {// &&
             //!points.at(epts[j]).wasProjected()) {
             onein = true;
             break;
             }
             }
             
             if (onein) {
             newele.push_back(octants[i]);
             }
             else {
             removed.push_back(octants[i]);
             }*/
		}
		
		if (removed.empty()) {
            auto end_time = chrono::high_resolution_clock::now();
            cout << "    * RemoveOnSurface in "
            << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
            cout << " ms"<< endl;
            return;
		}
		
		//clear removed elements
		removed.clear();
		//now element std::list from Vomule mesh can be cleared, as all remaining
		//elements are still in use and attached to newele std::list.
		octants.clear();
		for (eiter = newele.begin(); eiter!=newele.end(); eiter++) {
			octants.push_back(*eiter);
		}

        auto end_time = chrono::high_resolution_clock::now();
        cout << "    * RemoveOnSurface in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
        cout << " ms"<< endl;
    }
    
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	void Mesher::applySurfacePatterns(TriMesh &input){
        auto start_time = chrono::high_resolution_clock::now();

        //apply patterns to avoid flat, invalid and
		//poor quality elements.
		list<MeshPoint> tmppts;
		list<MeshPoint>::iterator piter;

        SurfaceTemplatesVisitor stv;
        stv.setPoints(points);
        stv.setInput(input);

		for (unsigned int i=0; i<octants.size(); i++) {
			
            
            
            //El siguiente if debe estar activo al final. Problema
            //con que nuevos Octantes internos y sub-divididos no
            //son considerados como de superficie.
            
            
            
            
			if (octants[i].isSurface()) {
                stv.setNewPoints(tmppts);
                stv.setIdx(i);
				if (!octants[i].accept(&stv)) {
					cout << "Error in Mesher::applySurfacePatterns: coultd't apply";
					cout << " a surface pattern\n";
					cout << octants[i] << "\n";
					continue;
				}
			}
		}
		
		//add new nodes to the vector meshPoints.
		if (!tmppts.empty()) {
			unsigned int npts = points.size()+tmppts.size();
			points.reserve(npts);
			for (piter=tmppts.begin(); piter!=tmppts.end(); piter++) {
				points.push_back(*piter);
			}
		}

        auto end_time = chrono::high_resolution_clock::now();
        cout << "    * ApplySurfacePatterns in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
        cout << " ms"<< endl;
    }
	
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	//shrink elements intersecting the envelope defined by all
	//input surfaces
	
	void Mesher::shrinkToBoundary(TriMesh &input){
        auto start_time = chrono::high_resolution_clock::now();

		//Slow element removed (but works): from elements intersecting the
		//input domain, detect inner nodes. Project this nodes onto the
		//surface. If after all is done, if an element counts only with "on
		//surface" and "outside" nodes, remove it.
		list<unsigned int> out_nodes;
		list<Octant>::iterator oiter;
        
		for (unsigned int i=0; i<octants.size(); i++) {
			if (octants[i].isInside()) {
				continue;
			}
			
			//Put in a std::list inside nodes of boundary elements that
			//may be projected to the input domain.
			vector<unsigned int> epts = octants[i].getPoints();
			for (unsigned int j=0; j < epts.size(); j++) {
				
				if (!points[epts[j]].wasOutsideChecked()) {
					cout << "error!!! in Mesher::shrinkToBoundary\n";
                    cout << "  some nodes were not outside checked (";
					cout << epts.size() << "): " << epts[j] << " (" << j << ")\n";
					
					points[epts[j]].outsideChecked();
					Point3D oct_p = points.at(epts[j]).getPoint();
					if (input.pointIsInMesh(oct_p,octants[i].getIntersectedFaces())) {
						points[epts[j]].setInside();
					}
				}
				
				if (points[epts[j]].wasProjected()) {
					continue;
				}
				
				if (points[epts[j]].isOutside()) {
					out_nodes.push_back(epts[j]);
				}
			}
		}
		
		out_nodes.sort();
		out_nodes.unique();
		
		//project all outside points onto the surface
		std::list<unsigned int>::iterator piter;
		
		for (piter=out_nodes.begin(); piter!=out_nodes.end(); piter++) {
            
			//get the faces of octants sharing this node
			list<unsigned int> p_faces, p_eles = points.at(*piter).getElements();
			list<unsigned int>::iterator p_eiter;
			
			for (p_eiter=p_eles.begin(); p_eiter!=p_eles.end(); p_eiter++) {
				list<unsigned int> o_faces = octants[*p_eiter].getIntersectedFaces();
				list<unsigned int>::iterator oct_fcs;
				for (oct_fcs=o_faces.begin(); oct_fcs!=o_faces.end(); oct_fcs++) {
					p_faces.push_back(*oct_fcs);
				}
			}
			
			p_faces.sort();
			p_faces.unique();
			
			if (p_faces.empty()) {
				cout << "\nWarning at Mesher::shrinkToBoundary";
				cout << " no faces to project an outside node\n";
				cout << *piter << " n_els " << p_eles.size() << ":";
				for (p_eiter=p_eles.begin(); p_eiter!=p_eles.end(); p_eiter++) {
					cout << " " << *p_eiter;
				}
				cout << "\n";
				continue;
			}
			
			Point3D current = points.at(*piter).getPoint();
			Point3D projected = input.getProjection(current,p_faces);
			
			/*for (p_eiter=p_eles.begin(); p_eiter!=p_eles.end(); p_eiter++) {
				octants[*p_eiter].addProjectionInfluence(projected-current);
			}*/
			
			points.at(*piter).setPoint(projected);
			points.at(*piter).setProjected();
		}
		
		//Shrink inside nodes with respect to outside ones.
		/*double factor = 0.75;
         for (unsigned int i=0; i<3; i++) {
         
         //detect all internal nodes to be displaced in this iteration.
         list<unsigned int> to_move, to_reset;
         list<unsigned int>::iterator o_iter, p_iter;
         for (unsigned int j=0; j<octants.size(); j++) {
         if (octants[j].wasShrink()) {
         octants[j].noMoreProjectionInfluences();
         to_reset.push_back(j);
         vector<unsigned int> o_pts = octants[j].getPoints();
         for (unsigned int k=0; k<o_pts.size(); k++) {
         if (!points.at(o_pts[k]).wasProjected()) {
         to_move.push_back(o_pts[k]);
         }
         }
         }
         }
         
         if (to_move.empty()) {
         //cout << "   > no one else to move\n";
         break;
         }
         
         //move each node only once per iteration.
         to_move.sort();
         to_move.unique();
         
         //cout << "   > moving " << to_move.size() << " inside nodes\n";
         
         for (p_iter=to_move.begin(); p_iter!=to_move.end(); p_iter++) {
         list<unsigned int> p_eles = points[*p_iter].getElements();
         Point3D p_to_add;
         unsigned short qty = 0;
         for (o_iter=p_eles.begin(); o_iter!=p_eles.end(); o_iter++) {
         if (!octants[*o_iter].wasConsideredInProjection()) {
         continue;
         }
         p_to_add += octants[*o_iter].getProjectionInfluence();
         qty++;
         }
         
         p_to_add = p_to_add * (factor/(qty));
         //use this for less displacement of internal nodes.
         //p_to_add = p_to_add * (factor/(2*qty));
         
         for (o_iter=p_eles.begin(); o_iter!=p_eles.end(); o_iter++) {
         if (octants[*o_iter].wasConsideredInProjection()) {
         continue;
         }
         octants[*o_iter].addProjectionInfluence(p_to_add);
         }
         p_to_add += points[*p_iter].getPoint();
         points.at(*p_iter).setPoint(p_to_add);
         points.at(*p_iter).setProjected();
         }
         
         //clear information over already shrunk octants
         for (o_iter=to_reset.begin(); o_iter!=to_reset.end(); o_iter++) {
         octants[*o_iter].resetProjectionInfluence();
         }
         
         factor -= 0.25;
         }*/
        
		//cout.flush();
        auto end_time = chrono::high_resolution_clock::now();
        cout << "    * ShrinkToBoundary in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
        cout << " ms"<< endl;
    }
	
	
	//--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
	
	void Mesher::projectCloseToBoundaryNodes(TriMesh &input){
        auto start_time = chrono::high_resolution_clock::now();

        //Slow element removed (but works): from elements intersecting the
        //input domain, detect inner nodes. Project this nodes onto the
        //surface. If after all is done, if an element counts only with "on
        //surface" and "outside" nodes, remove it.
        list<unsigned int> in_nodes;
        list<Octant>::iterator oiter;
        
        for (unsigned int i=0; i<octants.size(); i++) {
            if (!octants[i].isSurface()) {
                continue;
            }
            octants[i].computeMaxDistance(points);
        }
        
        for (unsigned int i=0; i<octants.size(); i++) {

            if (octants[i].isInside()) {
                continue;
            }
            
            //Put in a std::list inside nodes of boundary elements that
            //may be projected to the input domain.
            vector<unsigned int> epts = octants[i].getPoints();
            for (unsigned int j=0; j < epts.size(); j++) {
                
                if (!points[epts[j]].wasOutsideChecked()) {
                    cout << "error!! in Mesher::projectCloseToBoundaryNodes\n";
                    cout << "  point wasn't outside checked\n";
                    
                    points[epts[j]].outsideChecked();
                    Point3D oct_p = points.at(epts[j]).getPoint();
                    
                    if (input.pointIsInMesh(oct_p,octants[i].getIntersectedFaces())) {
                        points[epts[j]].setInside();
                    }
                }
                if (points[epts[j]].isInside()) {
                    in_nodes.push_back(epts[j]);
                    double md = octants[i].getMaxDistance();
                    if (j>7) {
                        md*=0.5;
                    }
                    points[epts[j]].setMaxDistance(md);
                }
            }
            
        }
        
        in_nodes.sort();
        in_nodes.unique();
        
        //move (when possible) all inner points to surface
        std::list<unsigned int>::iterator piter;
        
        for (piter=in_nodes.begin(); piter!=in_nodes.end(); piter++) {
            
            //if this node is attached to an octant which was split in
            //mixed-elements due to transition patterns, avoid the
            //displacement.
            
            //get the faces of octants sharing this node
            list<unsigned int> o_faces,p_faces, p_eles = points.at(*piter).getElements();
            list<unsigned int>::iterator peiter,oct_fcs;
            
            //bool trans_pattern = false;
            
            for (peiter=p_eles.begin(); peiter!=p_eles.end(); peiter++) {
                o_faces = octants[*peiter].getIntersectedFaces();
                for (oct_fcs=o_faces.begin(); oct_fcs!=o_faces.end(); oct_fcs++) {
                    p_faces.push_back(*oct_fcs);
                }
            }
            
            p_faces.sort();
            p_faces.unique();
            
            Point3D current = points.at(*piter).getPoint();
            Point3D projected = input.getProjection(current,p_faces);
            double dis = (current - projected).Norm();
            
            if(dis<points[*piter].getMaxDistance()){
                //this node have been moved to boundary, thus every element
                //sharing this node must be set as a border element in order
                //to avoid topological problems.
                //points.at(*piter).setOutside();
                points.at(*piter).setProjected();
                points.at(*piter).setPoint(projected);

                for (peiter=p_eles.begin(); peiter!=p_eles.end(); peiter++) {
                    octants[*peiter].setSurface();
                }
            }
        }
        
        /*insurf.sort();
        insurf.unique();
        
        //CL Debbuging
        {
            //save pure octree mesh
            std::shared_ptr<FEMesh> toref = make_shared<FEMesh>();
            vector<Octant> vvoid;
            list<Octant> inmov;
            for (auto inidx: insurf) {
                inmov.push_back(octants[inidx]);
            }
            saveOutputMesh(toref,points,inmov,vvoid);
            string tmp_name = "inregsurf";
            Services::WriteVTK(tmp_name,toref);
        }*/
        auto end_time = chrono::high_resolution_clock::now();
        cout << "    * ProjectCloseToBoundary in "
        << std::chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count();
        cout << " ms"<< endl;
    }
}
