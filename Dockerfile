FROM ubuntu:22.04
WORKDIR /usr/src/http-server
COPY ./http-server-client-dcsa.deb .
RUN dpkg -i ./http-server-client-dcsa.deb
CMD [ "./http-server" ]