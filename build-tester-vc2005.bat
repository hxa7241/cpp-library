@echo off


rem --- using: MS VC++ 2005 ---


set COMPILER=cl
set COMPILE_OPTIONS=/c /O2 /arch:SSE /fp:fast /EHsc /GR- /GS- /MT /W4 /WL /nologo /DTESTING /Igeneral /Igraphics /Iimage



mkdir obj
del /Q obj\*



@echo.
@echo --- compile ---

%COMPILER% %COMPILE_OPTIONS% general/Array.cpp /Foobj/Array.obj
%COMPILER% %COMPILE_OPTIONS% general/Bounded.cpp /Foobj/Bounded.obj
%COMPILER% %COMPILE_OPTIONS% general/Clamps.cpp /Foobj/Clamps.obj
%COMPILER% %COMPILE_OPTIONS% general/Float01.cpp /Foobj/Float01.obj
%COMPILER% %COMPILE_OPTIONS% general/FpToInt.cpp /Foobj/FpToInt.obj
%COMPILER% %COMPILE_OPTIONS% general/LogFast.cpp /Foobj/LogFast.obj
%COMPILER% %COMPILE_OPTIONS% general/PowFast.cpp /Foobj/PowFast.obj
%COMPILER% %COMPILE_OPTIONS% general/RandomMwc1.cpp /Foobj/RandomMwc1.obj
%COMPILER% %COMPILE_OPTIONS% general/RandomMwc2.cpp /Foobj/RandomMwc2.obj
%COMPILER% %COMPILE_OPTIONS% general/Sheet.cpp /Foobj/Sheet.obj

%COMPILER% %COMPILE_OPTIONS% graphics/ColorConstants.cpp /Foobj/ColorConstants.obj
%COMPILER% %COMPILE_OPTIONS% graphics/ColorSpace.cpp /Foobj/ColorSpace.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Coord2.cpp /Foobj/Coord2.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Filters.cpp /Foobj/Filters.obj
%COMPILER% %COMPILE_OPTIONS% graphics/HaltonSequence.cpp /Foobj/HaltonSequence.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Matrix3f.cpp /Foobj/Matrix3f.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Matrix4f.cpp /Foobj/Matrix4f.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Polar.cpp /Foobj/Polar.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Quaternion.cpp /Foobj/Quaternion.obj
%COMPILER% %COMPILE_OPTIONS% graphics/SobolSequence.cpp /Foobj/SobolSequence.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Spherahedron.cpp /Foobj/Spherahedron.obj
%COMPILER% %COMPILE_OPTIONS% graphics/UnitVector3f.cpp /Foobj/UnitVector3f.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Vector3f.cpp /Foobj/Vector3f.obj
%COMPILER% %COMPILE_OPTIONS% graphics/Vector4f.cpp /Foobj/Vector4f.obj
%COMPILER% %COMPILE_OPTIONS% graphics/XyzE.cpp /Foobj/XyzE.obj

%COMPILER% %COMPILE_OPTIONS% image/ImageQuantizing.cpp /Foobj/ImageQuantizing.obj
%COMPILER% %COMPILE_OPTIONS% image/PixelsPtr.cpp /Foobj/PixelsPtr.obj
%COMPILER% %COMPILE_OPTIONS% image/ppm.cpp /Foobj/ppm.obj
%COMPILER% %COMPILE_OPTIONS% image/rgbe.cpp /Foobj/rgbe.obj
%COMPILER% %COMPILE_OPTIONS% image/StreamExceptionSet.cpp /Foobj/StreamExceptionSet.obj

%COMPILER% %COMPILE_OPTIONS% hxa7241_cpp_library_tester.cpp /Foobj/hxa7241_cpp_library_tester.obj



@echo.
@echo --- link ---

link /OPT:REF /OPT:NOWIN98 /NOLOGO /OUT:hxa7241_cpp_library_tester.exe kernel32.lib obj/*.obj



del /Q obj\*
