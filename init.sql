CREATE SCHEMA IF NOT EXISTS public;

create table if not exists users
(
    id          serial              constraint users_pk     primary key,
    name        text    not null    constraint users_name   unique,   
    password    text    not null,
    role        text    not null,
    info        text
);

alter table users
    owner to admin;
