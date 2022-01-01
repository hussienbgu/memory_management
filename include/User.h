#ifndef USER_H_
#define USER_H_
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;
class User{
public:
    User(const std::string& name);
    User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;                          //done
    std::vector<Watchable*> get_history() const;
    void pushToHistory(Watchable* W);
    virtual ~User();//destructor
    virtual bool nonwatched(Watchable* w)=0;
    User(const User& other);///copy constructor
    User& operator=(const User& other);  //copy assignment operator
    virtual User* makeNew()=0;
protected:
    std::vector<Watchable*> history;                      //done
private:
    std::string name;

};
class LengthRecommenderUser : public User {
public:
    User* makeNew();
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ~LengthRecommenderUser();
    bool nonwatched(Watchable* W);
private:
};
class RerunRecommenderUser : public User {
public:
    User* makeNew();
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ~RerunRecommenderUser();
    bool nonwatched(Watchable* W);

private:
    int counter;
};
class GenreRecommenderUser : public User {
public:
    User* makeNew();
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ~GenreRecommenderUser();
    bool nonwatched(Watchable* W);
};
#endif
