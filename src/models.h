#ifndef _MODELS_PASS_H_
#define _MODELS_PASS_H_
class User {
private :
	std::string username;
	std::string password;
	std::string hashed_password;
	bool anonymous;

public :
	User(std::string username, std::string password, std::string hashed_password) {
		this->username = username;
		this->password = password;
		this->hashed_password;
		this->anonymous = false;
	}

	User() {
		this->anonymous = true;
	}

	User(User &u) {
		this->username = u.username;
		this->password = u.password;
		this->anonymous = u.anonymous;
	}

	bool is_anonymous() {
		return this->anonymous;
	}

	std::string get_username() {
		return this->username;
	}

	std::string get_password() {
		return this->password;
	}

	void logout() {
		this->username = "";
		this->password = "";
		this->hashed_password = "";
		this->anonymous = true;
	}
};
#endif
