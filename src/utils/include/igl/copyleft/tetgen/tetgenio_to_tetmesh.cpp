// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2013 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "tetgenio_to_tetmesh.h"

// IGL includes
#include "../../list_to_matrix.h"

// STL includes
#include <iostream>

template <
  typename DerivedV, 
  typename DerivedT,
  typename DerivedF,
  typename DerivedTM,
  typename DerivedR,
  typename DerivedN,
  typename DerivedPT,
  typename DerivedFT>
IGL_INLINE bool igl::copyleft::tetgen::tetgenio_to_tetmesh(
  const tetgenio & out,
  Eigen::PlainObjectBase<DerivedV>& V,
  Eigen::PlainObjectBase<DerivedT>& T,
  Eigen::PlainObjectBase<DerivedF>& F,
  Eigen::PlainObjectBase<DerivedTM>& TM,
  Eigen::PlainObjectBase<DerivedR>& R,
  Eigen::PlainObjectBase<DerivedN>& N,
  Eigen::PlainObjectBase<DerivedPT>& PT,
  Eigen::PlainObjectBase<DerivedFT>& FT,
  int & num_regions)
{
  // process points
  if(out.pointlist == NULL)
  {
    std::cerr<<"^tetgenio_to_tetmesh Error: point list is NULL\n"<<std::endl;
    return false;
  }
  V.resize(out.numberofpoints,3);
  // loop over points
  for(int i = 0;i < out.numberofpoints; i++)
  {
    V(i,0) = out.pointlist[i*3+0];
    V(i,1) = out.pointlist[i*3+1];
    V(i,2) = out.pointlist[i*3+2];
  }

  // process tets
  if(out.tetrahedronlist == NULL)
  {
    std::cerr<<"^tetgenio_to_tetmesh Error: tet list is NULL\n"<<std::endl;
    return false;
  }

  // When would this not be 4?
  assert(out.numberofcorners == 4);
  T.resize(out.numberoftetrahedra,out.numberofcorners);
  // loop over tetrahedra
  for(int i = 0; i < out.numberoftetrahedra; i++)
  {
    for(int j = 0; j<out.numberofcorners; j++)
    {
      T(i,j) = out.tetrahedronlist[i * out.numberofcorners + j];
    }
  }

  assert(T.maxCoeff() >= 0);
  assert(T.minCoeff() >= 0);
  assert(T.maxCoeff() < V.rows());

  F.resize(out.numberoftrifaces,3);
  // loop over tetrahedra
  for(int i = 0; i < out.numberoftrifaces; i++)
  {
    F(i,0) = out.trifacelist[i * 3 + 0];
    F(i,1) = out.trifacelist[i * 3 + 1];
    F(i,2) = out.trifacelist[i * 3 + 2];
  }

  if(out.pointmarkerlist)
  {
    TM.resize(out.numberofpoints);
    for (int i = 0; i < out.numberofpoints; ++i) 
    {
    	TM(i) = out.pointmarkerlist[i];
    }
  }

  if(out.tetrahedronattributelist)
  {
    R.resize(out.numberoftetrahedra);
    std::unordered_map<REAL, REAL> hashUniqueRegions;
    for(int i = 0; i < out.numberoftetrahedra; i++)
    {
         R(i) = out.tetrahedronattributelist[i];
         hashUniqueRegions[R(i)] = i;
    }
    // extract region marks
    num_regions = hashUniqueRegions.size();
  }else
  {
    num_regions = 0;
  }

  // extract neighbor list
  if(out.neighborlist)
  {
    N.resize(out.numberoftetrahedra, 4);
    for (int i = 0; i < out.numberoftetrahedra; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        N(i,j) = out.neighborlist[i * 4 + j];
      }
    }
  }

  // extract point 2 tetrahedron list
  if(out.point2tetlist)
  {
    PT.resize(out.numberofpoints);
    for (int i = 0; i < out.numberofpoints; i++)
    {
      PT(i) = out.point2tetlist[i];
    }
  }

  //extract face to tetrahedron list
  if(out.face2tetlist)
  {
    FT.resize(out.numberoftrifaces,2);
    int triface;
    for (int i = 0; i < out.numberoftrifaces; i++)
    {
      for (int j = 0; j < 2; j++)
      {
        FT(i,j) = out.face2tetlist[i * 2 + j];
      }
    }
  }

  return true;
}


#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
// generated by autoexplicit.sh
template bool igl::copyleft::tetgen::tetgenio_to_tetmesh<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>>(tetgenio const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1>>&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1>>&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1>>&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1>>&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1>>&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1>>&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1>>&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1>>&, int&);
#endif