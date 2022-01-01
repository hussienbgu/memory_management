#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags); //done
    virtual std::string toString() const=0;    //done
    virtual Watchable* getNextWatchable(Session&) const=0;
    virtual ~Watchable();
    long getId()const;                           //done
    int getLength()const ;                       //done
    std::vector<std::string> getTags() const;    //done
    std::string printTags();   //done
    virtual Watchable* makeNew()=0;


private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags); //done
    virtual std::string toString() const;                                                      //done
    virtual Watchable* getNextWatchable(Session&) const;
    std::string getMovieName();
    ~Movie();
    Watchable* makeNew();
private:
    std::string name;
};



class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags); //done
    virtual std::string toString() const;            //done
    virtual Watchable* getNextWatchable(Session&) const; //page3
    int getSeason() const;             //done
    int getEpisode() const;            //done
    long getnextEpisodeId() const ;     //done
    void setnextEpisodeId(long nextEpisodeID); //done
    std::string getSeriesName() const; //done
    ~Episode();
    Watchable* makeNew();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId ;
};

#endif
