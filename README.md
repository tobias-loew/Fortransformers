# Fortransformers
Fortran90 to C++

This is a Fortran to C++ translation-tool.

This project uses modified versions of f2c (http://www.netlib.org/f2c/) and Blitz++ (https://github.com/blitzpp/blitz).
When used, licenses from those projects also have to be obeyed.

Usage is granted as specified in the following "Boost Software License - Version 1.0", but usage and/or distribution and/or selling as a or as part of a commercial Fortran to C/C++ translation-tool and/or translation-service is prohibited.


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.




For compiling/running this project Visual Studio 2015 or later is required.
Additionally needed:
  - a "Boost" installation (www.boost.org, version 1.67 works)
  - "Bison for Windows" (http://gnuwin32.sourceforge.net/packages/bison.htm, version 2.4.1 works); the executable is expected to be installed in C:\GnuWin32\bin\ (otherwise adjust path in the "Custom Bould Tool" for F2C->grammar->gram_new_make)
  
Usage: 
  - open VS solution "f2cpp.sln"
  - adjust boost include directory in the "test" project
  - adjust path for bison.exe in "Custom Bould Tool" for F2C->grammar->gram_new_make (see above)
  - select "test" project as startup-project
  - build solution
  - run "test" project

watch the talk on C++Now 2018: https://www.youtube.com/watch?v=9jGPe1S17R0

If you or your company neess professional support transforming Fortran code to C++ you can contact me:
tobi < at > die-loews < dot > de
