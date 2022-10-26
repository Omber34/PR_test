create database pr_test;
use pr_test;

CREATE TABLE Users
(
    id            serial       not null unique auto_increment,
    name          varchar(255) not null,
    userName      varchar(255) not null unique,
    password_hash varchar(255) not null,
    
    CONSTRAINT PR_Users primary key (id)
);
insert into users(name, userName, password_hash) values (?, ?, ?);
insert into Users(name, userName, password_hash) values ('root', 'root1232', '123');
delete from Users where id = ?;

alter table Users add column friend_list_pk bigint;
CREATE TABLE Friendship ( 
    RequesterId     INT      NOT NULL,
    AddresseeId     INT      NOT NULL, -- Fixed with a well-delimited data type.
    CreatedDateTime DATETIME NOT NULL,
    --
    CONSTRAINT Friendship_PK            PRIMARY KEY (RequesterId, AddresseeId),
    CONSTRAINT FriendshipToRequester_FK FOREIGN KEY (RequesterId)
        REFERENCES UserProfile (UserId),
    CONSTRAINT FriendshipToAddressee_FK FOREIGN KEY (AddresseeId)
        REFERENCES UserProfile (UserId),
    CONSTRAINT FriendsAreDistinct_CK    CHECK       (RequesterId <> AddresseeId)
);
