/*
 * Copyright (c) 2013 www.360buy.com
 * All rights reserved.
 *
 * @FILE: 		string_algo.h
 * @AUTHOR: 	liyongqiang (yfliyongqiang@360buy.com)
 * @DATE: 		2013-01-24
 * @VERSION: 	1.0
 *
 * @DESCRIPTION:Header file of class Stringalgo.
 *				Defines some util method for string operation.
 */
#ifndef JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_STRING_ALGO_H_
#define JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_STRING_ALGO_H_

#include <string>
#include <vector>
using std::string;

class StringAlgo {
 public:
    StringAlgo();
    ~StringAlgo();
    static bool start_with(const string &str, const string &start);
    static bool end_with(const string &str, const string &end);
    static void to_lower(string &str, int len);                                                // NOLINT
    static void tokenize(const string &s, const string &split, std::vector<string> &res_vec);  // NOLINT
    template <typename T>
    static bool string_to(const string &s, T &v);  // NOLINT
    template <typename T>
    static bool string_of(const T &v, string &s);  // NOLINT
};                                                 // end class StringAlgo

#include "boost/lexical_cast.hpp"
using boost::lexical_cast;
using boost::bad_lexical_cast;
template<class T>
bool StringAlgo::string_to(const string &s,T &v){
	try{
		v=lexical_cast<T>(s);
		return true;
	}catch(bad_lexical_cast &){
		return false;
	}
}

template<class T>
bool StringAlgo::string_of(const T &v,string &s){
	try{
		s=lexical_cast<string>(v);
		return true;
	}catch(bad_lexical_cast &){
		return false;
	}
}
#endif  // JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_STRING_ALGO_H_
