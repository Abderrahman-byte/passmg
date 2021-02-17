CREATE TABLE user (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	username VARCHAR(200) UNIQUE NOT NULL,
	password TEXT NOT NULL
);

CREATE TABLE password (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	title VARCHAR(200) NOT NULL,
	user_id INTEGER REFERENCES user(id),
	content TEXT NOT NULL,
	UNIQUE (title, user_id)
);
