# Comment compiler le projet

## Liste des commandes :

git clone https://github.com/sythm7/gl-learn-render-project.git <br>
cd gl-learn-render-project <br>
mkdir build <br>
cd build <br>
cmake -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="make" .. <br>
make

## Liste des exécutables :

Dans le dossier src/, les fichiers "BezierUniformParametric.cpp", "Polygon3D.cpp" et "Triangulation2D.cpp" sont les trois programmes principaux de ce projet avec chacun leur fonction main().

Lors de la compilation, leurs exécutables correspondants sont produits dans le dossier build/.