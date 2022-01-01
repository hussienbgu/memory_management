#ifndef ACTION_H_
#define ACTION_H_
#include <string>
#include <iostream>
class Session;
enum ActionStatus{
	PENDING, COMPLETED, ERROR
};
class BaseAction{
public:
	BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
    virtual ~BaseAction()=0;
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;

private:
	std::string errorMsg;
	ActionStatus status;
};
class CreateUser  : public BaseAction {
public:
	CreateUser(std::string username,std::string algo);
	virtual void act(Session &sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const CreateUser& other);
     ~CreateUser();

private:
	const std::string username;
	const std::string algo;

};
class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(std::string username);
	virtual void act(Session& sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const ChangeActiveUser& other);
    ~ChangeActiveUser();
private:
	const std::string activeusername;
};
class DeleteUser : public BaseAction {
public:
    DeleteUser(std::string username);
	virtual void act(Session & sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const DeleteUser& other);
     ~DeleteUser();
private:
	const std::string Username;
};
class DuplicateUser : public BaseAction {
public:
    DuplicateUser(std:: string Orginal_username,std::string New_username);
	virtual void act(Session & sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const DuplicateUser& other);
    ~DuplicateUser();

private:
    std::string Orginal_username;
	std::string New_username;
};
class PrintContentList : public BaseAction {
public:
    PrintContentList();
	virtual void act (Session& sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const PrintContentList& other);
     ~PrintContentList();
};
class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
	virtual void act (Session& sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const PrintWatchHistory& other);
    ~PrintWatchHistory();
};
class Watch : public BaseAction {
public:
    Watch(long id);
	virtual void act(Session& sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const Watch& other);
     ~Watch();
private:
	const long id;
};
class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const PrintActionsLog& other);
  ~PrintActionsLog();
};
class Exit : public BaseAction {
public:
    Exit();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    BaseAction* makeNew(const Exit& other);
    ~Exit();
};
#endif
