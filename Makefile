CXXFLAGS=-std=c++11 -O3

all: xml-plaintext xml-tags

xml-plaintext: xml-plaintext.cc pugixml/src/pugixml.cpp

xml-tags: xml-tags.cc pugixml/src/pugixml.cpp

clean:
	rm -f xml-plaintext xml-tags
