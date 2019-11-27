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

//
// Created by nanairo on 08-03-16.
//

#include "IntersectionsVisitor.h"

#include <stdexcept>
#include "../Octant.h"

namespace Clobscode
{
    IntersectionsVisitor::IntersectionsVisitor() {
        mesh = NULL;
        points = NULL;
        faces = NULL;
        coords = NULL;
        select_faces = false;
    }


    IntersectionsVisitor::IntersectionsVisitor(bool select_faces) {
        mesh = NULL;
        points = NULL;
        faces = NULL;
        coords = NULL;
        this->select_faces = select_faces;
    }

    void IntersectionsVisitor::setTriMesh(TriMesh &mesh) {
        this->mesh = &mesh;
    }
    void IntersectionsVisitor::setPoints(vector<MeshPoint> &points){
        this->points = &points;
    }
    void IntersectionsVisitor::setFaces(list<unsigned int> &faces){
        this->faces = &faces;
    }
    void IntersectionsVisitor::setCoords(vector<Point3D> &coords){
        this->coords = &coords;
    }


    bool IntersectionsVisitor::visit(Octant *o) {
        //cout << "IntersectionsVisitor" << endl;
        //check intersections with selected input faces
        if (select_faces) //checkIntersections(TriMesh &mesh,list<unsigned int> &faces,vector<Point3D> &coords);
        {
            if (faces == NULL || coords == NULL)
                throw std::runtime_error(std::string("Calling wrong IntersectionsVisitor!"));
            list<unsigned int> &intersected_faces = o->intersected_faces;

            list<unsigned int>::iterator f_iter;
            vector<Point3D> input_pts = mesh->getPoints();

            for (f_iter = faces->begin(); f_iter!=faces->end(); f_iter++) {
                SurfTriangle face = mesh->getFaces()[*f_iter];
                if (intersectsTriangle(face,input_pts,coords->at(0),coords->at(1))) {
                    intersected_faces.push_back(*f_iter);
                }
            }

            return !intersected_faces.empty();
        }
        //check intersections with all input faces
        else //checkIntersections(TriMesh &mesh, vector<MeshPoint> &pts)
        {
            if (points == NULL)
                throw std::runtime_error(std::string("Calling wrong IntersectionsVisitor!"));
            vector<unsigned int> &pointindex = o->pointindex;
            list<unsigned int> &intersected_faces = o->intersected_faces;

            vector<SurfTriangle> faces = mesh->getFaces();
            vector<Point3D> input_pts = mesh->getPoints();

            for (unsigned int j=0; j<faces.size(); j++) {
                if (intersectsTriangle(faces[j],input_pts,
                                       points->at(pointindex[0]).getPoint(),
                                       points->at(pointindex[6]).getPoint())) {

                    intersected_faces.push_back(j);

                }
            }
            return !intersected_faces.empty();
        }

    }

    //auxiliary functions
    bool IntersectionsVisitor::intersectsTriangle(SurfTriangle &st, vector<Point3D> &input_pts, const Point3D &pmin,
                                             const Point3D &pmax) {

        //pmin and pmax describe the bounding box of the
        //octant. They are used to detect intersections
        //between input faces and the octant by a clipping
        //method.

        //This code is based on the Cohen-Sutherland algorithm
        //for image clipping over a given window. In this case
        //the "window" is the octant (cube), and the "lines"
        //to be clipped are the triangles.

        vector<unsigned int> face = st.getPoints();

        unsigned int n_pts = face.size();

        vector<unsigned int> sides(n_pts,0);

        for (unsigned int i=0; i<n_pts; i++) {
            sides[i] = computePosition(input_pts[face[i]],
                                       pmin,pmax);
            if (sides[i] == 0) {
                return true;
            }
        }

        //Compute logic and between bits associated to each
        //triangle node. If all of them are at same side (e.g.
        //all at left), the result will be different from cero,
        //meaning that there is no intersection.
        unsigned int all_and = sides[0];
        for (unsigned int i=1; i<n_pts; i++) {
            all_and = all_and & sides[i];
        }
        //If all are at the same side, return false.
        if (all_and != 0) {
            return false;
        }

        //Patch: all other cases, we cannot be sure.
        //return true;

        //TEST EDGES//

        //Find out if they cross the hex. Two cases: both are
        //"centered", but one is on the left and the other on
        //the right (same for Top/bottom and Front/Back).
        //Tested with logic or. The other case is that their
        //are in exact opposite "3D corners". Tested with XoR.
        for (unsigned int i=0; i<n_pts; i++) {
            unsigned int next = (i+1)%n_pts;
            unsigned int result_or = sides[i]|sides[next];
            unsigned int result_xor = sides[i]^sides[next];
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
            /*if (result_xor == 63) {
                //63 <> 11 11 11
                return true;
            }*/
        }

        //General case of clipping a triangle's edge against
        //the octant.
        for (unsigned int i=0; i<n_pts; i++) {
            //cout << pts[face_pts_idx[i]] << " code is ";
            Point3D p1 = input_pts[face[i]];
            Point3D p2 = input_pts[face[(i+1)%n_pts]];

            if (clipGeneralCase(p1,p2,pmin,pmax)) {
                return true;
            }
        }

        //Last "hard" test: Apply Ray Tracing to detect
        //intersection between every edge of the octant
        //(hexahedron) and the triangle. If at least one
        //of them instersects, then this triangle
        //intersects the octant.
        //If the test didn't succeed, the triangle
        //is not intersected by this octant

        return edgeTriangleIntersection(st,input_pts,pmin,pmax);
    }

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------

    bool IntersectionsVisitor::clipGeneralCase(const Point3D &p1, const Point3D &p2, const Point3D &pmin,
                                          const Point3D &pmax) {

        //compute the parametric equations of the given segment
        double dx = p2[0]-p1[0];
        double dy = p2[1]-p1[1];
        double dz = p2[2]-p1[2];
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
        t = (pmin[0]-p1[0])/dx;

        //if (0<t && t<1) {
        if (0<=t && t<=1) {
            //test Y axis for this 't'
            Y = p1[1] + t*dy;
            //if (pmin[1]<Y && Y<pmax[1]) {
            if (pmin[1]<=Y && Y<=pmax[1]) {
                //test Z axis for this 't'
                Z = p1[2] + t*dz;
                //if (pmin[2]<Z && Z<pmax[2]) {
                if (pmin[2]<=Z && Z<=pmax[2]) {
                    return true;
                }
            }
        }

        //test X axis over X_max
        t = (pmax[0]-p1[0])/dx;

        //if (0<t && t<1) {
        if (0<=t && t<=1) {
            //test Y axis for this 't'
            Y = p1[1] + t*dy;
            //if (pmin[1]<Y && Y<pmax[1]) {
            if (pmin[1]<=Y && Y<=pmax[1]) {
                //test Z axis for this 't'
                Z = p1[2] + t*dz;
                //if (pmin[2]<Z && Z<pmax[2]) {
                if (pmin[2]<=Z && Z<=pmax[2]) {
                    return true;
                }
            }
        }


        //test Y axis over Y_min
        t = (pmin[1]-p1[1])/dy;

        //if (0<t && t<1) {
        if (0<=t && t<=1) {
            //test Y axis for this 't'
            X = p1[0] + t*dx;
            //if (pmin[0]<X && X<pmax[0]) {
            if (pmin[0]<=X && X<=pmax[0]) {
                //test Z axis for this 't'
                Z = p1[2] + t*dz;
                //if (pmin[2]<Z && Z<pmax[2]) {
                if (pmin[2]<=Z && Z<=pmax[2]) {
                    return true;
                }
            }
        }

        //test Y axis over Y_max
        t = (pmax[1]-p1[1])/dy;

        //if (0<t && t<1) {
        if (0<=t && t<=1) {
            //test Y axis for this 't'
            X = p1[0] + t*dx;
            //if (pmin[0]<X && X<pmax[0]) {
            if (pmin[0]<=X && X<=pmax[0]) {
                //test Z axis for this 't'
                Z = p1[2] + t*dz;
                //if (pmin[2]<Z && Z<pmax[2]) {
                if (pmin[2]<=Z && Z<=pmax[2]) {
                    return true;
                }
            }
        }

        //test Z axis over Z_min
        t = (pmin[2]-p1[2])/dz;

        //if (0<t && t<1) {
        if (0<=t && t<=1) {
            //test Y axis for this 't'
            X = p1[0] + t*dx;
            //if (pmin[0]<X && X<pmax[0]) {
            if (pmin[0]<=X && X<=pmax[0]) {
                //test Z axis for this 't'
                Y = p1[1] + t*dy;
                //if (pmin[1]<Y && Y<pmax[1]) {
                if (pmin[1]<=Y && Y<=pmax[1]) {
                    return true;
                }
            }
        }

        //test Z axis over Z_max
        t = (pmax[2]-p1[2])/dz;

        //if (0<t && t<1) {
        if (0<=t && t<=1) {
            //test Y axis for this 't'
            X = p1[0] + t*dx;
            //if (pmin[0]<X && X<pmax[0]) {
            if (pmin[0]<=X && X<=pmax[0]) {
                //test Z axis for this 't'
                Y = p1[1] + t*dy;
                //if (pmin[1]<Y && Y<pmax[1]) {
                if (pmin[1]<=Y && Y<=pmax[1]) {
                    return true;
                }
            }
        }

        return false;
    }

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------

    bool IntersectionsVisitor::edgeTriangleIntersection(SurfTriangle &st,
                                                   vector<Point3D> &input_pts,
                                                   const Point3D &pmin,
                                                   const Point3D &pmax) {

        //compute the coords of all octant edges
        vector<vector<Point3D> > oct_edges = getEdges(pmin,pmax);

        //test each edge against the triangle
        for (unsigned int i=0; i<oct_edges.size(); i++) {
            vector<Point3D> oct_ed = oct_edges[i];

            if (st.segmentIntersection(input_pts,oct_ed[0],oct_ed[1])) {
                return true;
            }
        }

        return false;
    }

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------

    vector<vector<Point3D>> IntersectionsVisitor::getEdges(const Point3D &pmin,
                                                      const Point3D &pmax) {
        vector<vector<Point3D> > edges;
        edges.reserve(12);

        //creat the 8 nodes of the octant
        Point3D p0 (pmin[0],pmin[1],pmin[2]);
        Point3D p1 (pmin[0],pmin[1],pmax[2]);
        Point3D p2 (pmax[0],pmin[1],pmax[2]);
        Point3D p3 (pmax[0],pmin[1],pmin[2]);
        Point3D p4 (pmin[0],pmax[1],pmin[2]);
        Point3D p5 (pmin[0],pmax[1],pmax[2]);
        Point3D p6 (pmax[0],pmax[1],pmax[2]);
        Point3D p7 (pmax[0],pmax[1],pmin[2]);

        vector<Point3D> edge(2, Point3D ());

        edge[0] = p0;
        edge[1] = p1;
        edges.push_back(edge);

        edge[0] = p1;
        edge[1] = p2;
        edges.push_back(edge);

        edge[0] = p2;
        edge[1] = p3;
        edges.push_back(edge);

        edge[0] = p3;
        edge[1] = p0;
        edges.push_back(edge);

        edge[0] = p0;
        edge[1] = p4;
        edges.push_back(edge);

        edge[0] = p1;
        edge[1] = p5;
        edges.push_back(edge);

        edge[0] = p2;
        edge[1] = p6;
        edges.push_back(edge);

        edge[0] = p3;
        edge[1] = p7;
        edges.push_back(edge);

        edge[0] = p4;
        edge[1] = p5;
        edges.push_back(edge);

        edge[0] = p5;
        edge[1] = p6;
        edges.push_back(edge);

        edge[0] = p6;
        edge[1] = p7;
        edges.push_back(edge);

        edge[0] = p7;
        edge[1] = p4;
        edges.push_back(edge);

        return edges;
    }

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------

    unsigned int IntersectionsVisitor::computePosition(const Point3D &p, const Point3D &pmin, const Point3D &pmax) {

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


