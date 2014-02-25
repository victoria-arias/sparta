/* ----------------------------------------------------------------------
   SPARTA - Stochastic PArallel Rarefied-gas Time-accurate Analyzer
   http://sparta.sandia.gov
   Steve Plimpton, sjplimp@sandia.gov, Michael Gallis, magalli@sandia.gov
   Sandia National Laboratories

   Copyright (2014) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.

   See the README file in the top-level SPARTA directory.
------------------------------------------------------------------------- */

#ifndef SPARTA_CUT3D_H
#define SPARTA_CUT3D_H

#include "pointers.h"
#include "cut2d.h"
#include "my_vec.h"

namespace SPARTA_NS {

class Cut3d : protected Pointers {
 public:
  Cut3d(class SPARTA *);
  ~Cut3d();
  int surf2grid(cellint, double *, double *, int *, int);
  int split(cellint, double *, double *, int, int *,
            double *&, int *, int *, int &, double *);

 private:
  cellint id;            // ID of cell being worked on
  double *lo,*hi;        // opposite corner pts of cell
  int nsurf;             // # of surf elements in cell
  int *surfs;            // indices of surf elements in cell

  double **path1,**path2;

  // DEBUG
  //int totcell,totsurf,totvert,totedge;

  MyVec<double> vols;    // vols of each flow polyhedra found

  int empty;

  struct Vertex {
    int active;      // 1/0 if active or not
    int style;       // CTRI or CTRIFACE or FACEPGON or FACE
    int label;       // index in list of tris that intersect this cell
                     //   for CTRI or CTRIFACE
                     // face index (0-5) for FACEPGON or FACE
    int next;        // index of next vertex when walking a loop
    int nedge;       // # of edges in this vertex
    int first;       // first edge in vertex
    int dirfirst;    // dir of first edge in vertex
    int last;        // last edge in vertex
    int dirlast;     // dir of last edge in vertex
    double volume;   // volume of vertex projected against lower z face of cell
    double *norm;    // ptr to norm of tri, NULL for other styles
  };

  struct Edge {
    double p1[3],p2[3];  // 2 points in edge
    int active;          // 1/0 if active or not
    int style;           // CTRI or CTRIFACE or FACEPGON or FACE
    int clipped;         // 1/0 if already clipped during face iteration
    int nvert;           // flag for verts containing this edge
                         // 0 = no verts
                         // 1 = just 1 vert in forward dir
                         // 2 = just 1 vert in reverse dir
                         // 3 = 2 verts in both dirs
                         // all vecs are [0] in forward dir, [1] in reverse dir
    int verts[2];        // index of vertices containing this edge, -1 if not
    int next[2];         // index of next edge for each vertex, -1 for end
    int dirnext[2];      // next edge for each vertex is forward/reverse (0,1)
    int prev[2];         // index of prev edge for each vertex, -1 for start
    int dirprev[2];      // prev edge for each vertex is forward/reverse (0,1)
  };

  struct Loop {
    double volume;        // volume of loop
    int flag;             // INTERIOR (if all CTRI vertices) or BORDER
    int n;                // # of vertices in loop
    int first;            // index of first vertex in loop
    int next;             // index of next loop in same PH, -1 if last loop
  };

  struct PH {
    double volume;
    int n;
    int first;
  };

  MyVec<Vertex> verts;       // list of vertices in BPG
  MyVec<Edge> edges;         // list of edges in BPG
  MyVec<Loop> loops;         // list of loops of vertices = polyhedra
  MyVec<PH> phs;             // list of polyhedrons = one or more loops

  MyVec<int> facelist[6];    // list of edges on each cell face
  MyVec<int> used;           // 0/1 flag for each vertex when walking loops
  MyVec<int> stack;          // list of vertices to check when walking loops

  class Cut2d *cut2d;

  int clip(double *, double *, double *);
  void add_tris();
  int clip_tris();
  void ctri_volume();
  void edge2face();
  void edge2clines(int);
  void add_face_pgons(int);
  void add_face(int, double *, double *);
  void remove_faces();
  void check();
  void walk();
  void loop2ph();
  void create_surfmap(int *);
  int split_point(int *, double *);
  
  void edge_insert(int, int, int, int, int, int, int);
  void edge_remove(Edge *);
  void edge_remove(Edge *, int);
  void vertex_remove(Vertex *);
  int grazing(Vertex *);
  int which_faces(double *, double *, int *);
  void face_from_cell(int, double *, double *);
  void compress2d(int, double *, double *);
  void expand2d(int, double, double *, double *);

  int findedge(double *, double *, int, int &);
  void between(double *, double *, int, double, double *);
  int samepoint(double *, double *);
  int corner(double *);
  int ptflag(double *);

  void print_bpg(const char *);
  void print_loops();
};

}

#endif

/* ERROR/WARNING messages:

*/