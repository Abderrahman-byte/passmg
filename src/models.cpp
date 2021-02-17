#include <string>

class User {
private :
	std::string username;
	std::string password;
	bool anonymous;

public :
	User(std::string username, std::string password) {
		this->username = username;
		this->password = password;
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
};
