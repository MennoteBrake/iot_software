-- drop table data;
create table data (
    id integer primary key autoincrement,
    dataSensor1 integer,
    dataSensor2 integer
);

insert into data (dataSensor1, dataSensor2) values (
    20,
    10
);
insert into data (dataSensor1, dataSensor2) values (
    630,
    200
);