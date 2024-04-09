# Comment compiler et exécuter le projet

## Librairies à installer :

### Distributions Linux basées sur debian
sudo apt install cmake make g++ libglfw3-dev libglm-dev

### Distributions basées sur Arch Linux
sudo pacman -S make cmake gcc glfw glm

### Windows
Installer CMake pour Windows <br>
Installer g++ et make pour Windows puis les ajouter dans le path

## Liste des commandes :
git clone https://github.com/sythm7/gl-learn-render-project.git <br>
cd gl-learn-render-project <br>
mkdir build <br>
cd build <br>

### Sur Linux 
cmake .. <br>
make

### Sur Windows
cmake -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="make" .. <br>
make

## Liste des exécutables :

Dans le dossier src/, les fichiers "BezierUniformParametric.cpp", "Polygon3D.cpp" et "Triangulation2D.cpp" sont les trois programmes principaux de ce projet avec chacun leur fonction main().

Lors de la compilation, leurs exécutables correspondants sont produits dans le dossier build/.