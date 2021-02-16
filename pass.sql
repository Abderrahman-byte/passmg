CREATE TABLE user (
	id SERIAL PRIMARY KEY,
	username VARCHAR(200) UNIQUE NOT NULL,
	password TEXT NOT NULL
);

CREATE TABLE password (
	id SERIAL PRIMARY KEY,
	title VARCHAR(200) NOT NULL,
	user_id INTEGER REFERENCES user(id),
	content TEXT NOT NULL,
	UNIQUE (title, user_id)
);
