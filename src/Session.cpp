#include "../include/Session.h"
#include "../include/User.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "../include/json.hpp"
#include <vector>
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/User.h"
Session::Session():content(),actionsLog(),usersMap(),activeUser(){}
//Rule of five
Session::~Session() {
    for(auto u:usersMap)
        delete u;
    for (auto c:content)
        delete c;
    for (auto a:actionsLog)
        delete a;
}//destructor Done
Session::Session(const Session &other):content(),actionsLog(),usersMap(),activeUser() {
    for (auto &user:other.usersMap) {
        if (user->getName() == other.activeUser->getName()) {

                activeUser = other.activeUser->makeNew();
                usersMap.push_back(activeUser);
            }
            else {
            usersMap.push_back(user->makeNew());
        }
    }

    for (auto &W:other.content) {
                if (auto m = dynamic_cast<Movie *>(W)) {
                    content.push_back(m->makeNew());
                }
                if (auto e = dynamic_cast<Episode *>(W)) {
                    content.push_back(e->makeNew());
        }
    }
    //Action log Done
    for(auto &action:other.actionsLog) {
        if(auto ac= dynamic_cast<CreateUser*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
        if(auto ac= dynamic_cast<DeleteUser*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
        if(auto ac= dynamic_cast<DuplicateUser*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
        if(auto ac= dynamic_cast<ChangeActiveUser*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
        if(auto ac= dynamic_cast<Watch*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
        if(auto ac= dynamic_cast<PrintContentList*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
        if(auto ac= dynamic_cast<PrintWatchHistory*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
        if(auto ac= dynamic_cast<PrintActionsLog*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
        if(auto ac= dynamic_cast<Exit*>(action)){
            actionsLog.push_back(ac->makeNew(*ac));
        }
    }
}//Copy constructor
Session& Session::operator=(const Session &other) {
    if(this!=&other){
        for (auto &cont:content){
            delete cont;
        }
        content.clear();
        for(auto &cont:other.content) {
            content.push_back(cont->makeNew());
        }
        for(auto &user :usersMap){
            delete user;
        }
        usersMap.clear();
       for(auto user:other.usersMap){
            usersMap.push_back(user->makeNew());
        }
        for(auto &action:actionsLog){
            delete action;
        }
        actionsLog.clear();
        for(auto action:other.actionsLog){
            if(auto ac= dynamic_cast<CreateUser*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }
            if(auto ac= dynamic_cast<DeleteUser*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }
            if(auto ac= dynamic_cast<DuplicateUser*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }
            if(auto ac= dynamic_cast<ChangeActiveUser*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }
            if(auto ac= dynamic_cast<Watch*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }
            if(auto ac= dynamic_cast<PrintContentList*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }
            if(auto ac= dynamic_cast<PrintWatchHistory*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }
            if(auto ac= dynamic_cast<PrintActionsLog*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }
            if(auto ac= dynamic_cast<Exit*>(action)){
                actionsLog.push_back(ac->makeNew(*ac));
            }

        }
    }
    return (*this);
}////copy assignment operator
Session::Session(Session &&other):content(other.content),actionsLog(other.actionsLog),usersMap(other.usersMap),activeUser(other.activeUser) {
    other.activeUser= nullptr;
    other.actionsLog.clear();
    other.actionsLog.shrink_to_fit();
    other.content.clear();
    other.content.shrink_to_fit();
    other.usersMap.clear();
    other.usersMap.shrink_to_fit();
}//move constructor done
Session& Session::operator=(Session &&other) {
    if(this!=&other) {
        activeUser = other.activeUser;
        content=other.content;
        usersMap = other.usersMap;
        actionsLog = other.actionsLog;
        other.usersMap.clear();
        other.usersMap.shrink_to_fit();
        other.actionsLog.clear();
        other.actionsLog.shrink_to_fit();
        other.content.clear();
        other.content.shrink_to_fit();
        other.activeUser= nullptr;
    }
    return (*this);
}//move assignment operator Done
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Session::Session(const std::string &configFilePath):content(),actionsLog(),usersMap(),activeUser() {
    nlohmann::json j;
    std::ifstream infile(configFilePath);
    infile>>j;
    int id=1;
    //READING THE CONFIG.JSON FILE
    //Adding the movies to the content
    for (unsigned int i = 0 ; i < j["movies"].size();i++) {// Movies
        std::string movie_name=j["movies"][i]["name"];//get movie name
        int movie_length=j["movies"][i]["length"];// get movie length
        std::vector<std::string> Tags=j["movies"][i]["tags"];// get movie tags
        content.push_back(new Movie(id, movie_name, movie_length, Tags));
        id++;
    }
    //Adding the tv series to the content;
    for (unsigned int k = 0; k <j["tv_series"].size() ; k++) {
        std::string Ep_name = j["tv_series"][k]["name"];// get Episode name
        int Ep_length=j["tv_series"][k]["episode_length"];//get Episode length
        std::vector<std::string> Tags=j["tv_series"][k]["tags"];//get Episode Tags
        for (unsigned int l = 0; l < j["tv_series"][k]["seasons"].size(); l++) {
            for (unsigned int m = 1; m <=j["tv_series"][k]["seasons"][l]; m++) {
                Episode* ep=new Episode(id,Ep_name,Ep_length,l+1,m,Tags);
                content.push_back(ep);
                id=id+1;
                ep->setnextEpisodeId(id);
            }
            Episode* lastep= dynamic_cast<Episode *>(content.at(content.size() - 1));
            lastep->setnextEpisodeId(0);
        }
    }
    //Creating the deafult user;
    User* deuser= new LengthRecommenderUser("deafult");
    usersMap.push_back(deuser);
    activeUser=deuser;
}
void Session::start() {
    std::cout << "SPLFLIX is now on!" << std::endl;
    std::string action;
    bool stop = false;
    while (!stop) {
        getline(std::cin, action);

        if (action.find("createuser") != std::string::npos) {//done
            std::string action1 = action.substr(11);
            std::string newUserName = action1.substr(0, action1.find(' '));
            action1 = action1.substr(action1.find(' ') + 1);
            BaseAction *b1 = new CreateUser(newUserName, action1);
            b1->act(*this);
            actionsLog.push_back(b1);
        }

        if (action.find("changeuser") != std::string::npos) {//done
            std::string newUserName = action.substr(11);
            BaseAction *b1 = new ChangeActiveUser(newUserName);
            b1->act(*this);
            actionsLog.push_back(b1);
        }
        if (action.find("deleteuser") != std::string::npos) {//done
            std::string UserName = action.substr(11);
            BaseAction *b1 = new DeleteUser(UserName);
            b1->act(*this);
            actionsLog.push_back(b1);
        }

        if (action.find("dupuser") != std::string::npos) {//done
            std::string action1 = action.substr(8);
            std::string OriginalUserName = action1.substr(0, action1.find(' '));
            std::string NewUserName = action1.substr(action1.find(' ') + 1);
            BaseAction *b1 = new DuplicateUser(OriginalUserName, NewUserName);
            b1->act(*this);
            actionsLog.push_back(b1);
        }
        if (action.find("content") != std::string::npos) {

            BaseAction *b1 = new PrintContentList();
            b1->act(*this);
            actionsLog.push_back(b1);

        }
        if (action.find("watchhist") != std::string::npos) {//Error
            BaseAction *b1 = new PrintWatchHistory();
            b1->act(*this);
            actionsLog.push_back(b1);
        }

        if (action.find("watch ") != std::string::npos) {
            std::string id = action.substr(action.find(' ') + 1);
            long content_id = stol(id);
            BaseAction *watch_action = new Watch(content_id);
            actionsLog.push_back(watch_action);
            watch_action->act(*this);
            bool continue_watching = true;
            if (watch_action->getStatus() == ERROR)
                continue_watching = false;

            bool flag = false;
            while (continue_watching) {
                Watchable *recommend = nullptr;
                if (!flag) {
                    recommend = get_Active_User()->get_history().at(
                            get_Active_User()->get_history().size() - 1)->getNextWatchable(*this);
                    if (recommend == nullptr) {
                        break;
                    }
                    std::cout << "We recommend watching " << recommend->toString() << ", continue watching ? [y/n]"
                              << std::endl;
                    flag = true;
                }
                std::string answer;
                getline(std::cin, answer);
                if (answer == "n") {
                    continue_watching = false;
                } else if (answer == "y") {
                    content_id = recommend->getId();
                    BaseAction *another_watch_action = new Watch(content_id);
                    another_watch_action->act(*this);
                    actionsLog.push_back(another_watch_action);
                    if (another_watch_action->getStatus() == ERROR)
                        continue_watching = false;
                    else
                        flag = false;
                }
            }
        }
        if (action.find("log") != std::string::npos) {//done
            BaseAction *b1 = new PrintActionsLog();
            b1->act(*this);
            actionsLog.push_back(b1);
        }
        if (action.find("exit") != std::string::npos) {//done
            BaseAction *b1 = new Exit();
            b1->act(*this);
            actionsLog.push_back(b1);
            stop = true;
        }
    }
}
bool Session::Contain_User_Name(std::string Username) {
    for (unsigned int i = 0; i <usersMap.size() ; ++i) {
        if(usersMap.at(i)->getName()==Username)
            return true;
    }
    return false;
}
void Session::updateUsersList(User* user) {
      usersMap.push_back(user);
}
void Session::Remove_From_UserMap(User* user){
    for(unsigned int i=0;i<usersMap.size();i++){
        if(usersMap.at(i)==user){
            usersMap.erase(usersMap.begin()+i);
            break;
        }

    }
    delete(user);// delete the pointer after delete the user from the user map.
    return;
}
std::vector <User*>&  Session::getUsersMap() {
    return usersMap;
}
std::vector<Watchable*>& Session::getContent() {
    return  content;
}
std::vector<BaseAction*>& Session::getActionsLog() {
    return actionsLog;
}
 User* Session::get_Active_User() {
    return activeUser;
}
std::string Session::getActiveUserName() {
    return Session::activeUser->getName();
}
void Session::Update_active(User*newActive) {
    this->activeUser=newActive;
}
User* Session::getUser_byName(std::string username) {
    for(auto user:this->getUsersMap()) {
        if (user->getName() == username)
            return user;
    }
    return nullptr;
}
Watchable* Session::getWatchablebyID(long id) {
    for(auto w:content){
        if(w->getId()==id)
            return w;
    }
    return nullptr;
}
