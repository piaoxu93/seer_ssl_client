#include "singleparams.h"
#include <fstream>
#include <QtDebug>
#include <QCoreApplication>
#include <String>
#include <QDir>
#include <regex>

//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/path.hpp>
//namespace fs = boost::filesystem;
#include <iostream>
using namespace std;
SingleParams *SingleParams::single = nullptr;

SingleParams::SingleParams(){
//    fs::path full_path( fs::initial_path<fs::path>() );
//    string filename = full_path.string()+string("/params.json");
    string filename = string(QCoreApplication::applicationDirPath().toUtf8().constData())+string("/params.json");
    cout << endl << filename << endl;
    fstream file(filename,ios::in | ios::out);
    qDebug() << "READING :  params.json...";
    if(file.is_open()){
        file >> this->params;
        file.close();
        qDebug() << "......Done!" << endl;
    }else{
        qWarning( "Warning :  Can't open params.json...\n" );
    }
}

SingleParams *SingleParams::instance(){
    if(nullptr == single){
        single = new SingleParams();
    }
    return single;
}

// need to rewrite with 'flex', also json need to be replaced by QJsonObject
json SingleParams::_(const string& X){
    regex XCheck("\\w+((\\.\\w+)|\\[[0-9]+\\])*");
    json res = this->params;
    string key;
    if(!regex_match(X,XCheck)){
        return json();
    }
    auto n = string::size_type(-1);
    auto l = n+1;
    for(unsigned i=0;i<=X.size();i++){
        if(X[i]=='[' || X[i]=='.' || X[i]==']' || i==X.size()){
            n = i;
            key = X.substr(l,n-l);
            l = n+1;
            if(key == ""){
                continue;
            }else if(X[i]==']'){
                res = res[stoi(key)];
            }else{
                res = res[key];
            }
        }
    }
    return res;
}
