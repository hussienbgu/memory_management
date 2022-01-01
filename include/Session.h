#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include <iostream>
#include "User.h"


class User;
class Watchable;

class Session{
public:
    Session();
    Session(const std::string &configFilePath);
    //rull of five
    virtual ~Session();   //destructor;
    Session(const Session& other);///copy constructor;
    Session& operator=(const Session& other);  //copy assignment operator;
    Session(Session&& other);  //move constructor;
    Session& operator=(Session &&other);  //move assignment operator;
    void start();// start function;
    bool  Contain_User_Name(std::string Username);// true if the userMap contains the User;
    std::vector<User*>&  getUsersMap();       // get the userMap;
    std::vector<Watchable*>& getContent();     // get the Content;
    std::vector<BaseAction*>& getActionsLog(); // get the Action Log;
    User* get_Active_User();// get the Active user;
    std::string getActiveUserName();// get the activeuser name;
    void Update_active(User* newActive);// set the activeuser to be NewActive;
    User* getUser_byName(std::string username);// return the user his name is username
    void updateUsersList(User* user);// add user to the user map
    Watchable* getWatchablebyID(long id);//return the watchable with ID=id;
    void Remove_From_UserMap(User* u);// remove u from the userMap;

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::vector<User*> usersMap;
    User* activeUser;
};
#endif

