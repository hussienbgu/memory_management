#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include <string>
#include <iostream>
#include <vector>
#include <typeinfo>
extern Session* s2;
BaseAction::BaseAction():errorMsg(),status(PENDING) {}
ActionStatus BaseAction::getStatus() const {
    return status;
}
void BaseAction::complete() {
    status=COMPLETED;
}
void BaseAction::error(const std::string &errorMsg) {
    this->errorMsg= std::move(errorMsg);
    status=ERROR;
}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
BaseAction::~BaseAction() {}

///Class CreateUser
CreateUser ::CreateUser(std::string username,std::string algo1):username(username),algo(algo1){}
void CreateUser ::act(Session &sess) {
    if (sess.Contain_User_Name(username) == true) {
        this->error("username already used");
        std::cout << "Error: " + getErrorMsg() << std::endl;
    }
   else if (algo != "len" && algo != "rer" && algo != "gen") {
        this->error("invalid recommendation algorithm");
        std::cout << "Error: " + getErrorMsg() << std::endl;
    }
   else if (algo == "len") {
        User *user = new LengthRecommenderUser(username);
        sess.updateUsersList(user);
        this->complete();
    }
    else if (algo == "rer") {
        User *user = new RerunRecommenderUser(username);
        sess.updateUsersList(user);
        this->complete();

    }
   else if (algo == "gen") {
        User *user = new GenreRecommenderUser(username);
        sess.updateUsersList(user);
        this->complete();
    }




}
std::string CreateUser::toString() const {
    std::string output="createuser ";
    return output;
}
BaseAction* CreateUser::makeNew(const CreateUser &other) {
    CreateUser* cr=new CreateUser(other.username,other.algo);
    if(other.getStatus()==ERROR){
        cr->error(other.getErrorMsg());
    }
    return cr;
}
CreateUser::~CreateUser() {}

///Class ChangeactiveUser
ChangeActiveUser :: ChangeActiveUser(std:: string username):activeusername(username) {}
void ChangeActiveUser ::act(Session &sess) {
    if (sess.Contain_User_Name(activeusername)==false) {
        this->error("there's no such username");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    User* NewActiveUser = sess.getUser_byName(activeusername);
    if(NewActiveUser != nullptr) {
        sess.Update_active(NewActiveUser);
        this->complete();
    }
    else return;

}
std::string ChangeActiveUser ::toString() const {
    std::string output="changeuser ";
    return output;
}
BaseAction* ChangeActiveUser::makeNew(const ChangeActiveUser &other) {
    ChangeActiveUser* change=new ChangeActiveUser(other.activeusername);
    if(other.getStatus()==ERROR){
        change->error(other.getErrorMsg());
    }
    return change;
}
ChangeActiveUser::~ChangeActiveUser() {}

///Class DeleteUser
DeleteUser::DeleteUser(std::string username) :Username(username){}
void DeleteUser::act(Session &sess) {
       if (sess.Contain_User_Name(Username)==false) {
           this->error("there's no such username");
           std::cout << "Error: " + getErrorMsg() << std::endl;
       }
       else {
           sess.Remove_From_UserMap(sess.getUser_byName(Username));
           this->complete();
       }
}
std::string DeleteUser::toString() const {
    std::string output="deleteuser ";
    return output;
}
BaseAction* DeleteUser::makeNew(const DeleteUser &other) {
    DeleteUser* del=new DeleteUser(other.Username);
    if(other.getStatus()==ERROR){
        del->error(other.getErrorMsg());
    }
    return del;
}
DeleteUser::~DeleteUser() {}

///Class DuplicateUser
DuplicateUser :: DuplicateUser(std::string Orginal_username,std::string New_username):Orginal_username(Orginal_username),New_username(New_username){}
void DuplicateUser ::act(Session &sess) {
    if(sess.Contain_User_Name(Orginal_username)==false) {
        this->error("there's no such username");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    if(sess.Contain_User_Name(New_username)==true) {
        this->error("already taken");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else{
        User* OriginalUser= sess.getUser_byName(Orginal_username);
        if (LengthRecommenderUser* original= dynamic_cast<LengthRecommenderUser*>(OriginalUser)){
            User *newUser= new LengthRecommenderUser(New_username);
            for (unsigned int i=0; i<original->get_history().size(); i++){
                newUser->pushToHistory(original->get_history().at(i));
            }
            sess.updateUsersList(newUser);
            this->complete();
        }
        if (RerunRecommenderUser* original= dynamic_cast<RerunRecommenderUser*>(OriginalUser)){
            User *newUser= new RerunRecommenderUser(New_username);
            for (unsigned int i=0; i<original->get_history().size(); i++){
                newUser->pushToHistory(original->get_history().at(i));
            }
            sess.updateUsersList(newUser);
            this->complete();
        }
        if (GenreRecommenderUser* original= dynamic_cast<GenreRecommenderUser*>(OriginalUser)){
            User *newUser= new GenreRecommenderUser(New_username);
            for (unsigned int i=0; i<original->get_history().size(); i++){
                newUser->pushToHistory(original->get_history().at(i));
            }
            sess.updateUsersList(newUser);
            this->complete();
        }
    }
    }
std::string DuplicateUser::toString() const {
    std::string output="dupuser ";
    return output;
}
BaseAction* DuplicateUser::makeNew(const DuplicateUser &other) {
    DuplicateUser* du=new DuplicateUser(other.Orginal_username,other.New_username);
    if(other.getStatus()==ERROR){
        du->error(other.getErrorMsg());
    }
    return du;
}
DuplicateUser::~DuplicateUser() {}

///class PrintContentList
PrintContentList :: PrintContentList(){}
void PrintContentList::act(Session &sess) {
    int num=1;
    for(auto i :sess.getContent()){
           std::cout<<num<<"."<<i->toString()<<" "<<i->getLength()<<" minutes "<<i->printTags()<<std::endl;
        num++;
    }
    this->complete();
    }
std::string PrintContentList::toString() const {
    std::string output="Content ";
    return output;
}
BaseAction* PrintContentList::makeNew(const PrintContentList &other) {
    PrintContentList* pc=new PrintContentList();
    if(other.getStatus()==ERROR){
        pc->error(other.getErrorMsg());
    }
    return pc;
}
PrintContentList::~PrintContentList() {}

///class PrintWatchHistory
PrintWatchHistory ::PrintWatchHistory(){}
void PrintWatchHistory::act(Session &sess) {
    std::cout << "Watch history for " << sess.getActiveUserName() << std::endl;
    int num = 1;
    for (auto i:sess.get_Active_User()->get_history()) {
            std::cout << num << ". " << i->toString() << std::endl;
        num++;
    }
    this->complete();
}
std::string PrintWatchHistory::toString() const {
    return "watchhist";
}
BaseAction* PrintWatchHistory::makeNew(const PrintWatchHistory &other) {
    PrintWatchHistory* pw=new PrintWatchHistory();
    if(other.getStatus()==ERROR){
        pw->error(other.getErrorMsg());
    }
    return pw;
}
PrintWatchHistory::~PrintWatchHistory() {}

///class Watch
Watch::Watch(long id):id(id) {}
void Watch::act(Session &sess) {
        Watchable* watched_content = sess.getContent().at(id-1);
        std::cout<<"Watching "<<watched_content->toString()<<std::endl;
        sess.get_Active_User()->pushToHistory(watched_content);
        complete();

    }
std::string Watch::toString() const {
    return "watch";
}
BaseAction* Watch::makeNew(const Watch &other) {
    Watch* w=new Watch(id);
    if(other.getStatus()==ERROR){
        w->error(other.getErrorMsg());
    }
    return w;
}
Watch::~Watch() {}

/// class PrintActionsLog
PrintActionsLog ::PrintActionsLog() {}
void PrintActionsLog::act(Session &sess) {
    for (auto i:sess.getActionsLog()) {
        if (i->getStatus() == ERROR) {
            if (dynamic_cast<CreateUser *>(i)) {
                std::cout << i->toString() +" Error - username already used or invalid recommendation algorithm"<<std::endl;
            }
            if (dynamic_cast<ChangeActiveUser *>(i)) {
                std::cout << i->toString() + " Errror - there's no such username" << std::endl;
            }
            if (dynamic_cast<DeleteUser *>(i)) {
                std::cout << i->toString() + " Error - there's no such username" << std::endl;
            }
            if (dynamic_cast<DuplicateUser *>(i)) {
                std::cout << i->toString() + " Error - there's no such username or already taken" << std::endl;
            }
        }
            if (i->getStatus() != ERROR)
                std::cout << i->toString() + " Completed " << std::endl;
    }
}
PrintActionsLog::~PrintActionsLog() {}
std::string PrintActionsLog::toString() const {
    std::string output="log ";
    return output;
}
BaseAction* PrintActionsLog::makeNew(const PrintActionsLog &other) {
    PrintActionsLog* pa= new PrintActionsLog();
    if(other.getStatus()==ERROR){
        pa->error(other.getErrorMsg());
    }
    return pa;
}

///class Exit
Exit ::  Exit(){}
void Exit::act(Session &sess) {
    complete();
}
std::string Exit::toString() const {
    std::string output="exit";
    return output;
}
BaseAction* Exit::makeNew(const Exit &other) {
    Exit* e=new Exit();
    if(other.getStatus()==ERROR){
        e->error(other.getErrorMsg());
    }
    return e;
}
Exit::~Exit() {}