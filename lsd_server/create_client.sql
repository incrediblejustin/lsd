--
-- Create The Client
--

CREATE TABLE Client
(
    name varchar(250) NOT NULL PRIMARY KEY,
    pwd  varchar(64) NOT NULL    
)default charset utf8;

--
-- Populate the table 'Client'
--

INSERT INTO Client(name, pwd) VALUES ('Sunxiaoqiang','1');
INSERT INTO Client(name, pwd) VALUES ('liubei','12');
INSERT INTO Client(name, pwd) VALUES ('caocao','123');


