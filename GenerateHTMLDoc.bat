rmdir %%f _doc/html /s/q
cd lib-SLProject
doxygen.exe
start ../_doc/html/index.html
exit