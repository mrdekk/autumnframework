
#ifndef AUTUMN_MYCOMBINEDTYPEMAP_H
#define AUTUMN_MYCOMBINEDTYPEMAP_H

#include <cstdlib>
#include <map>
#include "ICombinedType.h"

using namespace std;

class MyCombinedTypeMap: public ICombinedType{
public:
	void* createValue(const vector<string>& vl, string basicType, vector<string>::iterator& it){
		//�������������������������ͣ����ø���ķ���ȥ����������ͣ����ǣ�û������
		//���map����˹��죬����һ���������͡�
		map<string, int>* p = new map<string, int>;
		for( ; it!=vl.end(); it++){
			string s = *it++;
			int i;
			if( it != vl.end() ) i = atoi(*it++);
			p->insert(make_pair(name, i));
		}
		return (void*)p;
	}

	void free
};

#endif
