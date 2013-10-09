Boost.Log implementation
========================

The Windows-Version of openCONFIGURATOR uses Boost.Log 1.54 for logging-output.
Requirements for Boost.Log compilation:

1) Download Boost >= 1.54 from http://www.boost.org and unzip to c:\Programme(Program Files)\boost
2) From the boost root-directory run bootstrap.bat
3) From the boost root-directory run following command to compile Boost.Log and its dependent libraries:

b2 variant=debug,release link=shared threading=multi runtime-link=shared --with-log

4) This will create dll's and their import-libraries in ./stage/lib

5) CMake-Configuration is set up to find Boost in the aforementioned directory and will set the 
include and linker directories accordingly.