FROM ubuntu:22.04
WORKDIR /usr/src/http-server
EXPOSE 7777
# COPY build . 
RUN apt-get update
RUN apt-get install libpq-dev -y
COPY ./http-server-client-dcsa.deb .
RUN dpkg -i ./http-server-client-dcsa.deb
CMD [ "http-server" ]