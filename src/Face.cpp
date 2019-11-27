#include "Face.h"

Face::Face(vector<unsigned int> &apoints){
    unsigned int pn = apoints.size();
    color = " 1 1 1 ";
    minindex = apoints[0];
    points.reserve(pn);
    for (unsigned int i=0; i<pn; i++) {
        addPoint(apoints[i]);
    }
}

Face::~Face(){
    
}

void Face::addPoint(unsigned int index){
    if(index<minindex)
        minindex = index;
    points.push_back(index);
}

void Face::printFace(FILE *f){
    for(int i=0;i<points.size();i++){
        fprintf(f,"%u ",points[i]);
    }
}

void Face::replacePoints(vector<unsigned int> &newpts){
    points.clear();
    points.reserve(newpts.size());
    for (unsigned int i=0; i<newpts.size(); i++) {
        addPoint(newpts[i]);
    }
}

list<vector <unsigned int> > Face::getTriangles(){
    list<vector <unsigned int> > triangles;
    for (unsigned int i=2; i<points.size(); i++) {
        vector<unsigned int> t (3,0);
        t[0] = points[0];
        t[1] = points[i-1];
        t[2] = points[i];
        triangles.push_back(t);
    }
    return triangles;
}

void Face::printTriangles(FILE *f){
    for(int i=2;i<points.size();i++){
        fprintf(f,"%u ",points[0]);
        fprintf(f,"%u ",points[i-1]);
        fprintf(f,"%u ",points[i]);
    }
}

bool Face::hasPoint(unsigned int index){
    for (unsigned int i=0; i<points.size(); i++) {
        if (points[i] == index) {
            return true;
        }
    }
    return false;
}

void Face::replacePoint(unsigned int oldp, unsigned int newp){
    for (unsigned int i=0; i<points.size(); i++){
        if (points[i] == oldp) {
            points[i]=newp;
        }
    }
}

void Face::print(){
    cout << points.size() << " " << elements.size();
}


ostream& operator<<(ostream& o,Face &f){
    vector<unsigned int> p = f.getPoints();
    int n = p.size();
    o << n;
    for (unsigned int i=0;i<n;i++){
        o << " " << p.at(i);
    }
    return o;
}

bool operator==(Face &f1, Face &f2){
    vector<unsigned int> p = f2.getPoints();
    if(f1.numberOfPoints()!=f2.numberOfPoints())
        return false;
    for(unsigned int i=0;  i<p.size(); i++) {
        if (!f1.hasPoint(p.at(i))) {
            return false;
        }
    }
    return true;
}

