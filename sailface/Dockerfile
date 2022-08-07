FROM gcc:9

RUN set -ex; \
    apt-get update; \
    apt-get install -y cmake


RUN mkdir /sailface
WORKDIR /sailface


# install ARDUINO CLI
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
