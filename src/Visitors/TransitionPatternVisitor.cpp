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

#include "TransitionPatternVisitor.h"
#include "../Octant.h"

namespace Clobscode
{

    TransitionPatternVisitor::TransitionPatternVisitor() {
        points = NULL;
        new_pts = NULL;
        edges = NULL;
        max_ref_level = NULL;
        apply_pattern = false;
    }

    TransitionPatternVisitor::TransitionPatternVisitor(bool apply_pattern) {
        points = NULL;
        new_pts = NULL;
        edges = NULL;
        max_ref_level = NULL;
        this->apply_pattern = apply_pattern;
    }

    void TransitionPatternVisitor::setPoints(vector<MeshPoint> &points) {
        this->points = &points;
    }

    void TransitionPatternVisitor::setNewPoints(list<Point3D> &new_pts) {
        this->new_pts = &new_pts;
    }

    void TransitionPatternVisitor::setEdges(const set<OctreeEdge> &edges) {
        this->edges = &edges;
    }

    void TransitionPatternVisitor::setMaxRefLevel(const unsigned short &max_ref_level) {
        this->max_ref_level = &max_ref_level;
    }

    void TransitionPatternVisitor::setApplyMode(bool apply_pattern){
        this->apply_pattern = apply_pattern;
    }


    bool TransitionPatternVisitor::visit(Octant *o) {
        //cout << "TransitionPatternVisitor" << endl;
        //applyTransitionPattern
        if (apply_pattern)
        {
            if (new_pts == NULL)
                throw std::runtime_error(std::string("TransitionPatternVisitor: need new_pts (calling apply instead of check?)"));
            //if this octant is refined to the maximum level, return it immediately
            if (*max_ref_level == o->ref_level) {
                return true;
            }

            vector<unsigned int> &pointindex = o->pointindex;
            EdgeVisitor ev;

            vector<unsigned int> nodes, mid_nodes, tmp_nodes(12,0);
            list<unsigned int> l_nodes, l_mid_nodes;
            //insert the 8 nodes of the cube.
            for (unsigned int i=0; i<8; i++) {
                l_nodes.push_back(pointindex[i]);
            }
            //search for nodes inserted in edges
            for (unsigned int i=0; i<12; i++) {
                OctreeEdge e;
                ev.getEdge(o,i,e);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge==edges->end()) {
                    cout << "  edge " << e << " not found at applyTransitionPattern\n";
                }
                else {
                    if ((*my_edge)[2]!=0) {
                        l_nodes.push_back((*my_edge)[2]);
                        l_mid_nodes.push_back(i+8);
                        tmp_nodes[i]=(*my_edge)[2];
                    }
                }
            }
            //if this elements do not present nodes inserted in its edges
            //then return true (meaning this case is already considered in
            //the transition patterns) and add this element to the vector
            //of "new elements"
            if (l_mid_nodes.empty()) {
                return true;
            }
            //at this point the element has at least one node inserted in
            //an edge and we need to search for nodes inserted as middle
            //nodes of their faces

            if (tmp_nodes[0]!=0 && tmp_nodes[2]!=0) {
                //search for node 20
                OctreeEdge e(tmp_nodes[0],tmp_nodes[2]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(20);
                }
            }
            if (tmp_nodes[0]!=0 && tmp_nodes[8]!=0) {
                //search for node 21
                OctreeEdge e(tmp_nodes[0],tmp_nodes[8]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(21);
                }
            }
            if (tmp_nodes[1]!=0 && tmp_nodes[9]!=0) {
                //search for node 22
                OctreeEdge e(tmp_nodes[1],tmp_nodes[9]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(22);
                }
            }
            if (tmp_nodes[2]!=0 && tmp_nodes[10]!=0) {
                //search for node 23
                OctreeEdge e(tmp_nodes[2],tmp_nodes[10]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(23);
                }
            }
            if (tmp_nodes[3]!=0 && tmp_nodes[11]!=0) {
                //search for node 24
                OctreeEdge e(tmp_nodes[3],tmp_nodes[11]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(24);
                }
            }
            if (tmp_nodes[9]!=0 && tmp_nodes[11]!=0) {
                //search for node 25
                OctreeEdge e(tmp_nodes[9],tmp_nodes[11]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(25);
                }
            }
            //The middle node of the hexahedron can never be inserted
            //otherwise this octant was already removed from the list
            //of elements and replaced with the new 8 octants

            //------------------------------------------------------
            //Finally, apply the transition pattern
            //------------------------------------------------------

            //add the indexes to the vectors
            list<unsigned int>::iterator ui_iter;
            nodes.reserve(l_nodes.size());
            for (ui_iter = l_nodes.begin(); ui_iter!=l_nodes.end(); ui_iter++) {
                nodes.push_back(*ui_iter);
            }
            mid_nodes.reserve(l_mid_nodes.size());
            for (ui_iter=l_mid_nodes.begin(); ui_iter!=l_mid_nodes.end(); ui_iter++) {
                mid_nodes.push_back(*ui_iter);
            }

            //creat the pattern
            patterns::TransitionTemplate tt (nodes,mid_nodes);
            //new points goes to new_pts (if any new node is inserted) and
            //the new elements to new_eles

            //the subelements of this octant will no longer be the points
            //of the original cube. It will now contain mixed-elements.
            vector<vector<unsigned int>> &sub_elements = o->sub_elements;
            sub_elements.clear();

            unsigned int old_size = new_pts->size();

            bool succeed = tt.getNewElements(nodes,mid_nodes,*points,*new_pts,sub_elements);

            if (succeed) {
                pointindex.clear();
                pointindex.reserve(l_nodes.size());
                for (ui_iter = l_nodes.begin(); ui_iter!=l_nodes.end(); ui_iter++) {
                    pointindex.push_back(*ui_iter);
                }

                if (new_pts->size()!=old_size) {
                    pointindex.push_back(points->size() + old_size);
                }
            }

            return succeed;
        }
        //checkTransitionPattern
        else
        {
            if (*max_ref_level==o->ref_level) {
                return true;
            }
            vector<unsigned int> &pointindex = o->pointindex;

            vector<unsigned int> nodes, mid_nodes, tmp_nodes(12,0);
            list<unsigned int> l_nodes, l_mid_nodes;
            //insert the 8 nodes of the cube.
            //cout << "checking element";
            for (unsigned int i=0; i<8; i++) {
                //cout << " " << pointindex[i];
                l_nodes.push_back(pointindex[i]);
            }
            //cout << "\nInserted edges:\n";
            //search for nodes inserted in edges
            for (unsigned int i=0; i<12; i++) {
                OctreeEdge e;
                EdgeVisitor::getEdge(o,i,e);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge==edges->end()) {
                    cout << "  edge " << e << " not found at applyTransitionPattern\n";
                    return false;
                }
                if ((*my_edge)[2]!=0) {
                    //cout << (*my_edge)[2] << "(" << i << ") [";
                    //cout << *my_edge << "]\n";
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(i+8);
                    tmp_nodes[i]=(*my_edge)[2];
                }
            }
            //if this elements do not present nodes inserted in its edges
            //then return true (meaning this case is already considered in
            //the transition patterns) and add this element to the vector
            //of "new elements"
            if (l_mid_nodes.empty()) {
                //cout << "element without mid nodes\n";
                //new_eles.push_back(pointindex);
                return true;
            }

            //at this point the element has at least one node inserted in
            //an edge and we need to search for nodes inserted as middle
            //nodes of their faces

            //cout << "looking for nodes inserted in faces:\n";
            if (tmp_nodes[0]!=0 && tmp_nodes[2]!=0) {
                //search for node 20
                OctreeEdge e(tmp_nodes[0],tmp_nodes[2]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(20);
                    //cout << *my_edge << "\n";
                }
            }
            if (tmp_nodes[0]!=0 && tmp_nodes[8]!=0) {
                //search for node 21
                OctreeEdge e(tmp_nodes[0],tmp_nodes[8]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(21);
                    //cout << *my_edge << "\n";
                }
            }
            if (tmp_nodes[1]!=0 && tmp_nodes[9]!=0) {
                //search for node 22
                OctreeEdge e(tmp_nodes[1],tmp_nodes[9]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(22);
                    //cout << *my_edge << "\n";
                }
            }
            if (tmp_nodes[2]!=0 && tmp_nodes[10]!=0) {
                //search for node 23
                OctreeEdge e(tmp_nodes[2],tmp_nodes[10]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(23);
                    //cout << *my_edge << "\n";
                }
            }
            if (tmp_nodes[3]!=0 && tmp_nodes[11]!=0) {
                //search for node 24
                OctreeEdge e(tmp_nodes[3],tmp_nodes[11]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(24);
                    //cout << *my_edge << "\n";
                }
            }
            if (tmp_nodes[9]!=0 && tmp_nodes[11]!=0) {
                //search for node 25
                OctreeEdge e(tmp_nodes[9],tmp_nodes[11]);
                set<OctreeEdge>::iterator my_edge = edges->find(e);
                if (my_edge != edges->end() && (*my_edge)[2]!=0) {
                    l_nodes.push_back((*my_edge)[2]);
                    l_mid_nodes.push_back(25);
                    //cout << *my_edge << "\n";
                }
            }
            //The middle node of the hexahedron can never be inserted
            //otherwise this octant was already removed from the list
            //of elements and replaced with the new 8 octants

            //------------------------------------------------------
            //Finally, apply the transition pattern
            //------------------------------------------------------

            //add the indexes to the vectors
            list<unsigned int>::iterator ui_iter;
            nodes.reserve(l_nodes.size());
            for (ui_iter = l_nodes.begin(); ui_iter!=l_nodes.end(); ui_iter++) {
                nodes.push_back(*ui_iter);
            }
            mid_nodes.reserve(l_mid_nodes.size());
            for (ui_iter=l_mid_nodes.begin(); ui_iter!=l_mid_nodes.end(); ui_iter++) {
                mid_nodes.push_back(*ui_iter);
            }
            //temporal new points, but they won't be inserted. It's just
            //to see if the pattern for this configuration is already implemented
            list<Point3D> local_new_pts;

            vector<vector<unsigned int> > new_eles;
            list<MeshPoint> new_pts;

            patterns::TransitionTemplate tt (nodes,mid_nodes);
            //find out if the pattern is implemented
            return tt.getNewElements(nodes,mid_nodes,*points,local_new_pts,new_eles);
        }
    }
}