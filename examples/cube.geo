// Gmsh project created on Fri Jun 11 16:16:46 2021
SetFactory("OpenCASCADE");
//+
Box(1) = {-1, -1, -1, 2, 2, 2};
//+
Transfinite Curve {9, 11, 10, 12} = 3 Using Progression 1;
//+
Transfinite Curve {1, 5, 7, 3} = 3 Using Progression 1;
//+
Transfinite Curve {2, 4, 8, 6} = 3 Using Progression 1;
//+
Transfinite Surface {3} = {2, 6, 5, 1};
//+
Transfinite Surface {4} = {4, 8, 7, 3};
//+
Transfinite Surface {6} = {1, 5, 7, 3};
//+
Transfinite Surface {5} = {2, 6, 8, 4};
//+
Transfinite Surface {2} = {6, 8, 7, 5};
//+
Transfinite Surface {1} = {2, 4, 3, 1};
//+
Transfinite Volume{1} = {2, 6, 8, 4, 1, 5, 7, 3};
//+
Recombine Surface {5, 4, 3, 6, 2, 1};//+
Physical Surface("dirichlet", 13) = {6};
