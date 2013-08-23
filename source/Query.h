//parsed query
#include <vector>
#include <string>
using namespace std;
class Query
{
public:
	Query(void);

	virtual ~Query(void);

private:
	vector<string> result_synonym;
	vector<string> with;
	vector<string> and;
};

