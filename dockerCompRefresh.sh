#!/bin/bash

docker compose stop
docker compose up -d --build --force-recreate
docker image prune -f