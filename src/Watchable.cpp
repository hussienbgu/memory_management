#include "../include/Watchable.h"
#include <iostream>
#include <vector>
#include "../include/User.h"
#include "../include/Session.h"
Watchable::Watchable(long w_id, int w_length, const std::vector<std::string> &w_tags) : id(w_id),length(w_length),tags(w_tags){}
long Watchable::getId() const {
    return id;
}
int Watchable::getLength() const {
    return length;
}
std::vector<std::string> Watchable::getTags() const {
    return tags;
}
std::string Watchable::printTags() {
    std::string output="[";
    for (unsigned int i=0; i<Watchable::getTags().size(); i++){
        output=output+Watchable::getTags()[i]+",";
    }
    output.pop_back();
    output.push_back(']');
    return output;
}
Watchable::~Watchable() {}
//MOVIE CLASS
    Movie::Movie(long id,const std::string &name,int length,const std::vector<std::string> &tags):Watchable(id, length, tags),name(name) {}
    Movie::~Movie() {}
    Watchable *Movie::getNextWatchable(Session &s) const {
    Watchable* output=s.get_Active_User()->getRecommendation(s);
        return output;

    }
    std::string Movie::getMovieName() {
        return name;
    }
    std::string Movie::toString() const {
        return name;
    }
    Watchable* Movie::makeNew() {
    Movie* m=new Movie(getId(),getMovieName(),getLength(),getTags());
        return m;
    }


//SERIES CLASS
    Episode::Episode(long id, const std::string &seriesName, int length,
            int season, int episode, const std::vector<std::string> &tags) : Watchable(id, length, tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(0){}

    Watchable *Episode::getNextWatchable(Session &sess) const {
        Watchable *next = nullptr;
        if (this->nextEpisodeId==0){
            next=sess.get_Active_User()->getRecommendation(sess);
            return next;
        }
        else{
            next=sess.getWatchablebyID(getnextEpisodeId());
            if (sess.get_Active_User()->nonwatched(next)){
                return next;
            }
            else return next->getNextWatchable(sess);
        }
    }
    int Episode::getEpisode() const {
        return episode;
    }
    std::string Episode::getSeriesName() const {
        return seriesName;
    }
    int Episode::getSeason() const {
        return season;
    }
    long Episode::getnextEpisodeId() const {
        return nextEpisodeId;
    }
    std::string Episode::toString() const {
    std::string output= seriesName+" S" + std::to_string(season) +"E" + std::to_string(episode);
        return output;
    }

    void ::Episode::setnextEpisodeId(long nextEpisodeID) {
        this->nextEpisodeId=nextEpisodeID;
    }
    Episode::~Episode() {}
    Watchable* Episode::makeNew() {
        return new Episode(getId(),getSeriesName(),getLength(),getSeason(),getEpisode(),getTags());
}




