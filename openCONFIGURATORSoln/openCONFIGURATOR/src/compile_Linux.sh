rm ./openCONFIGURATOR.so
#g++ -shared -o ../SharedLibrary/openCONFIGURATOR.so `xml2-config --cflags` *.cpp `xml2-config --libs`
g++ -shared -o ./openCONFIGURATOR.so `xml2-config --cflags` *.cpp `xml2-config --libs`
