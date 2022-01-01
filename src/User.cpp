#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include<map>
#include <algorithm>

//USER
User::User():history(),name() {}
User::User(const std::string &name):history(),name(name){}
std::string User::getName() const {
    return name;
}
std::vector<Watchable*> User::get_history() const{
    return history;
}
void User::pushToHistory(Watchable *W) {
    history.push_back(W->makeNew());
}
User::~User() {
    for (unsigned int i = 0; i < history.size(); ++i) {
        delete history.at(i);
        history.at(i)= nullptr;
    }
}//des
User::User(const User &other):history(),name(other.getName()){
    for (unsigned int i = 0; i <other.history.size() ; ++i) {
        history.push_back(other.history.at(i)->makeNew());
    }

}//copy cons
User& User::operator=(const User &other) {
    if(this!=&other) {
        for (auto &watched:history)
            delete watched;
        history.clear();
        for (auto &i : other.history) {
            if (auto ch = dynamic_cast<Movie *>(i)) {
                this->history.push_back(ch->makeNew());
            }
            if (auto ch = dynamic_cast<Episode *>(i)) {
                this->history.push_back(ch->makeNew());

            }
        }
    }
    return (*this);
}// copy assignment operator

// LengthRecommenderUser class
LengthRecommenderUser::~LengthRecommenderUser() {}
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name){}
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    int length=0;
    for(auto x : history){
        length=length + x->getLength();
    }
    length= static_cast<int>(length / history.size());
    if(s.getContent().empty())
        return nullptr;
    Watchable* rec= nullptr;
    int hfresh=length;
    for(auto &x:s.getContent()){
        if(nonwatched(x)){
            if((abs ((x -> getLength())-length)  < hfresh)) {
                hfresh = abs((x->getLength()) - length);
                rec = x;
            }
        }
    }
    return rec;
    }
bool LengthRecommenderUser::nonwatched(Watchable *W) {
        for(auto &i:this->history){
            if (i->getId() == W->getId()){
                return false;
            }
        }
        return true;
}
User* LengthRecommenderUser::makeNew() {
    LengthRecommenderUser* lu=new LengthRecommenderUser(this->getName());
    for(unsigned int i=0;i<history.size();i++){
        lu->history.push_back(history.at(i)->makeNew());
    }
    return lu;
}

//RerunRecommender
RerunRecommenderUser::~RerunRecommenderUser() {}
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name),counter(0){}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    if(this->get_history().empty())
        return nullptr;
    Watchable* rec = this->history.at(counter % this->get_history().size());
    counter++;
    return rec;



}
User* RerunRecommenderUser::makeNew() {
    RerunRecommenderUser* ru=new RerunRecommenderUser(this->getName());
    for(unsigned int i=0;i<history.size();i++){
        ru->history.push_back(history.at(i)->makeNew());
    }
    return ru;
}
bool RerunRecommenderUser::nonwatched(Watchable *W) {
    for(auto &i:this->history){
        if (i->getId() == W->getId()){
            return false;
        }
    }
    return true;
}
//GenreReccomender
GenreRecommenderUser::~GenreRecommenderUser() {}
bool GenreRecommenderUser::nonwatched(Watchable *W) {
    for(auto &i:this->history){
        if (i->getId() == W->getId()){
            return false;
        }
    }
    return true;
}
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name){}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    //build a map with key tag and value counter;
    std::map<std::string, int> tagcounter;
    std::map<std::string, int>::iterator it;
    for (auto &k:this->history) {// k is a vector of Tags
        for (auto &t:k->getTags()) {// t is a Tag (string);
            bool found = false;
                if (tagcounter.count(t)!=0) {
                    tagcounter.find(t).operator*().second++;
                    found = true;
                }
            if (!found)
                tagcounter.insert({t,1});// if the tag does not exist in the tag counter ,adding with counter 1.
        }
    }
    //sorting the tagcounter map using vector
    std::vector <std::pair<std::string,int >> vecTagCountersorted;
    std::copy(tagcounter.begin(),
              tagcounter.end(),
              std::back_inserter<std::vector<std::pair<std::string,int>>>(vecTagCountersorted));

    std::sort(vecTagCountersorted.begin(),vecTagCountersorted.end(),[](const std::pair<std::string,int>& l,const std::pair<std::string,int>& r){
        if (l.second != r.second)
            return l.second < r.second;
        return l.first < r.first;
    });
    for (unsigned int i = vecTagCountersorted.size()-1; i >=0 ; --i) {
        unsigned int x=i;
        while( x>0 && vecTagCountersorted[x].second==vecTagCountersorted[x-1].second )
            x--;
        for (unsigned int j = 0; j < s.getContent().size(); ++j) {
            Watchable* X=s.getContent().at(j);
            if(this->nonwatched(X)) {
                for (unsigned int k = 0; k < X->getTags().size() ; ++k) {
                    if (vecTagCountersorted.at(x).first==X->getTags().at(k))
                        return X;
                }
            }
        }
    }
    return nullptr;
}
User* GenreRecommenderUser::makeNew() {
    GenreRecommenderUser* gu=new GenreRecommenderUser(this->getName());
    for(unsigned int i=0;i<history.size();i++){
        gu->history.push_back(history.at(i)->makeNew());
    }
    return gu;
}