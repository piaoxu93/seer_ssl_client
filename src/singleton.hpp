#ifndef _SINGLETON_H_
#define _SINGLETON_H_
template <class SingletonClass >
class Singleton{
public:
    static SingletonClass * Instance(){
        static SingletonClass instance;
        return &instance;
    }
    SingletonClass* operator ->() { return Instance(); }
    const SingletonClass* operator ->() const { return Instance(); }
private:
    Singleton(){}
    ~Singleton(){}
};
#endif // _SINGLETON_H_
