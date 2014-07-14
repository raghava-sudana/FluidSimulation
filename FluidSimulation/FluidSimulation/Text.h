#include <fstream>

using std::ifstream;
using std::ofstream;
using std::ios;

class Text {
private:
	int nLength;
	char *pContent;
public:
	Text() {nLength = 0; pContent = NULL;}
	~Text() {delete [] pContent;}

	const char* getContent() const {return pContent;}

	bool readTexFile(const char* name) {
		ifstream file(name, ios::in|ios::binary);

		file.seekg(0, ios::end);
		nLength = (int)file.tellg();
		if ( file.bad() )
			return false;

		file.seekg(0, ios::beg);
		delete [] pContent;
		pContent = new char [nLength+1];
		file.read(pContent, nLength);
		pContent[nLength] = 0;

		return ( ! file.bad() );
	}

	bool writeTexFile(const char* name) {
		ofstream file(name, ios::out|ios::binary);

		file << pContent;

		return ( ! file.bad() );
	}
};
